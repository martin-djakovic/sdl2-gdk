#include <errorcolors.hpp>
#include <sound.hpp>

namespace gdk {
Sound::Sound() {
  this->file_path = "";
  setChannel(-1);
}

Sound::Sound(const char *audio_file_path, unsigned int volume) {
  setSound(audio_file_path);
  setChannel(-1);
  setVolume(volume);
}

void Sound::loadChunk() {
  Mix_FreeChunk(chunk);
  chunk = Mix_LoadWAV(file_path);

  if (chunk == NULL) {
    printf(ERR_COLOR "GDK ERROR:" DEF_COLOR
                     "Failed loading audio file:" FPATH_COLOR " %s\n",
           file_path);
  }
}

void Sound::setSound(const char *audio_file_path) {
  this->file_path = audio_file_path;
  loadChunk();
}

void Sound::setVolume(unsigned int volume) noexcept {
  this->volume = volume;
  Mix_Volume(channel, volume);
}
const int Sound::getVolume() noexcept { return volume; }

void Sound::setChannel(int channel) noexcept {
  if (channel < -1) {
    printf(ERR_COLOR "GDK ERROR:" DEF_COLOR " Invalid channel value %i\n",
           channel);
    return;
  }

  this->channel = channel;
}
const int Sound::getChannel() noexcept { return channel; }

void Sound::destroy() { Mix_FreeChunk(chunk); }

void Sound::play(unsigned int loops) {
  Mix_PlayChannel(channel, chunk, loops);
}
void Sound::playTimed(unsigned int time_ms, unsigned int loops) {
  Mix_PlayChannelTimed(channel, chunk, loops, time_ms);
}

void Sound::pause() { Mix_Pause(channel); }
void Sound::resume() { Mix_Resume(channel); }
} // namespace gdk
