#include "Media/MusicPlayer.h"

#include <algorithm>
#include <iostream>


extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>

#include <libavutil/channel_layout.h>
#include <libavutil/samplefmt.h>

#include <libswresample/swresample.h>
}


// ==================================================
// CONSTRUCTOR
// ==================================================

MusicPlayer::MusicPlayer()
{
    audioDevice =
        0;


    audioPosition =
        0;


    loaded =
        false;
}


// ==================================================
// DESTRUCTOR
// ==================================================

MusicPlayer::~MusicPlayer()
{
    Stop();
}


// ==================================================
// LOAD
// ==================================================

bool MusicPlayer::Load(
    const std::string& filePath)
{
    Stop();


    // ==========================================
    // OUTPUT AUDIO FORMAT
    // ==========================================

    const int outputSampleRate =
        48000;


    const int outputChannels =
        2;


    // ==========================================
    // OPEN MEDIA FILE
    // ==========================================

    AVFormatContext* formatContext =
        nullptr;


    if (
        avformat_open_input(
            &formatContext,
            filePath.c_str(),
            nullptr,
            nullptr) < 0)
    {
        std::cout
            << "Cannot open background music: "
            << filePath
            << std::endl;


        return false;
    }


    if (
        avformat_find_stream_info(
            formatContext,
            nullptr) < 0)
    {
        std::cout
            << "Cannot read background music stream information."
            << std::endl;


        avformat_close_input(
            &formatContext);


        return false;
    }


    // ==========================================
    // FIND AUDIO STREAM
    // ==========================================

    int audioStreamIndex =
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
            AVMEDIA_TYPE_AUDIO)
        {
            audioStreamIndex =
                static_cast<int>(
                    i);


            break;
        }
    }


    if (audioStreamIndex == -1)
    {
        std::cout
            << "Background music has no audio stream."
            << std::endl;


        avformat_close_input(
            &formatContext);


        return false;
    }


    // ==========================================
    // AUDIO DECODER
    // ==========================================

    AVCodecParameters* parameters =
        formatContext
        ->streams[audioStreamIndex]
        ->codecpar;


    const AVCodec* codec =
        avcodec_find_decoder(
            parameters->codec_id);


    if (codec == nullptr)
    {
        std::cout
            << "Background music decoder not found."
            << std::endl;


        avformat_close_input(
            &formatContext);


        return false;
    }


    AVCodecContext* codecContext =
        avcodec_alloc_context3(
            codec);


    if (codecContext == nullptr)
    {
        avformat_close_input(
            &formatContext);


        return false;
    }


    if (
        avcodec_parameters_to_context(
            codecContext,
            parameters) < 0 ||
        avcodec_open2(
            codecContext,
            codec,
            nullptr) < 0)
    {
        std::cout
            << "Cannot open background music decoder."
            << std::endl;


        avcodec_free_context(
            &codecContext);


        avformat_close_input(
            &formatContext);


        return false;
    }


    // ==========================================
    // RESAMPLER
    // ==========================================

    SwrContext* swrContext =
        nullptr;


    AVChannelLayout outputLayout =
        AV_CHANNEL_LAYOUT_STEREO;


    int swrResult =
        swr_alloc_set_opts2(
            &swrContext,

            &outputLayout,

            AV_SAMPLE_FMT_S16,

            outputSampleRate,

            &codecContext->ch_layout,

            codecContext->sample_fmt,

            codecContext->sample_rate,

            0,

            nullptr);


    av_channel_layout_uninit(
        &outputLayout);


    if (
        swrResult < 0 ||
        swrContext == nullptr ||
        swr_init(
            swrContext) < 0)
    {
        std::cout
            << "Cannot initialize background music resampler."
            << std::endl;


        if (swrContext != nullptr)
        {
            swr_free(
                &swrContext);
        }


        avcodec_free_context(
            &codecContext);


        avformat_close_input(
            &formatContext);


        return false;
    }


    // ==========================================
    // DECODE AUDIO
    // ==========================================

    AVPacket* packet =
        av_packet_alloc();


    AVFrame* frame =
        av_frame_alloc();


    if (
        packet == nullptr ||
        frame == nullptr)
    {
        av_packet_free(
            &packet);


        av_frame_free(
            &frame);


        swr_free(
            &swrContext);


        avcodec_free_context(
            &codecContext);


        avformat_close_input(
            &formatContext);


        return false;
    }


    audioData.clear();


    auto appendFrame =
        [&](AVFrame* currentFrame)
        {
            int outputSamples =
                static_cast<int>(
                    av_rescale_rnd(

                        swr_get_delay(
                            swrContext,
                            codecContext->sample_rate)
                        +
                        currentFrame->nb_samples,

                        outputSampleRate,

                        codecContext->sample_rate,

                        AV_ROUND_UP));


            int bufferSize =
                av_samples_get_buffer_size(
                    nullptr,

                    outputChannels,

                    outputSamples,

                    AV_SAMPLE_FMT_S16,

                    1);


            if (bufferSize <= 0)
            {
                return;
            }


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
                    currentFrame
                    ->extended_data);


            int convertedSamples =
                swr_convert(
                    swrContext,

                    outputData,

                    outputSamples,

                    inputData,

                    currentFrame
                    ->nb_samples);


            if (convertedSamples <= 0)
            {
                return;
            }


            int convertedSize =
                av_samples_get_buffer_size(
                    nullptr,

                    outputChannels,

                    convertedSamples,

                    AV_SAMPLE_FMT_S16,

                    1);


            if (convertedSize <= 0)
            {
                return;
            }


            std::size_t oldSize =
                audioData.size();


            audioData.resize(
                oldSize +
                static_cast<std::size_t>(
                    convertedSize));


            std::copy(
                buffer.begin(),

                buffer.begin() +
                convertedSize,

                audioData.begin() +
                oldSize);
        };


    while (
        av_read_frame(
            formatContext,
            packet) >= 0)
    {
        if (
            packet->stream_index ==
            audioStreamIndex)
        {
            if (
                avcodec_send_packet(
                    codecContext,
                    packet) >= 0)
            {
                while (
                    avcodec_receive_frame(
                        codecContext,
                        frame) >= 0)
                {
                    appendFrame(
                        frame);
                }
            }
        }


        av_packet_unref(
            packet);
    }


    // ==========================================
    // FLUSH DECODER
    // ==========================================

    avcodec_send_packet(
        codecContext,
        nullptr);


    while (
        avcodec_receive_frame(
            codecContext,
            frame) >= 0)
    {
        appendFrame(
            frame);
    }


    // ==========================================
    // CLEAN FFMPEG
    // ==========================================

    av_packet_free(
        &packet);


    av_frame_free(
        &frame);


    swr_free(
        &swrContext);


    avcodec_free_context(
        &codecContext);


    avformat_close_input(
        &formatContext);


    if (audioData.empty())
    {
        std::cout
            << "Background music decoded no audio."
            << std::endl;


        return false;
    }


    // ==========================================
    // OPEN SDL AUDIO DEVICE
    // ==========================================

    SDL_AudioSpec wantedSpec;
    SDL_AudioSpec obtainedSpec;


    SDL_zero(
        wantedSpec);


    SDL_zero(
        obtainedSpec);


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
        MusicPlayer::AudioCallback;


    wantedSpec.userdata =
        this;


    audioDevice =
        SDL_OpenAudioDevice(
            nullptr,

            0,

            &wantedSpec,

            &obtainedSpec,

            0);


    if (audioDevice == 0)
    {
        std::cout
            << "Cannot open background music audio device: "
            << SDL_GetError()
            << std::endl;


        audioData.clear();


        return false;
    }


    audioPosition =
        0;


    loaded =
        true;


    // Device stays paused until Play().
    SDL_PauseAudioDevice(
        audioDevice,
        1);


    std::cout
        << "Background music loaded: "
        << filePath
        << std::endl;


    return true;
}


// ==================================================
// PLAY
// ==================================================

void MusicPlayer::Play()
{
    if (
        !loaded ||
        audioDevice == 0)
    {
        return;
    }


    SDL_PauseAudioDevice(
        audioDevice,
        1);


    audioPosition =
        0;


    SDL_PauseAudioDevice(
        audioDevice,
        0);
}


// ==================================================
// PAUSE
// ==================================================

void MusicPlayer::Pause()
{
    if (
        !loaded ||
        audioDevice == 0)
    {
        return;
    }


    SDL_PauseAudioDevice(
        audioDevice,
        1);
}


// ==================================================
// RESUME
// ==================================================

void MusicPlayer::Resume()
{
    if (
        !loaded ||
        audioDevice == 0)
    {
        return;
    }


    SDL_PauseAudioDevice(
        audioDevice,
        0);
}


// ==================================================
// STOP
// ==================================================

void MusicPlayer::Stop()
{
    if (audioDevice != 0)
    {
        SDL_PauseAudioDevice(
            audioDevice,
            1);


        SDL_CloseAudioDevice(
            audioDevice);


        audioDevice =
            0;
    }


    audioData.clear();


    audioPosition =
        0;


    loaded =
        false;
}


// ==================================================
// IS LOADED
// ==================================================

bool MusicPlayer::IsLoaded() const
{
    return loaded;
}


// ==================================================
// SDL AUDIO CALLBACK
// ==================================================

void MusicPlayer::AudioCallback(
    void* userdata,
    Uint8* stream,
    int length)
{
    MusicPlayer* musicPlayer =
        static_cast<MusicPlayer*>(
            userdata);


    if (musicPlayer == nullptr)
    {
        SDL_memset(
            stream,
            0,
            length);


        return;
    }


    musicPlayer->FillAudio(
        stream,
        length);
}


// ==================================================
// FILL AUDIO
//
// When the end is reached, audioPosition returns to 0.
// Therefore the music loops forever.
// ==================================================

void MusicPlayer::FillAudio(
    Uint8* stream,
    int length)
{
    SDL_memset(
        stream,
        0,
        length);


    if (
        !loaded ||
        audioData.empty())
    {
        return;
    }


    int remaining =
        length;


    Uint8* destination =
        stream;


    while (remaining > 0)
    {
        std::size_t bytesLeft =
            audioData.size() -
            audioPosition;


        std::size_t copySize =
            std::min(
                static_cast<std::size_t>(
                    remaining),

                bytesLeft);


        SDL_memcpy(
            destination,

            audioData.data() +
            audioPosition,

            copySize);


        destination +=
            copySize;


        remaining -=
            static_cast<int>(
                copySize);


        audioPosition +=
            copySize;


        if (
            audioPosition >=
            audioData.size())
        {
            audioPosition =
                0;
        }
    }
}
