#include "Utilogeny/source/core/lib/precomp.h"
#include "Utilogeny/source/core/exceptions.h"
#include "Utilogeny/source/core/sdl/freeserf/audio-sdlmixer.h"
#include "Utilogeny/source/core/sdl/freeserf/data.h"

#include <algorithm>
#include <memory>
#include <string>

#include <SDL.h>
#include <SDL_mixer.h>

class AudioSDL {
public:
    AudioSDL();
    ~AudioSDL();
};

class Buffer {
public:
	void* get_data() { return 0; };
	int get_size() { return 0; };
};
typedef std::shared_ptr<Buffer> PBuffer;


class AudioObject {
public:
};

class AudioObjectSDL : public AudioObject {
public:
    void set_volume(float);
    float get_volume();
protected:
    SDL_RWops* rw = 0;
};

class AudioObjectSDLWav : public AudioObjectSDL {
public:
    ~AudioObjectSDLWav();
    void load(std::string name);
    void play();
    void stop();
private:
    Mix_Chunk* chunk = 0;
};

class AudioObjectSDLMidi : public AudioObjectSDL {
public:
    ~AudioObjectSDLMidi();
    void load(std::string name);
    void play();
private:
    Mix_Music* music = 0;
};




AudioSDL::AudioSDL() {
    std::cout << "Initializing \"sdlmixer\".";

    std::cout << "Available drivers:";
    int num_drivers = SDL_GetNumAudioDrivers();
    for (int i = 0; i < num_drivers; ++i) {
        std::cout << "\t" << SDL_GetAudioDriver(i);
    }

    if (SDL_AudioInit(NULL) != 0) {
        throwl("Could not init SDL audio");
    }

    SDL_version version;
    SDL_GetVersion(&version);
    std::cout << "Initialized with SDL "
        << static_cast<int>(version.major) << '.'
        << static_cast<int>(version.minor) << '.'
        << static_cast<int>(version.patch)
        << " (driver: " << SDL_GetCurrentAudioDriver() << ")";

    const SDL_version* mversion = Mix_Linked_Version();
    std::cout << "Initializing SDL_mixer "
        << static_cast<int>(mversion->major) << '.'
        << static_cast<int>(mversion->minor) << '.'
        << static_cast<int>(mversion->patch);

    int r = Mix_Init(0);
    if (r != 0) {
        throwl("Could not init SDL_mixer");
    }

    r = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 512);
    if (r < 0) {
        throwl("Could not open audio device");
    }

    r = Mix_AllocateChannels(128);
    if (r != 128) {
        throwl("Failed to allocate channels");
    }

    volume = 1.f;

    sfx_player = std::make_shared<AudioSDL::PlayerSFX>();
    midi_player = std::make_shared<AudioSDL::PlayerMIDI>();

    std::cout << "Initialized";
}

AudioSDL::~AudioSDL() {
    sfx_player = nullptr;
    midi_player = nullptr;

    Mix_CloseAudio();
    Mix_Quit();
    SDL_AudioQuit();
}



float AudioObjectSDL::get_volume() {
    return static_cast<float>(Mix_VolumeMusic(-1)) /
        static_cast<float>(MIX_MAX_VOLUME);
}

void AudioObjectSDL::set_volume(float volume) {
    volume = std::max(0.f, std::min(volume, 1.f));
    float mix_volume = static_cast<float>(MIX_MAX_VOLUME) * volume;
    Mix_VolumeMusic(static_cast<int>(mix_volume));
}

void AudioObjectSDLWav::load(std::string name) {
    PBuffer wav;// = data_source->get_music(track_id);
    if (!midi) { return nullptr; }
    rw = SDL_RWFromMem(midi->get_data(), static_cast<int>(midi->get_size()));
    chunk = Mix_LoadWAV_RW(rw, 0);
    if (chunk == nullptr) {
        std::cout << "Mix_LoadWAV_RW: " << Mix_GetError();
    }
}

void AudioObjectSDLWav::play() {
    if (chunk == nullptr) return;
    int r = Mix_PlayChannel(-1, chunk, 0);
    if (r < 0) std::cout << "Could not play SFX clip: " << Mix_GetError();
}

void AudioObjectSDLWav::stop() {
    Mix_HaltChannel(-1);
}

AudioObjectSDLWav::~AudioObjectSDLWav() {
    if (chunk) Mix_FreeChunk(chunk);
}


void AudioObjectSDLMidi::load(std::string name) {
    PBuffer midi;// = data_source->get_music(track_id);
    if (!midi) { return nullptr; }
    rw = SDL_RWFromMem(midi->get_data(), static_cast<int>(midi->get_size()));
    music = Mix_LoadMUS_RW(rw, 0);

}

void AudioObjectSDLMidi::play() {
    if (music == nullptr) return;
    int r = Mix_PlayMusic(music, 0);
    if (r < 0) std::cerr << "Could not play MIDI track: " << Mix_GetError();
}

void AudioObjectSDLMidi::stop() {
    Mix_HaltMusic();
}

AudioObjectSDLMidi::~AudioObjectSDLMidi() {
    if(music) Mix_FreeMusic(music);
}
