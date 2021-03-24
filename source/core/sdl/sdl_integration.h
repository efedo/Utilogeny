// Copyright 2017-20 Eric Fedosejevs
//

#pragma once
#include "Utilogeny/source/core/precomp.h"
#include "Utilogeny/source/core/sdl/freeserf/audio.h"
#include "Utilogeny/source/core/sdl/freeserf/gfx.h"
#include "Utilogeny/source/core/sdl/freeserf/event_loop-sdl.h"

void launchSDLwindow() {
    Graphics& gfx = Graphics::get_instance();
    std::cout << "test\n";

    /* TODO move to right place */
    Audio& audio = Audio::get_instance();
    Audio::PPlayer player = audio.get_music_player();
    if (player) {
        //Audio::PTrack t = player->play_track(Audio::TypeMidiTrack0);
    }

    /* Init game loop */
    EventLoop& event_loop = EventLoop::get_instance();
    //event_loop.add_handler(&interface);

    /* Start game loop */
    //event_loop.run();

    //event_loop.del_handler(&interface);

}