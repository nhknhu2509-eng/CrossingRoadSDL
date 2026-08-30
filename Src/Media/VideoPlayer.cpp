#include "Media/VideoPlayer.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <algorithm>
#include <cmath>

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


// ======================================================
// CONSTRUCTOR
// ======================================================

VideoPlayer::VideoPlayer()
{
}


// ======================================================
// DESTRUCTOR
// ======================================================

VideoPlayer::~VideoPlayer()
{
}


// ======================================================
// PLAY
// ======================================================

bool VideoPlayer::Play(
    SDL_Renderer* renderer,
    const std::string& videoPath,
    bool& quitRequested)
{
    quitRequested = false;


    // ==================================================
    // VIDEO FORMAT CONTEXT
    // ==================================================

    AVFormatContext* videoFormatContext = nullptr;


    if (avformat_open_input(
        &videoFormatContext,
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
        videoFormatContext,
        nullptr) < 0)
    {
        std::cout
            << "Cannot find video stream information."
            << std::endl;

        avformat_close_input(
            &videoFormatContext);

        return false;
    }


    // ==================================================
    // FIND VIDEO STREAM
    // ==================================================

    int videoStreamIndex = -1;


    for (unsigned int i = 0;
        i < videoFormatContext->nb_streams;
        ++i)
    {
        if (
            videoFormatContext
            ->streams[i]
            ->codecpar
            ->codec_type
            ==
            AVMEDIA_TYPE_VIDEO)
        {
            videoStreamIndex =
                static_cast<int>(i);

            break;
        }
    }


    if (videoStreamIndex == -1)
    {
        std::cout
            << "Video stream not found."
            << std::endl;

        avformat_close_input(
            &videoFormatContext);

        return false;
    }


    // ==================================================
    // VIDEO DECODER
    // ==================================================

    AVCodecParameters* videoParameters =
        videoFormatContext
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
            &videoFormatContext);

        return false;
    }


    AVCodecContext* videoCodecContext =
        avcodec_alloc_context3(
            videoCodec);


    if (videoCodecContext == nullptr)
    {
        avformat_close_input(
            &videoFormatContext);

        return false;
    }


    if (avcodec_parameters_to_context(
        videoCodecContext,
        videoParameters) < 0)
    {
        avcodec_free_context(
            &videoCodecContext);

        avformat_close_input(
            &videoFormatContext);

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
            &videoFormatContext);

        return false;
    }


    // ==================================================
    // AUDIO
    //
    // Audio is decoded completely BEFORE video playback.
    //
    // This prevents the video frame delay from starving
    // SDL's audio queue.
    // ==================================================

    AVFormatContext* audioFormatContext = nullptr;

    AVCodecContext* audioCodecContext = nullptr;

    SwrContext* swrContext = nullptr;

    SDL_AudioDeviceID audioDevice = 0;

    bool audioEnabled = false;

    std::vector<uint8_t> completeAudio;


    const int outputSampleRate = 48000;

    const int outputChannels = 2;


    // ==================================================
    // OPEN SECOND FORMAT CONTEXT FOR AUDIO
    // ==================================================

    if (avformat_open_input(
        &audioFormatContext,
        videoPath.c_str(),
        nullptr,
        nullptr) >= 0)
    {
        if (avformat_find_stream_info(
            audioFormatContext,
            nullptr) >= 0)
        {
            int audioStreamIndex = -1;


            // ==========================================
            // FIND AUDIO STREAM
            // ==========================================

            for (unsigned int i = 0;
                i < audioFormatContext->nb_streams;
                ++i)
            {
                if (
                    audioFormatContext
                    ->streams[i]
                    ->codecpar
                    ->codec_type
                    ==
                    AVMEDIA_TYPE_AUDIO)
                {
                    audioStreamIndex =
                        static_cast<int>(i);

                    break;
                }
            }


            if (audioStreamIndex != -1)
            {
                AVCodecParameters* audioParameters =
                    audioFormatContext
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
                        // ==============================
                        // RESAMPLER
                        // ==============================

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
                            // ==========================
                            // DECODE ALL AUDIO
                            // ==========================

                            AVPacket* audioPacket =
                                av_packet_alloc();


                            AVFrame* audioFrame =
                                av_frame_alloc();


                            if (
                                audioPacket != nullptr &&
                                audioFrame != nullptr)
                            {
                                while (
                                    av_read_frame(
                                        audioFormatContext,
                                        audioPacket) >= 0)
                                {
                                    if (
                                        audioPacket
                                        ->stream_index
                                        ==
                                        audioStreamIndex)
                                    {
                                        if (
                                            avcodec_send_packet(
                                                audioCodecContext,
                                                audioPacket) >= 0)
                                        {
                                            while (
                                                avcodec_receive_frame(
                                                    audioCodecContext,
                                                    audioFrame) >= 0)
                                            {
                                                // ==================
                                                // OUTPUT SAMPLE COUNT
                                                // ==================

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


                                                int bufferSize =
                                                    av_samples_get_buffer_size(
                                                        nullptr,

                                                        outputChannels,

                                                        outputSamples,

                                                        AV_SAMPLE_FMT_S16,

                                                        1);


                                                if (bufferSize > 0)
                                                {
                                                    std::vector<uint8_t>
                                                        buffer(
                                                            bufferSize);


                                                    uint8_t* outputData[1] =
                                                    {
                                                        buffer.data()
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
                                                            size_t oldSize =
                                                                completeAudio
                                                                .size();


                                                            completeAudio
                                                                .resize(
                                                                    oldSize
                                                                    +
                                                                    convertedSize);


                                                            std::copy(
                                                                buffer.begin(),

                                                                buffer.begin()
                                                                +
                                                                convertedSize,

                                                                completeAudio.begin()
                                                                +
                                                                oldSize);
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }


                                    av_packet_unref(
                                        audioPacket);
                                }


                                // ======================
                                // FLUSH AUDIO DECODER
                                // ======================

                                avcodec_send_packet(
                                    audioCodecContext,
                                    nullptr);


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


                                    int bufferSize =
                                        av_samples_get_buffer_size(
                                            nullptr,

                                            outputChannels,

                                            outputSamples,

                                            AV_SAMPLE_FMT_S16,

                                            1);


                                    if (bufferSize > 0)
                                    {
                                        std::vector<uint8_t>
                                            buffer(
                                                bufferSize);


                                        uint8_t* outputData[1] =
                                        {
                                            buffer.data()
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
                                                size_t oldSize =
                                                    completeAudio
                                                    .size();


                                                completeAudio
                                                    .resize(
                                                        oldSize
                                                        +
                                                        convertedSize);


                                                std::copy(
                                                    buffer.begin(),

                                                    buffer.begin()
                                                    +
                                                    convertedSize,

                                                    completeAudio.begin()
                                                    +
                                                    oldSize);
                                            }
                                        }
                                    }
                                }


                                av_packet_free(
                                    &audioPacket);


                                av_frame_free(
                                    &audioFrame);
                            }
                        }
                    }
                }
            }
        }
    }


    // ==================================================
    // OPEN SDL AUDIO DEVICE
    // ==================================================

    if (!completeAudio.empty())
    {
        SDL_AudioSpec wantedSpec;
        SDL_AudioSpec obtainedSpec;

        SDL_zero(wantedSpec);
        SDL_zero(obtainedSpec);


        wantedSpec.freq =
            outputSampleRate;

        wantedSpec.format =
            AUDIO_S16SYS;

        wantedSpec.channels =
            static_cast<Uint8>(
                outputChannels);

        wantedSpec.samples =
            4096;

        wantedSpec.callback =
            nullptr;


        audioDevice =
            SDL_OpenAudioDevice(
                nullptr,

                0,

                &wantedSpec,

                &obtainedSpec,

                0);


        if (audioDevice != 0)
        {
            if (
                SDL_QueueAudio(
                    audioDevice,

                    completeAudio.data(),

                    static_cast<Uint32>(
                        completeAudio.size()))
                == 0)
            {
                audioEnabled = true;


                std::cout
                    << "Audio loaded: "
                    << completeAudio.size()
                    << " bytes"
                    << std::endl;
            }
            else
            {
                std::cout
                    << "SDL_QueueAudio error: "
                    << SDL_GetError()
                    << std::endl;
            }
        }
        else
        {
            std::cout
                << "SDL_OpenAudioDevice error: "
                << SDL_GetError()
                << std::endl;
        }
    }


    if (!audioEnabled)
    {
        std::cout
            << "Audio disabled or unavailable."
            << std::endl;
    }


    // ==================================================
    // VIDEO CONVERTER
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
            SDL_ClearQueuedAudio(
                audioDevice);

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


        if (audioFormatContext != nullptr)
        {
            avformat_close_input(
                &audioFormatContext);
        }


        avcodec_free_context(
            &videoCodecContext);


        avformat_close_input(
            &videoFormatContext);


        return false;
    }


    // ==================================================
    // SDL VIDEO TEXTURE
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
            SDL_ClearQueuedAudio(
                audioDevice);

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


        if (audioFormatContext != nullptr)
        {
            avformat_close_input(
                &audioFormatContext);
        }


        avcodec_free_context(
            &videoCodecContext);


        avformat_close_input(
            &videoFormatContext);


        return false;
    }


    // ==================================================
    // VIDEO FRAME
    // ==================================================

    AVFrame* decodedVideoFrame =
        av_frame_alloc();


    AVFrame* yuvFrame =
        av_frame_alloc();


    AVPacket* videoPacket =
        av_packet_alloc();


    if (
        decodedVideoFrame == nullptr ||
        yuvFrame == nullptr ||
        videoPacket == nullptr)
    {
        std::cout
            << "Cannot allocate video frames."
            << std::endl;


        av_frame_free(
            &decodedVideoFrame);


        av_frame_free(
            &yuvFrame);


        av_packet_free(
            &videoPacket);


        SDL_DestroyTexture(
            texture);


        sws_freeContext(
            swsContext);


        if (audioDevice != 0)
        {
            SDL_ClearQueuedAudio(
                audioDevice);

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


        if (audioFormatContext != nullptr)
        {
            avformat_close_input(
                &audioFormatContext);
        }


        avcodec_free_context(
            &videoCodecContext);


        avformat_close_input(
            &videoFormatContext);


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
    // CALCULATE DESTINATION RECT ONCE
    // ==================================================

    int windowWidth = 0;
    int windowHeight = 0;


    SDL_GetRendererOutputSize(
        renderer,

        &windowWidth,

        &windowHeight);


    double videoRatio =
        static_cast<double>(
            videoCodecContext->width)
        /
        static_cast<double>(
            videoCodecContext->height);


    double windowRatio =
        static_cast<double>(
            windowWidth)
        /
        static_cast<double>(
            windowHeight);


    SDL_Rect destination;


    if (windowRatio > videoRatio)
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


    // ==================================================
    // START AUDIO
    // ==================================================

    if (audioEnabled)
    {
        SDL_PauseAudioDevice(
            audioDevice,
            0);
    }


    // ==================================================
    // VIDEO CLOCK
    // ==================================================

    auto playbackStart =
        std::chrono
        ::steady_clock
        ::now();


    AVStream* videoStream =
        videoFormatContext
        ->streams[videoStreamIndex];


    AVRational frameRate =
        av_guess_frame_rate(

            videoFormatContext,

            videoStream,

            nullptr);


    double fps =
        av_q2d(
            frameRate);


    if (fps <= 0.0)
    {
        fps = 30.0;
    }


    double fallbackFrameDuration =
        1.0 / fps;


    long long frameNumber =
        0;


    bool playing =
        true;


    // ==================================================
    // VIDEO LOOP
    // ==================================================

    while (
        playing &&
        av_read_frame(
            videoFormatContext,
            videoPacket) >= 0)
    {
        // ==============================================
        // INPUT
        // ==============================================

        SDL_Event event;


        while (
            SDL_PollEvent(
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
                    event.key
                    .keysym
                    .sym;


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
                videoPacket);

            break;
        }


        // Ignore everything except video packets.
        if (
            videoPacket->stream_index
            !=
            videoStreamIndex)
        {
            av_packet_unref(
                videoPacket);

            continue;
        }


        // ==============================================
        // SEND VIDEO PACKET
        // ==============================================

        if (
            avcodec_send_packet(
                videoCodecContext,
                videoPacket) >= 0)
        {
            while (
                avcodec_receive_frame(
                    videoCodecContext,
                    decodedVideoFrame) >= 0)
            {
                // ======================================
                // FRAME PRESENTATION TIME
                // ======================================

                double targetSeconds;


                if (
                    decodedVideoFrame
                    ->best_effort_timestamp
                    !=
                    AV_NOPTS_VALUE)
                {
                    targetSeconds =
                        decodedVideoFrame
                        ->best_effort_timestamp
                        *
                        av_q2d(
                            videoStream
                            ->time_base);


                    if (targetSeconds < 0.0)
                    {
                        targetSeconds =
                            frameNumber
                            *
                            fallbackFrameDuration;
                    }
                }
                else
                {
                    targetSeconds =
                        frameNumber
                        *
                        fallbackFrameDuration;
                }


                // ======================================
                // WAIT UNTIL THIS FRAME SHOULD APPEAR
                //
                // Audio is already queued in SDL,
                // therefore waiting here does NOT starve it.
                // ======================================

                while (playing)
                {
                    auto current =
                        std::chrono
                        ::steady_clock
                        ::now();


                    double elapsedSeconds =
                        std::chrono
                        ::duration<double>(
                            current -
                            playbackStart)
                        .count();


                    double remaining =
                        targetSeconds -
                        elapsedSeconds;


                    if (remaining <= 0.001)
                    {
                        break;
                    }


                    // Sleep only a small amount so keyboard
                    // input remains responsive.
                    int sleepMilliseconds =
                        static_cast<int>(
                            std::min(
                                remaining * 1000.0,
                                5.0));


                    if (sleepMilliseconds < 1)
                    {
                        sleepMilliseconds = 1;
                    }


                    SDL_Delay(
                        static_cast<Uint32>(
                            sleepMilliseconds));


                    // ==================================
                    // CHECK SKIP/QUIT DURING WAIT
                    // ==================================

                    SDL_Event waitEvent;


                    while (
                        SDL_PollEvent(
                            &waitEvent))
                    {
                        if (
                            waitEvent.type ==
                            SDL_QUIT)
                        {
                            quitRequested =
                                true;


                            playing =
                                false;


                            break;
                        }


                        if (
                            waitEvent.type ==
                            SDL_KEYDOWN)
                        {
                            SDL_Keycode key =
                                waitEvent
                                .key
                                .keysym
                                .sym;


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
                }


                if (!playing)
                {
                    break;
                }


                // ======================================
                // CONVERT VIDEO FRAME TO YUV420
                // ======================================

                sws_scale(

                    swsContext,

                    decodedVideoFrame->data,

                    decodedVideoFrame->linesize,

                    0,

                    videoCodecContext->height,

                    yuvFrame->data,

                    yuvFrame->linesize);


                // ======================================
                // UPDATE SDL TEXTURE
                // ======================================

                SDL_UpdateYUVTexture(

                    texture,

                    nullptr,

                    yuvFrame->data[0],

                    yuvFrame->linesize[0],

                    yuvFrame->data[1],

                    yuvFrame->linesize[1],

                    yuvFrame->data[2],

                    yuvFrame->linesize[2]);


                // ======================================
                // RENDER
                // ======================================

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


                ++frameNumber;
            }
        }


        av_packet_unref(
            videoPacket);
    }


    // ==================================================
    // NORMAL VIDEO END
    //
    // If the video ends before the audio queue drains,
    // allow the remaining audio to finish.
    // ==================================================

    if (
        playing &&
        audioEnabled)
    {
        while (
            SDL_GetQueuedAudioSize(
                audioDevice) > 0)
        {
            SDL_Event event;


            while (
                SDL_PollEvent(
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
                        event.key
                        .keysym
                        .sym;


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
                break;
            }


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
    // CLEAN AUDIO
    // ==================================================

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


    if (audioFormatContext != nullptr)
    {
        avformat_close_input(
            &audioFormatContext);
    }


    // ==================================================
    // CLEAN VIDEO
    // ==================================================

    av_packet_free(
        &videoPacket);


    av_frame_free(
        &decodedVideoFrame);


    av_frame_free(
        &yuvFrame);


    SDL_DestroyTexture(
        texture);


    sws_freeContext(
        swsContext);


    avcodec_free_context(
        &videoCodecContext);


    avformat_close_input(
        &videoFormatContext);


    return true;
}