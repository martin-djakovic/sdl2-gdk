#ifndef SOUND_HPP
#define SOUND_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

class GDK_Sound {
private:
  Mix_Chunk *chunk = nullptr;
  const char *file_path;
  int channel;
  int volume;

  void loadChunk();

public:
  GDK_Sound();

  GDK_Sound(const char *audio_file_path, unsigned int volume = 100);

  /*
   * @brief Set sound file to be played
   * @param audio_file_path Relative or absolute file path to the audio file.
   * Supports all formats supported by SDL_mixer (.mp3, .wav, etc.)
   */
  void setSound(const char *audio_file_path);

  /*
   * @brief Set volume in % (100 is maximum, 0 is minimum volume)
   */
  void setVolume(unsigned int volume) noexcept;

  /*
   * @brief Get volume in % (100 is maximum, 0 is minimum volume)
   */
  const int getVolume() noexcept;

  /*
   * @brief Set channel on which audio is played. Only one sound at a time can
   * be played per channel. Value of -1 means sound will play on the first
   * available channel
   */
  void setChannel(int channel) noexcept;

  /*
   * @brief Get channel on which audio is played. Only one sound at a time can
   * be played per channel
   */
  const int getChannel() noexcept;

  /*
   * @brief Frees loaded audio file from memory
   */
  void destroy();

  /*
   * @brief Play loaded sound
   * @param loops Number of times to play the sound
   */
  void play(unsigned int loops = 1);

  /*
   * @brief Play loaded sound for given time
   * @param time_ms Length of time for which sound will be played in
   * milliseconds
   * @param loops Number of times to play the sound (loops will not finish if
   * time has been exceeded)
   */
  void playTimed(unsigned int time_ms, unsigned int loops = 1);

  /*
   * Pause playing this sound
   */
  void pause();

  /*
   * Resume playing this sound
   */
  void resume();
};

#endif
