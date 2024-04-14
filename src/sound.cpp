#include <errorcolors.hpp>
#include <sound.hpp>

GDK_Sound::GDK_Sound() {
  this->file_path = "";
  setChannel(-1);
}

GDK_Sound::GDK_Sound(const char *audio_file_path, unsigned int volume) {
  setSound(audio_file_path);
  setChannel(-1);
  setVolume(volume);
}

void GDK_Sound::loadChunk() {
  Mix_FreeChunk(chunk);
  chunk = Mix_LoadWAV(file_path);

  if (chunk == NULL) {
    printf(ERR_COLOR "GDK ERROR:" DEF_COLOR
                     "Failed loading audio file:" FPATH_COLOR " %s\n",
           file_path);
  }
}

void GDK_Sound::setSound(const char *audio_file_path) {
  this->file_path = audio_file_path;
  loadChunk();
}

void GDK_Sound::setVolume(unsigned int volume) noexcept {
  this->volume = volume;
  Mix_Volume(channel, volume);
}
const int GDK_Sound::getVolume() noexcept { return volume; }

void GDK_Sound::setChannel(int channel) noexcept {
  if (channel < -1) {
    printf(ERR_COLOR "GDK ERROR:" DEF_COLOR " Invalid channel value %i\n",
           channel);
    return;
  }

  this->channel = channel;
}
const int GDK_Sound::getChannel() noexcept { return channel; }

void GDK_Sound::destroy() { Mix_FreeChunk(chunk); }

void GDK_Sound::play(unsigned int loops) {
  Mix_PlayChannel(channel, chunk, loops);
}
void GDK_Sound::playTimed(unsigned int time_ms, unsigned int loops) {
  Mix_PlayChannelTimed(channel, chunk, loops, time_ms);
}

void GDK_Sound::pause() { Mix_Pause(channel); }
void GDK_Sound::resume() { Mix_Resume(channel); }
