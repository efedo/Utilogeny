/*
 * gfx.cc - General graphics and data file functions
 *
 * Copyright (C) 2013-2019  Jon Lund Steffensen <jonlst@gmail.com>
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

#include "Utilogeny/source/core/precomp.h"
#include "Utilogeny/source/core/exceptions.h"
#include "Utilogeny/source/core/sdl/freeserf/gfx.h"
#include <utility>
#include <algorithm>
#include "Utilogeny/source/core/sdl/freeserf/data.h"
#include "Utilogeny/source/core/sdl/freeserf/video.h"

const Color Color::black = Color(0x00, 0x00, 0x00);
const Color Color::white = Color(0xff, 0xff, 0xff);
const Color Color::green = Color(0x73, 0xb3, 0x43);
const Color Color::transparent = Color(0x00, 0x00, 0x00, 0x00);

double Color::get_cyan() const 
{
  double k = get_key();
  return (1. - (static_cast<double>(r)/255.) -k ) / (1. - k);
}

double Color::get_magenta() const 
{
  double k = get_key();
  return (1. - (static_cast<double>(g)/255.) -k ) / (1. - k);
}

double Color::get_yellow() const 
{
  double k = get_key();
  return (1. - (static_cast<double>(b)/255.) -k ) / (1. - k);
}

double Color::get_key() const 
{
  return 1. - std::max(static_cast<double>(r)/255.,
                       std::max(static_cast<double>(g)/255.,
                       static_cast<double>(b)/255.));
}

Image::Image(Video *_video, Data::PSprite sprite) 
{
  video = _video;
  width = static_cast<unsigned int>(sprite->get_width());
  height = static_cast<unsigned int>(sprite->get_height());
  offset_x = sprite->get_offset_x();
  offset_y = sprite->get_offset_y();
  delta_x = sprite->get_delta_x();
  delta_y = sprite->get_delta_y();
  video_image = video->create_image(sprite->get_data(), width, height);
}

Image::~Image() 
{
  if (video_image != nullptr) {
    video->destroy_image(video_image);
    video_image = nullptr;
  }
  video = nullptr;
}

/* Sprite cache hash table */
Image::ImageCache Image::image_cache;

void Image::cache_image(uint64_t id, Image *image)
{
  image_cache[id] = image;
}

/* Return a pointer to the sprite pointer associated with id. */
Image * Image::get_cached_image(uint64_t id)
{
  ImageCache::iterator result = image_cache.find(id);
  if (result == image_cache.end()) {
    return nullptr;
  }
  return result->second;
}

void Image::clear_cache()
{
  while (!image_cache.empty()) {
    Image *image = image_cache.begin()->second;
    image_cache.erase(image_cache.begin());
    delete image;
  }
}

Graphics *Graphics::instance = nullptr;

Graphics::Graphics()
{
  if (instance != nullptr) {
    throwl("Unable to create second instance.");
  }

  try {
    video = &Video::get_instance();
  } catch_rethrow("test");
      
  Data &data = Data::get_instance();
  //Data::PSource data_source = data.get_data_source();
  Data::PSprite sprite = 0;//data_source->get_sprite(Data::AssetCursor, 0,
  //                                               {0, 0, 0, 0});
  //video->set_cursor(sprite->get_data(),
  //                  static_cast<unsigned int>(sprite->get_width()),
  //                  static_cast<unsigned int>(sprite->get_height()));

  Graphics::instance = this;
}

Graphics::~Graphics()
{
  Image::clear_cache();
}

Graphics & Graphics::get_instance()
{
  static Graphics graphics;
  return graphics;
}

/* Draw the opaque sprite with data file index of
   sprite at x, y in dest frame. */
void Frame::draw_sprite(int x, int y, Data::Resource res, unsigned int index)
{
  draw_sprite(x, y, res, index, false, Color::transparent, 1.f);
}

void Frame::draw_sprite(int x, int y, Data::Resource res, unsigned int index,
                   bool use_off, const Color &color, float progress)
{
  Data::Sprite::Color pc = {color.get_blue(),
                            color.get_green(),
                            color.get_red(),
                            color.get_alpha()};

  //video->draw_image(image->get_video_image(), x, y, y_off, video_frame);
}


void Frame::draw_sprite(int x, int y, Data::Resource res, unsigned int index, bool use_off)
{
  draw_sprite(x, y, res, index, use_off, Color::transparent, 1.f);
}

void Frame::draw_sprite(int x, int y, Data::Resource res, unsigned int index, bool use_off, float progress)
{
  draw_sprite(x, y, res, index, use_off, Color::transparent, progress);
}

void Frame::draw_sprite(int x, int y, Data::Resource res, unsigned int index, bool use_off, const Color &color)
{
  draw_sprite(x, y, res, index, use_off, color, 1.f);
}

/* Draw the masked sprite with given mask and sprite
   indices at x, y in dest frame. */
void Frame::draw_masked_sprite(int x, int y, Data::Resource mask_res, unsigned int mask_index, Data::Resource res, unsigned int index)
{
  //video->draw_image(image->get_video_image(), x, y, 0, video_frame);
}

/* Draw a character at x, y in the dest frame. */
void Frame::draw_char_sprite(int x, int y, unsigned char c, const Color &color, const Color &shadow)
{
}

/* Draw the string str at x, y in the dest frame. */
void Frame::draw_string(int x, int y, const std::string &str, const Color &color, const Color &shadow)
{
  int cx = x;

  for (char c : str) {
    if (c == '\t') {
      cx += 8 * 2;
    } else if (c == '\n') {
      y += 8;
      cx = x;
    } else {
      draw_char_sprite(cx, y, c, color, shadow);
      cx += 8;
    }
  }
}

/* Draw the number n at x, y in the dest frame. */
void Frame::draw_number(int x, int y, int value, const Color &color, const Color &shadow)
{
  if (value < 0) {
    draw_char_sprite(x, y, '-', color, shadow);
    x += 8;
    value *= -1;
  }

  if (value == 0) {
    draw_char_sprite(x, y, '0', color, shadow);
    return;
  }

  int digits = 0;
  for (int i = value; i > 0; i /= 10) digits += 1;

  for (int i = digits-1; i >= 0; i--) {
    draw_char_sprite(x+8*i, y, '0'+(value % 10), color, shadow);
    value /= 10;
  }
}

/* Draw a rectangle with color at x, y in the dest frame. */
void Frame::draw_rect(int x, int y, int width, int height, const Color &color) {
  Video::Color c = { color.get_red(),
                     color.get_green(),
                     color.get_blue(),
                     color.get_alpha() };
  video->draw_rect(x, y, width, height, c, video_frame);
}

/* Draw a rectangle with color at x, y in the dest frame. */
void Frame::fill_rect(int x, int y, int width, int height, const Color &color) {
  Video::Color c = { color.get_red(),
                     color.get_green(),
                     color.get_blue(),
                     color.get_alpha() };
  video->fill_rect(x, y, width, height, c, video_frame);
}

/* Initialize new graphics frame. If dest is NULL a new
   backing surface is created, otherwise the same surface
   as dest is used. */
Frame::Frame(Video *video_, unsigned int width, unsigned int height)
{
  video = video_;
  video_frame = video->create_frame(width, height);
  owner = true;
  //data_source = Data::get_instance().get_data_source();
}

Frame::Frame(Video *video_, Video::Frame *video_frame_)
{
  video = video_;
  video_frame = video_frame_;
  owner = false;
  //data_source = Data::get_instance().get_data_source();
}

/* Deinitialize frame and backing surface. */
Frame::~Frame() 
{
  if (owner) {
    video->destroy_frame(video_frame);
  }
  video_frame = nullptr;
}

/* Draw source frame from rectangle at sx, sy with given
   width and height, to destination frame at dx, dy. */
void Frame::draw_frame(int dx, int dy, int sx, int sy, Frame *src, int w, int h) 
{
  video->draw_frame(dx, dy, video_frame, sx, sy, src->video_frame, w, h);
}

void Frame::draw_line(int x, int y, int x1, int y1, const Color &color) {
  Video::Color c = {color.get_red(),
                    color.get_green(),
                    color.get_blue(),
                    color.get_alpha()};
  video->draw_line(x, y, x1, y1, c, video_frame);
}

Frame * Graphics::create_frame(unsigned int width, unsigned int height) {
  return new Frame(video, width, height);
}

/* Enable or disable fullscreen mode */
void Graphics::set_fullscreen(bool enable) {
  video->set_fullscreen(enable);
}

/* Check whether fullscreen mode is enabled */
bool Graphics::is_fullscreen() {
  return video->is_fullscreen();
}

Frame * Graphics::get_screen_frame() {
  return new Frame(video, video->get_screen_frame());
}

void Graphics::set_resolution(unsigned int width, unsigned int height,
                         bool fullscreen) {
  video->set_resolution(width, height, fullscreen);
}

void Graphics::get_resolution(unsigned int *width, unsigned int *height) {
  video->get_resolution(width, height);
}

void Graphics::swap_buffers() {
  video->swap_buffers();
}

float Graphics::get_zoom_factor() {
  return video->get_zoom_factor();
}

bool Graphics::set_zoom_factor(float factor) {
  return video->set_zoom_factor(factor);
}

void Graphics::get_screen_factor(float *fx, float *fy) {
  video->get_screen_factor(fx, fy);
}
