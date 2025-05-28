#ifndef SOUND_HPP
#define SOUND_HPP

#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#elif _WIN32
#include <SDL.h>
#include <SDL_mixer.h>

#elif __APPLE__
#include <SDL.h>
#include <SDL_mixer.h>

#else
#error "FATAL ERROR: Operating system not supported"

#endif

namespace gdk {
class Sound {
private:
  Mix_Chunk *chunk = nullptr;
  const char *file_path;
  int channel;
  int volume;

  void loadChunk();

public:
  Sound();

  Sound(const char *audio_file_path, unsigned int volume = 100);

  Sound(Sound *sound);

  /**
   * @brief Set sound file to be played
   * @param audio_file_path Relative or absolute file path to the audio file.
   * Supports all formats supported by SDL_mixer (.mp3, .wav, etc.)
   */
  void setSound(const char *audio_file_path);

  /**
   * @brief Set volume in % (100 is maximum, 0 is minimum volume)
   */
  void setVolume(unsigned int volume) noexcept;

  /**
   * @brief Get volume in % (100 is maximum, 0 is minimum volume)
   */
  const int getVolume() noexcept;

  /**
   * @brief Set channel on which audio is played. Only one sound at a time can
   * be played per channel. Value of -1 means sound will play on the first
   * available channel
   */
  void setChannel(int channel) noexcept;

  /**
   * @brief Get channel on which audio is played. Only one sound at a time can
   * be played per channel
   */
  const int getChannel() noexcept;

  /**
   * @brief Frees loaded audio file from memory
   */
  void destroy();

  /**
   * @brief Play loaded sound
   * @param loops Number of times to play the sound
   */
  void play(unsigned int loops = 1);

  /**
   * @brief Play loaded sound for given time
   * @param time_ms Length of time for which sound will be played in
   * milliseconds
   * @param loops Number of times to play the sound (loops will not finish if
   * time has been exceeded)
   */
  void playTimed(unsigned int time_ms, unsigned int loops = 1);

  /**
   * @brief Pause playing this sound
   */
  void pause();

  /**
   * @brief Resume playing this sound
   */
  void resume();
};
} // namespace gdk

#endif
