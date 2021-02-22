/*
 * audio.h - Music and sound effects playback base.
 *
 * Copyright (C) 2012-2018  Wicked_Digger <wicked_digger@mail.ru>
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

#include <map>
#include <memory>
#include <string>

using TypeMidi = int; // used to be enum with track list

class Audio {
public:
    class VolumeController;
    class Track;
    class Player;
    typedef std::shared_ptr<VolumeController> PVolumeController;
    typedef std::shared_ptr<Track> PTrack;
    typedef std::shared_ptr<Player> PPlayer;
    virtual ~Audio() {}
    static Audio& get_instance();
    virtual VolumeController* get_volume_controller() = 0;
    virtual PPlayer get_sound_player() = 0;
    virtual PPlayer get_music_player() = 0;
protected:
    static Audio *instance;
    float volume;
    Audio() : volume(0.75f) {}
};

class Audio::VolumeController {
public:
    virtual ~VolumeController() {}
    virtual float get_volume() = 0;
    virtual void set_volume(float volume) = 0;
    virtual void volume_up() = 0;
    virtual void volume_down() = 0;
};

class Audio::Track {
public:
    virtual ~Track() {}
    virtual void play() = 0;
};

class Audio::Player {
public:
    Player();
    virtual ~Player();
    virtual PTrack play_track(int track_id);
    virtual void enable(bool enable) = 0;
    virtual bool is_enabled() const { return enabled; }
    virtual PVolumeController get_volume_controller() = 0;
protected:
    typedef std::map<int, PTrack> TrackCache;
    TrackCache track_cache;
    bool enabled;
    virtual PTrack create_track(int track_id) = 0;
    virtual void stop() = 0;
};