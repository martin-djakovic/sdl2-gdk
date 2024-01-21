#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_cpuinfo.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <iostream>

/* Returns unparsed system info
   [0] - video driver,
   [1] - audio driver,
   [2] - CPU thread count,
   [3] - system RAM capacity in MB
*/
const std::vector<std::string> GDK_GetSystemInfoRaw() {
    std::string video_driver = SDL_GetCurrentVideoDriver();
    std::string audio_driver = SDL_GetCurrentAudioDriver();
    std::string cpu_thread_count = std::to_string(SDL_GetCPUCount()).c_str();
    std::string ram_total = std::to_string(SDL_GetSystemRAM()).c_str();

    return {video_driver, audio_driver, cpu_thread_count, ram_total};
}

// Returns parsed system info
const std::vector<std::string> GDK_GetSystemInfo() {
    const std::vector<std::string> system_info = GDK_GetSystemInfoRaw();

    return {
        "video_driver: " + system_info[0], "audio_driver: " + system_info[1],
        "cpu_thread_count: " + system_info[2], "ram_total: " + system_info[3] + " MB"};
}

#endif
