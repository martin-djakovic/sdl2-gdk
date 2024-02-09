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

    void LoadChunk();

  public:
    Sound();

    Sound(const char *audio_file_path, int volume = 100);

    void SetSound(const char *audio_file_path);

    void SetVolume(int volume);
    int GetVolume();

    void SetChannel(int channel);
    int GetChannel();

    void Destroy();

    void Play(int loops = 1);
    // Play audio for time_ms milliseconds
    void PlayTimed(int time_ms, int loops = 1);

    void Pause();
    void Resume();
};

#endif