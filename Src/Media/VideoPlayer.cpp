#include "Media/VideoPlayer.h"

#include <iostream>


extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
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
    const std::string& videoPath)
{
    std::cout
        << "=================================="
        << std::endl;


    std::cout
        << "FFmpeg test"
        << std::endl;


    std::cout
        << "Video: "
        << videoPath
        << std::endl;


    std::cout
        << "FFmpeg version: "
        << av_version_info()
        << std::endl;


    std::cout
        << "=================================="
        << std::endl;


    return true;
}