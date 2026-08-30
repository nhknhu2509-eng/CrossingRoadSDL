#include "Media/VideoPlayer.h"

#include <iostream>
#include <chrono>
#include <thread>


extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
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
// PLAY
// ==================================================

bool VideoPlayer::Play(
    SDL_Renderer* renderer,
    const std::string& videoPath,
    bool& quitRequested)
{
    quitRequested =
        false;


    // ==================================================
    // CHECK RENDERER
    // ==================================================

    if (renderer == nullptr)
    {
        std::cout
            << "VideoPlayer Error: renderer is null."
            << std::endl;

        return false;
    }


    // ==================================================
    // OPEN VIDEO FILE
    // ==================================================

    AVFormatContext* formatContext =
        nullptr;


    if (
        avformat_open_input(
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


    // ==================================================
    // READ STREAM INFORMATION
    // ==================================================

    if (
        avformat_find_stream_info(
            formatContext,
            nullptr) < 0)
    {
        std::cout
            << "Cannot read stream information."
            << std::endl;


        avformat_close_input(
            &formatContext);


        return false;
    }


    // ==================================================
    // FIND VIDEO STREAM
    // ==================================================

    int videoStreamIndex =
        -1;


    for (
        unsigned int i = 0;
        i < formatContext->nb_streams;
        i++)
    {
        if (
            formatContext
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
            &formatContext);


        return false;
    }


    // ==================================================
    // VIDEO STREAM
    // ==================================================

    AVStream* videoStream =
        formatContext
        ->streams[videoStreamIndex];


    AVCodecParameters* codecParameters =
        videoStream
        ->codecpar;


    // ==================================================
    // FIND DECODER
    // ==================================================

    const AVCodec* codec =
        avcodec_find_decoder(
            codecParameters->codec_id);


    if (codec == nullptr)
    {
        std::cout
            << "Video decoder not found."
            << std::endl;


        avformat_close_input(
            &formatContext);


        return false;
    }


    // ==================================================
    // CODEC CONTEXT
    // ==================================================

    AVCodecContext* codecContext =
        avcodec_alloc_context3(
            codec);


    if (codecContext == nullptr)
    {
        std::cout
            << "Cannot create codec context."
            << std::endl;


        avformat_close_input(
            &formatContext);


        return false;
    }


    // ==================================================
    // COPY CODEC PARAMETERS
    // ==================================================

    if (
        avcodec_parameters_to_context(
            codecContext,
            codecParameters) < 0)
    {
        std::cout
            << "Cannot copy codec parameters."
            << std::endl;


        avcodec_free_context(
            &codecContext);


        avformat_close_input(
            &formatContext);


        return false;
    }


    // ==================================================
    // OPEN DECODER
    // ==================================================

    if (
        avcodec_open2(
            codecContext,
            codec,
            nullptr) < 0)
    {
        std::cout
            << "Cannot open decoder."
            << std::endl;


        avcodec_free_context(
            &codecContext);


        avformat_close_input(
            &formatContext);


        return false;
    }


    // ==================================================
    // ALLOCATE FRAMES
    // ==================================================

    AVFrame* frame =
        av_frame_alloc();


    AVFrame* rgbaFrame =
        av_frame_alloc();


    if (
        frame == nullptr ||
        rgbaFrame == nullptr)
    {
        std::cout
            << "Cannot allocate video frame."
            << std::endl;


        av_frame_free(
            &frame);


        av_frame_free(
            &rgbaFrame);


        avcodec_free_context(
            &codecContext);


        avformat_close_input(
            &formatContext);


        return false;
    }


    // ==================================================
    // RGBA BUFFER
    // ==================================================

    int rgbaBufferSize =
        av_image_get_buffer_size(
            AV_PIX_FMT_RGBA,
            codecContext->width,
            codecContext->height,
            1);


    unsigned char* rgbaBuffer =
        static_cast<unsigned char*>(
            av_malloc(
                rgbaBufferSize));


    if (rgbaBuffer == nullptr)
    {
        std::cout
            << "Cannot allocate RGBA buffer."
            << std::endl;


        av_frame_free(
            &frame);


        av_frame_free(
            &rgbaFrame);


        avcodec_free_context(
            &codecContext);


        avformat_close_input(
            &formatContext);


        return false;
    }


    // ==================================================
    // CONNECT BUFFER TO RGBA FRAME
    // ==================================================

    av_image_fill_arrays(
        rgbaFrame->data,
        rgbaFrame->linesize,
        rgbaBuffer,
        AV_PIX_FMT_RGBA,
        codecContext->width,
        codecContext->height,
        1);


    // ==================================================
    // SCALE CONTEXT
    // ==================================================

    SwsContext* swsContext =
        sws_getContext(
            codecContext->width,
            codecContext->height,
            codecContext->pix_fmt,

            codecContext->width,
            codecContext->height,
            AV_PIX_FMT_RGBA,

            SWS_BILINEAR,

            nullptr,
            nullptr,
            nullptr);


    if (swsContext == nullptr)
    {
        std::cout
            << "Cannot create SwsContext."
            << std::endl;


        av_free(
            rgbaBuffer);


        av_frame_free(
            &rgbaFrame);


        av_frame_free(
            &frame);


        avcodec_free_context(
            &codecContext);


        avformat_close_input(
            &formatContext);


        return false;
    }


    // ==================================================
    // SDL VIDEO TEXTURE
    // ==================================================

    SDL_Texture* videoTexture =
        SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_RGBA32,
            SDL_TEXTUREACCESS_STREAMING,
            codecContext->width,
            codecContext->height);


    if (videoTexture == nullptr)
    {
        std::cout
            << "Cannot create SDL video texture: "
            << SDL_GetError()
            << std::endl;


        sws_freeContext(
            swsContext);


        av_free(
            rgbaBuffer);


        av_frame_free(
            &rgbaFrame);


        av_frame_free(
            &frame);


        avcodec_free_context(
            &codecContext);


        avformat_close_input(
            &formatContext);


        return false;
    }


    // ==================================================
    // PACKET
    // ==================================================

    AVPacket* packet =
        av_packet_alloc();


    if (packet == nullptr)
    {
        std::cout
            << "Cannot allocate AVPacket."
            << std::endl;


        SDL_DestroyTexture(
            videoTexture);


        sws_freeContext(
            swsContext);


        av_free(
            rgbaBuffer);


        av_frame_free(
            &rgbaFrame);


        av_frame_free(
            &frame);


        avcodec_free_context(
            &codecContext);


        avformat_close_input(
            &formatContext);


        return false;
    }


    // ==================================================
    // GET FPS
    // ==================================================

    AVRational frameRate =
        av_guess_frame_rate(
            formatContext,
            videoStream,
            nullptr);


    double fps =
        30.0;


    if (
        frameRate.num > 0 &&
        frameRate.den > 0)
    {
        fps =
            av_q2d(
                frameRate);
    }


    if (fps <= 0.0)
    {
        fps =
            30.0;
    }


    const double frameDurationMilliseconds =
        1000.0 / fps;


    // ==================================================
    // INFORMATION
    // ==================================================

    std::cout
        << "=================================="
        << std::endl;


    std::cout
        << "Playing intro video"
        << std::endl;


    std::cout
        << "Video: "
        << videoPath
        << std::endl;


    std::cout
        << "Resolution: "
        << codecContext->width
        << " x "
        << codecContext->height
        << std::endl;


    std::cout
        << "FPS: "
        << fps
        << std::endl;


    std::cout
        << "=================================="
        << std::endl;


    // ==================================================
    // PLAYBACK
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
        // VIDEO PACKETS ONLY
        // ==============================================

        if (
            packet->stream_index ==
            videoStreamIndex)
        {
            int sendResult =
                avcodec_send_packet(
                    codecContext,
                    packet);


            if (sendResult >= 0)
            {
                while (playing)
                {
                    int receiveResult =
                        avcodec_receive_frame(
                            codecContext,
                            frame);


                    if (
                        receiveResult ==
                        AVERROR(EAGAIN) ||
                        receiveResult ==
                        AVERROR_EOF)
                    {
                        break;
                    }


                    if (receiveResult < 0)
                    {
                        playing =
                            false;

                        break;
                    }


                    // ==================================
                    // FRAME START TIME
                    // ==================================

                    auto frameStart =
                        std::chrono::
                        steady_clock::now();


                    // ==================================
                    // EVENTS DURING VIDEO
                    // ==================================

                    SDL_Event event;


                    while (
                        SDL_PollEvent(
                            &event))
                    {
                        // ==================================
                        // CLOSE GAME
                        // ==================================

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


                        // ==================================
                        // SKIP INTRO
                        // ==================================

                        if (
                            event.type ==
                            SDL_KEYDOWN)
                        {
                            if (
                                event.key.keysym.sym ==
                                SDLK_ESCAPE ||
                                event.key.keysym.sym ==
                                SDLK_RETURN ||
                                event.key.keysym.sym ==
                                SDLK_SPACE)
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


                    // ==================================
                    // CONVERT FRAME TO RGBA
                    // ==================================

                    sws_scale(
                        swsContext,
                        frame->data,
                        frame->linesize,
                        0,
                        codecContext->height,
                        rgbaFrame->data,
                        rgbaFrame->linesize);


                    // ==================================
                    // UPDATE SDL TEXTURE
                    // ==================================

                    if (
                        SDL_UpdateTexture(
                            videoTexture,
                            nullptr,
                            rgbaFrame->data[0],
                            rgbaFrame->linesize[0])
                        != 0)
                    {
                        std::cout
                            << "SDL_UpdateTexture Error: "
                            << SDL_GetError()
                            << std::endl;


                        playing =
                            false;


                        break;
                    }


                    // ==================================
                    // RENDERER SIZE
                    // ==================================

                    int rendererWidth =
                        0;


                    int rendererHeight =
                        0;


                    SDL_GetRendererOutputSize(
                        renderer,
                        &rendererWidth,
                        &rendererHeight);


                    // ==================================
                    // VIDEO ASPECT RATIO
                    // ==================================

                    double videoRatio =
                        static_cast<double>(
                            codecContext->width)
                        /
                        static_cast<double>(
                            codecContext->height);


                    double screenRatio =
                        static_cast<double>(
                            rendererWidth)
                        /
                        static_cast<double>(
                            rendererHeight);


                    SDL_Rect destination =
                    {
                        0,
                        0,
                        rendererWidth,
                        rendererHeight
                    };


                    // ==================================
                    // LETTERBOX
                    // ==================================

                    if (
                        screenRatio >
                        videoRatio)
                    {
                        destination.h =
                            rendererHeight;


                        destination.w =
                            static_cast<int>(
                                rendererHeight *
                                videoRatio);


                        destination.x =
                            (
                                rendererWidth -
                                destination.w
                                )
                            / 2;


                        destination.y =
                            0;
                    }
                    else
                    {
                        destination.w =
                            rendererWidth;


                        destination.h =
                            static_cast<int>(
                                rendererWidth /
                                videoRatio);


                        destination.x =
                            0;


                        destination.y =
                            (
                                rendererHeight -
                                destination.h
                                )
                            / 2;
                    }


                    // ==================================
                    // BLACK BACKGROUND
                    // ==================================

                    SDL_SetRenderDrawColor(
                        renderer,
                        0,
                        0,
                        0,
                        255);


                    SDL_RenderClear(
                        renderer);


                    // ==================================
                    // DRAW VIDEO
                    // ==================================

                    SDL_RenderCopy(
                        renderer,
                        videoTexture,
                        nullptr,
                        &destination);


                    SDL_RenderPresent(
                        renderer);


                    // ==================================
                    // CONTROL PLAYBACK SPEED
                    // ==================================

                    auto frameEnd =
                        std::chrono::
                        steady_clock::now();


                    double elapsedMilliseconds =
                        std::chrono::duration<
                        double,
                        std::milli>(
                            frameEnd -
                            frameStart)
                        .count();


                    double waitMilliseconds =
                        frameDurationMilliseconds -
                        elapsedMilliseconds;


                    if (waitMilliseconds > 0.0)
                    {
                        std::this_thread::
                            sleep_for(
                                std::chrono::
                                milliseconds(
                                    static_cast<int>(
                                        waitMilliseconds)));
                    }
                }
            }
        }


        av_packet_unref(
            packet);
    }


    // ==================================================
    // CLEANUP
    // ==================================================

    av_packet_free(
        &packet);


    SDL_DestroyTexture(
        videoTexture);


    sws_freeContext(
        swsContext);


    av_free(
        rgbaBuffer);


    av_frame_free(
        &rgbaFrame);


    av_frame_free(
        &frame);


    avcodec_free_context(
        &codecContext);


    avformat_close_input(
        &formatContext);


    std::cout
        << "Intro video finished."
        << std::endl;


    return true;
}