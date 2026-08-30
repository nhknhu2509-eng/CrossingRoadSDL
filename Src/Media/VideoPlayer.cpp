#include "Media/VideoPlayer.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <algorithm>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>

#include <libavutil/avutil.h>
#include <libavutil/channel_layout.h>
#include <libavutil/imgutils.h>
#include <libavutil/samplefmt.h>

#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
}


// ==================================================
// CONSTRUCTOR
// ==================================================

VideoPlayer::VideoPlayer()
{
}


// ==================================================
// DESTRUCTOR
// ==================================================

VideoPlayer::~VideoPlayer()
{
}


// ==================================================
// PLAY VIDEO
// ==================================================

bool VideoPlayer::Play(
    SDL_Renderer* renderer,
    const std::string& videoPath,
    bool& quitRequested)
{
    quitRequested = false;


    // ==================================================
    // FORMAT CONTEXT
    // ==================================================

    AVFormatContext* formatContext = nullptr;


    if (avformat_open_input(
        &formatContext,
        videoPath.c_str(),
        nullptr,
        nullptr) < 0)
    {
        std::cout
            << "Cannot open video: "
            << videoPath
            << std::endl;

        return false;
    }


    if (avformat_find_stream_info(
        formatContext,
        nullptr) < 0)
    {
        std::cout
            << "Cannot find stream information."
            << std::endl;

        avformat_close_input(
            &formatContext);

        return false;
    }


    // ==================================================
    // FIND VIDEO + AUDIO STREAM
    // ==================================================

    int videoStreamIndex = -1;
    int audioStreamIndex = -1;


    for (unsigned int i = 0;
        i < formatContext->nb_streams;
        ++i)
    {
        AVMediaType type =
            formatContext
            ->streams[i]
            ->codecpar
            ->codec_type;


        if (
            type == AVMEDIA_TYPE_VIDEO &&
            videoStreamIndex == -1)
        {
            videoStreamIndex =
                static_cast<int>(i);
        }


        if (
            type == AVMEDIA_TYPE_AUDIO &&
            audioStreamIndex == -1)
        {
            audioStreamIndex =
                static_cast<int>(i);
        }
    }


    if (videoStreamIndex == -1)
    {
        std::cout
            << "Video stream not found."
            << std::endl;

        avformat_close_input(
            &formatContext);

        return false;
    }


    // ==================================================
    // VIDEO DECODER
    // ==================================================

    AVCodecParameters* videoParameters =
        formatContext
        ->streams[videoStreamIndex]
        ->codecpar;


    const AVCodec* videoCodec =
        avcodec_find_decoder(
            videoParameters->codec_id);


    if (videoCodec == nullptr)
    {
        std::cout
            << "Video decoder not found."
            << std::endl;

        avformat_close_input(
            &formatContext);

        return false;
    }


    AVCodecContext* videoCodecContext =
        avcodec_alloc_context3(
            videoCodec);


    if (videoCodecContext == nullptr)
    {
        avformat_close_input(
            &formatContext);

        return false;
    }


    if (avcodec_parameters_to_context(
        videoCodecContext,
        videoParameters) < 0)
    {
        avcodec_free_context(
            &videoCodecContext);

        avformat_close_input(
            &formatContext);

        return false;
    }


    if (avcodec_open2(
        videoCodecContext,
        videoCodec,
        nullptr) < 0)
    {
        std::cout
            << "Cannot open video decoder."
            << std::endl;

        avcodec_free_context(
            &videoCodecContext);

        avformat_close_input(
            &formatContext);

        return false;
    }


    // ==================================================
    // AUDIO DECODER
    // ==================================================

    AVCodecContext* audioCodecContext =
        nullptr;

    SwrContext* swrContext =
        nullptr;

    SDL_AudioDeviceID audioDevice =
        0;

    bool audioEnabled =
        false;


    const int outputSampleRate =
        48000;

    const int outputChannels =
        2;


    if (audioStreamIndex != -1)
    {
        AVCodecParameters* audioParameters =
            formatContext
            ->streams[audioStreamIndex]
            ->codecpar;


        const AVCodec* audioCodec =
            avcodec_find_decoder(
                audioParameters->codec_id);


        if (audioCodec != nullptr)
        {
            audioCodecContext =
                avcodec_alloc_context3(
                    audioCodec);


            if (
                audioCodecContext != nullptr &&
                avcodec_parameters_to_context(
                    audioCodecContext,
                    audioParameters) >= 0 &&
                avcodec_open2(
                    audioCodecContext,
                    audioCodec,
                    nullptr) >= 0)
            {
                // ======================================
                // SDL AUDIO
                // ======================================

                SDL_AudioSpec wantedSpec;
                SDL_zero(wantedSpec);


                wantedSpec.freq =
                    outputSampleRate;

                wantedSpec.format =
                    AUDIO_S16SYS;

                wantedSpec.channels =
                    outputChannels;

                wantedSpec.samples =
                    4096;

                wantedSpec.callback =
                    nullptr;


                audioDevice =
                    SDL_OpenAudioDevice(
                        nullptr,
                        0,
                        &wantedSpec,
                        nullptr,
                        0);


                if (audioDevice != 0)
                {
                    // ==================================
                    // AUDIO RESAMPLER
                    // ==================================

                    AVChannelLayout outputLayout =
                        AV_CHANNEL_LAYOUT_STEREO;


                    int swrResult =
                        swr_alloc_set_opts2(
                            &swrContext,

                            &outputLayout,
                            AV_SAMPLE_FMT_S16,
                            outputSampleRate,

                            &audioCodecContext
                            ->ch_layout,

                            audioCodecContext
                            ->sample_fmt,

                            audioCodecContext
                            ->sample_rate,

                            0,
                            nullptr);


                    av_channel_layout_uninit(
                        &outputLayout);


                    if (
                        swrResult >= 0 &&
                        swrContext != nullptr &&
                        swr_init(
                            swrContext) >= 0)
                    {
                        audioEnabled =
                            true;


                        SDL_PauseAudioDevice(
                            audioDevice,
                            0);


                        std::cout
                            << "Audio enabled."
                            << std::endl;
                    }
                }
            }
        }
    }


    if (!audioEnabled)
    {
        std::cout
            << "Audio disabled or unavailable."
            << std::endl;
    }


    // ==================================================
    // VIDEO CONVERSION
    // ==================================================

    SwsContext* swsContext =
        sws_getContext(
            videoCodecContext->width,
            videoCodecContext->height,
            videoCodecContext->pix_fmt,

            videoCodecContext->width,
            videoCodecContext->height,
            AV_PIX_FMT_YUV420P,

            SWS_BILINEAR,
            nullptr,
            nullptr,
            nullptr);


    if (swsContext == nullptr)
    {
        if (audioDevice != 0)
        {
            SDL_CloseAudioDevice(
                audioDevice);
        }


        if (swrContext != nullptr)
        {
            swr_free(
                &swrContext);
        }


        if (audioCodecContext != nullptr)
        {
            avcodec_free_context(
                &audioCodecContext);
        }


        avcodec_free_context(
            &videoCodecContext);

        avformat_close_input(
            &formatContext);

        return false;
    }


    // ==================================================
    // SDL TEXTURE
    // ==================================================

    SDL_Texture* texture =
        SDL_CreateTexture(
            renderer,

            SDL_PIXELFORMAT_IYUV,

            SDL_TEXTUREACCESS_STREAMING,

            videoCodecContext->width,
            videoCodecContext->height);


    if (texture == nullptr)
    {
        std::cout
            << "Cannot create video texture: "
            << SDL_GetError()
            << std::endl;


        sws_freeContext(
            swsContext);


        if (audioDevice != 0)
        {
            SDL_CloseAudioDevice(
                audioDevice);
        }


        if (swrContext != nullptr)
        {
            swr_free(
                &swrContext);
        }


        if (audioCodecContext != nullptr)
        {
            avcodec_free_context(
                &audioCodecContext);
        }


        avcodec_free_context(
            &videoCodecContext);

        avformat_close_input(
            &formatContext);

        return false;
    }


    // ==================================================
    // FRAMES
    // ==================================================

    AVFrame* decodedVideoFrame =
        av_frame_alloc();


    AVFrame* yuvFrame =
        av_frame_alloc();


    AVFrame* audioFrame =
        av_frame_alloc();


    AVPacket* packet =
        av_packet_alloc();


    if (
        decodedVideoFrame == nullptr ||
        yuvFrame == nullptr ||
        audioFrame == nullptr ||
        packet == nullptr)
    {
        std::cout
            << "Cannot allocate FFmpeg frames."
            << std::endl;


        av_frame_free(
            &decodedVideoFrame);

        av_frame_free(
            &yuvFrame);

        av_frame_free(
            &audioFrame);

        av_packet_free(
            &packet);


        SDL_DestroyTexture(
            texture);


        sws_freeContext(
            swsContext);


        if (audioDevice != 0)
        {
            SDL_CloseAudioDevice(
                audioDevice);
        }


        if (swrContext != nullptr)
        {
            swr_free(
                &swrContext);
        }


        if (audioCodecContext != nullptr)
        {
            avcodec_free_context(
                &audioCodecContext);
        }


        avcodec_free_context(
            &videoCodecContext);

        avformat_close_input(
            &formatContext);

        return false;
    }


    // ==================================================
    // YUV BUFFER
    // ==================================================

    int yuvBufferSize =
        av_image_get_buffer_size(
            AV_PIX_FMT_YUV420P,

            videoCodecContext->width,
            videoCodecContext->height,

            1);


    std::vector<uint8_t>
        yuvBuffer(
            yuvBufferSize);


    av_image_fill_arrays(
        yuvFrame->data,
        yuvFrame->linesize,

        yuvBuffer.data(),

        AV_PIX_FMT_YUV420P,

        videoCodecContext->width,
        videoCodecContext->height,

        1);


    // ==================================================
    // VIDEO FPS
    // ==================================================

    AVRational frameRate =
        av_guess_frame_rate(
            formatContext,
            formatContext
            ->streams[videoStreamIndex],
            nullptr);


    double fps =
        av_q2d(frameRate);


    if (fps <= 0.0)
    {
        fps =
            30.0;
    }


    int frameDelay =
        static_cast<int>(
            1000.0 / fps);


    // ==================================================
    // PLAY LOOP
    // ==================================================

    bool playing =
        true;


    while (
        playing &&
        av_read_frame(
            formatContext,
            packet) >= 0)
    {
        // ==============================================
        // EVENTS
        // ==============================================

        SDL_Event event;


        while (SDL_PollEvent(
            &event))
        {
            if (
                event.type ==
                SDL_QUIT)
            {
                quitRequested =
                    true;

                playing =
                    false;

                break;
            }


            if (
                event.type ==
                SDL_KEYDOWN)
            {
                SDL_Keycode key =
                    event.key.keysym.sym;


                if (
                    key == SDLK_RETURN ||
                    key == SDLK_SPACE ||
                    key == SDLK_ESCAPE)
                {
                    playing =
                        false;

                    break;
                }
            }
        }


        if (!playing)
        {
            av_packet_unref(
                packet);

            break;
        }


        // ==============================================
        // VIDEO PACKET
        // ==============================================

        if (
            packet->stream_index ==
            videoStreamIndex)
        {
            if (
                avcodec_send_packet(
                    videoCodecContext,
                    packet) >= 0)
            {
                while (
                    avcodec_receive_frame(
                        videoCodecContext,
                        decodedVideoFrame) >= 0)
                {
                    auto frameStart =
                        std::chrono
                        ::steady_clock
                        ::now();


                    // ==================================
                    // CONVERT FRAME
                    // ==================================

                    sws_scale(
                        swsContext,

                        decodedVideoFrame
                        ->data,

                        decodedVideoFrame
                        ->linesize,

                        0,

                        videoCodecContext
                        ->height,

                        yuvFrame
                        ->data,

                        yuvFrame
                        ->linesize);


                    // ==================================
                    // UPDATE TEXTURE
                    // ==================================

                    SDL_UpdateYUVTexture(
                        texture,
                        nullptr,

                        yuvFrame->data[0],
                        yuvFrame->linesize[0],

                        yuvFrame->data[1],
                        yuvFrame->linesize[1],

                        yuvFrame->data[2],
                        yuvFrame->linesize[2]);


                    // ==================================
                    // WINDOW SIZE
                    // ==================================

                    int windowWidth =
                        0;

                    int windowHeight =
                        0;


                    SDL_GetRendererOutputSize(
                        renderer,

                        &windowWidth,
                        &windowHeight);


                    // ==================================
                    // KEEP VIDEO ASPECT RATIO
                    // ==================================

                    double videoRatio =
                        static_cast<double>(
                            videoCodecContext
                            ->width)
                        /
                        static_cast<double>(
                            videoCodecContext
                            ->height);


                    double windowRatio =
                        static_cast<double>(
                            windowWidth)
                        /
                        static_cast<double>(
                            windowHeight);


                    SDL_Rect destination;


                    if (
                        windowRatio >
                        videoRatio)
                    {
                        destination.h =
                            windowHeight;

                        destination.w =
                            static_cast<int>(
                                windowHeight *
                                videoRatio);

                        destination.x =
                            (windowWidth -
                                destination.w)
                            / 2;

                        destination.y =
                            0;
                    }
                    else
                    {
                        destination.w =
                            windowWidth;

                        destination.h =
                            static_cast<int>(
                                windowWidth /
                                videoRatio);

                        destination.x =
                            0;

                        destination.y =
                            (windowHeight -
                                destination.h)
                            / 2;
                    }


                    // ==================================
                    // RENDER
                    // ==================================

                    SDL_SetRenderDrawColor(
                        renderer,
                        0,
                        0,
                        0,
                        255);


                    SDL_RenderClear(
                        renderer);


                    SDL_RenderCopy(
                        renderer,
                        texture,
                        nullptr,
                        &destination);


                    SDL_RenderPresent(
                        renderer);


                    // ==================================
                    // FRAME DELAY
                    // ==================================

                    auto frameEnd =
                        std::chrono
                        ::steady_clock
                        ::now();


                    int elapsed =
                        static_cast<int>(
                            std::chrono
                            ::duration_cast<
                            std::chrono
                            ::milliseconds>(
                                frameEnd -
                                frameStart)
                            .count());


                    if (
                        elapsed <
                        frameDelay)
                    {
                        std::this_thread::sleep_for(
                            std::chrono
                            ::milliseconds(
                                frameDelay -
                                elapsed));
                    }
                }
            }
        }


        // ==============================================
        // AUDIO PACKET
        // ==============================================

        else if (
            audioEnabled &&
            packet->stream_index ==
            audioStreamIndex)
        {
            if (
                avcodec_send_packet(
                    audioCodecContext,
                    packet) >= 0)
            {
                while (
                    avcodec_receive_frame(
                        audioCodecContext,
                        audioFrame) >= 0)
                {
                    int outputSamples =
                        static_cast<int>(
                            av_rescale_rnd(
                                swr_get_delay(
                                    swrContext,
                                    audioCodecContext
                                    ->sample_rate)
                                +
                                audioFrame
                                ->nb_samples,

                                outputSampleRate,

                                audioCodecContext
                                ->sample_rate,

                                AV_ROUND_UP));


                    int outputBufferSize =
                        av_samples_get_buffer_size(
                            nullptr,

                            outputChannels,

                            outputSamples,

                            AV_SAMPLE_FMT_S16,

                            1);


                    if (
                        outputBufferSize <=
                        0)
                    {
                        continue;
                    }


                    std::vector<uint8_t>
                        audioBuffer(
                            outputBufferSize);


                    uint8_t* outputData[1] =
                    {
                        audioBuffer.data()
                    };


                    const uint8_t**
                        inputData =
                        const_cast<
                        const uint8_t**>(
                            audioFrame
                            ->extended_data);


                    int convertedSamples =
                        swr_convert(
                            swrContext,

                            outputData,
                            outputSamples,

                            inputData,

                            audioFrame
                            ->nb_samples);


                    if (
                        convertedSamples >
                        0)
                    {
                        int convertedSize =
                            av_samples_get_buffer_size(
                                nullptr,

                                outputChannels,

                                convertedSamples,

                                AV_SAMPLE_FMT_S16,

                                1);


                        if (
                            convertedSize >
                            0)
                        {
                            SDL_QueueAudio(
                                audioDevice,

                                audioBuffer
                                .data(),

                                static_cast<
                                Uint32>(
                                    convertedSize));
                        }
                    }
                }
            }
        }


        // ==============================================
        // RELEASE PACKET
        // ==============================================

        av_packet_unref(
            packet);
    }


    // ==================================================
    // WAIT FOR REMAINING AUDIO
    // ==================================================

    if (
        playing &&
        audioEnabled)
    {
        while (
            SDL_GetQueuedAudioSize(
                audioDevice) > 0)
        {
            SDL_Delay(
                10);
        }
    }


    // ==================================================
    // STOP AUDIO
    // ==================================================

    if (audioDevice != 0)
    {
        SDL_ClearQueuedAudio(
            audioDevice);


        SDL_CloseAudioDevice(
            audioDevice);
    }


    // ==================================================
    // CLEANUP
    // ==================================================

    av_packet_free(
        &packet);


    av_frame_free(
        &decodedVideoFrame);


    av_frame_free(
        &yuvFrame);


    av_frame_free(
        &audioFrame);


    SDL_DestroyTexture(
        texture);


    sws_freeContext(
        swsContext);


    if (swrContext != nullptr)
    {
        swr_free(
            &swrContext);
    }


    if (audioCodecContext != nullptr)
    {
        avcodec_free_context(
            &audioCodecContext);
    }


    avcodec_free_context(
        &videoCodecContext);


    avformat_close_input(
        &formatContext);


    return true;
}