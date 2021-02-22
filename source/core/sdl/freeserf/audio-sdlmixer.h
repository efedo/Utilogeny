/*
 * audio-sdlmixer.h - Music and sound effects playback using SDL_mixer.
 *
 * Copyright (C) 2015-2017  Wicked_Digger <wicked_digger@mail.ru>
 *
 * This file is part of freeserf.
 *
 * freeserf is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * freeserf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with freeserf.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <string>
#include <SDL_mixer.h>
#include "Utilogeny/source/core/sdl/freeserf/audio.h"
#include "Utilogeny/source/core/sdl/freeserf/data.h"
#include "Utilogeny/source/core/sdl/freeserf/event_loop.h"

using TypeMidi = int; // was originally enum

class AudioSDL : public Audio, public Audio::VolumeController {
public:
    AudioSDL();
    virtual ~AudioSDL();
    virtual Audio::VolumeController* get_volume_controller() { return this; }
    virtual Audio::PPlayer get_sound_player() { return sfx_player; }
    virtual Audio::PPlayer get_music_player() { return midi_player; }
    virtual float get_volume();
    virtual void set_volume(float volume);
    virtual void volume_up();
    virtual void volume_down();
protected:
    class TrackSFX;
    class PlayerSFX;
    class TrackMIDI;
    class PlayerMIDI;
    Audio::PPlayer sfx_player;
    Audio::PPlayer midi_player;
};

class AudioSDL::TrackSFX : public Audio::Track {
public:
    explicit TrackSFX(Mix_Chunk* chunk);
    virtual ~TrackSFX();
    virtual void play();
protected:
    Mix_Chunk* chunk;
};

class AudioSDL::PlayerSFX : public Audio::Player, public Audio::VolumeController, public std::enable_shared_from_this<PlayerSFX> {
public:
    virtual void enable(bool enable);
    virtual Audio::PVolumeController get_volume_controller() { return shared_from_this(); }
    virtual float get_volume();
    virtual void set_volume(float volume);
    virtual void volume_up();
    virtual void volume_down();
protected:
    virtual Audio::PTrack create_track(int track_id);
    virtual void stop();
};

class AudioSDL::TrackMIDI : public Audio::Track {
public:
    explicit TrackMIDI(PBuffer data, Mix_Music* chunk);
    virtual ~TrackMIDI();
    virtual void play(); 
protected:
    PBuffer data;
    Mix_Music* chunk;
};

class AudioSDL::PlayerMIDI : public Audio::Player, public Audio::VolumeController, public std::enable_shared_from_this<PlayerMIDI> {
public:
    PlayerMIDI();
    virtual ~PlayerMIDI();
    virtual Audio::PTrack play_track(int track_id);
    virtual void enable(bool enable);
    virtual Audio::PVolumeController get_volume_controller() { return shared_from_this(); }
    virtual float get_volume();
    virtual void set_volume(float volume);
    virtual void volume_up();
    virtual void volume_down();
protected:
    TypeMidi current_track;
    virtual Audio::PTrack create_track(int track_id);
    virtual void stop();
    static PlayerMIDI* current_midi_player;
    static void music_finished_hook();
    void music_finished();
    friend class TrackMIDI;
};