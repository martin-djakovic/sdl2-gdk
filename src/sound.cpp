#include <sound.hpp>

Sound::Sound() {
    this->file_path = "";
    SetChannel(-1);
}

Sound::Sound(const char *audio_file_path, int volume) {
    SetSound(audio_file_path);
    SetChannel(-1);
    SetVolume(volume);
}

void Sound::LoadChunk() {
    Mix_FreeChunk(chunk);
    chunk = Mix_LoadWAV(file_path);

    if (chunk == NULL) {
        printf("Failed loading audio file: %s\n", file_path);
    }
}

void Sound::SetSound(const char *audio_file_path) {
    this->file_path = audio_file_path;
    LoadChunk();
}

void Sound::SetVolume(int volume) {
    this->volume = volume;
    Mix_Volume(channel, volume);
}
int Sound::GetVolume() { return volume; }

void Sound::SetChannel(int channel) { this->channel = channel; }
int Sound::GetChannel() { return channel; }

void Sound::Destroy() { Mix_FreeChunk(chunk); }

void Sound::Play(int loops) { Mix_PlayChannel(channel, chunk, loops); }
// Play audio for time_ms milliseconds
void Sound::PlayTimed(int time_ms, int loops) {
    Mix_PlayChannelTimed(channel, chunk, loops, time_ms);
}

void Sound::Pause() { Mix_Pause(channel); }
void Sound::Resume() { Mix_Resume(channel); }