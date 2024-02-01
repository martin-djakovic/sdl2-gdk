#ifndef SOUND_HPP
#define SOUND_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

class Sound {
  private:
    Mix_Chunk *chunk = nullptr;
    const char *file_path;
    int channel;
    int volume;

    void LoadChunk() {
        Mix_FreeChunk(chunk);
        chunk = Mix_LoadWAV(file_path);

        if (chunk == NULL) {
            printf("Failed loading audio file: %s\n", file_path);
        }
    }

  public:
    Sound() {
        this->file_path = "";
        SetChannel(-1);
    }

    Sound(const char *audio_file_path, int volume = 100) {
        SetSound(audio_file_path);
        SetChannel(-1);
        SetVolume(volume);
    }

    void SetSound(const char *audio_file_path) {
        this->file_path = audio_file_path;
        LoadChunk();
    }

    void SetVolume(int volume) {
        this->volume = volume;
        Mix_Volume(channel, volume);
    }
    int GetVolume() { return volume; }

    void SetChannel(int channel) { this->channel = channel; }
    int GetChannel() { return channel; }

    void Destroy() { Mix_FreeChunk(chunk); }

    void Play(int loops = 1) { Mix_PlayChannel(channel, chunk, loops); }
    // Play audio for time_ms milliseconds
    void PlayTimed(int time_ms, int loops = 1) {
        Mix_PlayChannelTimed(channel, chunk, loops, time_ms);
    }

    void Pause() { Mix_Pause(channel); }
    void Resume() { Mix_Resume(channel); }
};

#endif