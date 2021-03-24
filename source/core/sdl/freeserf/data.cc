/*
 * data.cc - Game resources file functions
 *
 * Copyright (C) 2014-2019  Wicked_Digger <wicked_digger@mail.ru>
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
#include "Utilogeny/source/core/sdl/freeserf/data.h"
#include <vector>
#include <memory>
#include <utility>
#include <string>
#include <functional>

#ifdef _WIN32
// need for GetModuleFileName
#include <Windows.h>
#endif

typedef struct DataResource {
  Data::Resource resource;
  Data::Type type;
  unsigned int count;
  const char *name;
} DataResource;

DataResource data_resources[] = {
  { Data::AssetNone,         Data::TypeUnknown,   0,   "error"         },
  { Data::AssetArtLandscape, Data::TypeSprite,    1,   "art_landscape" },
}; // Trimmed

Data::Data() : data_source(nullptr) {}

Data & Data::get_instance() {
  static Data instance;
  return instance;
}

Data::~Data() {
}

// Try to load data file from given path or standard paths.
//
// Return true if successful. Standard paths will be searched only if the
// given path is empty string.
//bool Data::load(const std::string &path) {
//  // If it is possible, prefer DOS game data.
//  typedef std::function<Data::PSource(const std::string &)> SourceFactory;
//  std::vector<SourceFactory> sources_factories;
//
//  std::list<std::string> search_paths;
//  if (path.empty()) {
//    search_paths = get_standard_search_paths();
//  } else {
//    search_paths.push_front(path);
//  }
//
//  // Use each data source to try to find the data files in the search paths.
//  for (const SourceFactory &factory : sources_factories) {
//    for (const std::string &path : search_paths) {
//      Data::PSource source = factory(path);
//      if (source->check()) {
//        std::cout << "Game data found in '" << source->get_path()
//                          << "'...";
//        if (source->load()) {
//          data_source = std::move(source);
//          break;
//        }
//      }
//    }
//    if (data_source) {
//      break;
//    }
//  }
//
//  return data_source.get() != nullptr;
//}

// Return standard game data search paths for current platform.
//std::list<std::string> Data::get_standard_search_paths() const {
//  // Data files are searched for in some common directories, some of which are
//  // specific to the platform we're running on.
//  //
//  // On platforms where the XDG specification applies, the data file is
//  // searched for in the directories specified by the
//  // XDG Base Directory Specification
//  // <http://standards.freedesktop.org/basedir-spec/basedir-spec-latest.html>.
//  //
//  // On Windows platforms the %localappdata% is used in place of
//  // XDG_DATA_HOME.
//
//  std::list<std::string> paths;
//
//  // Add path where base is obtained from an environment variable and can
//  // be nullptr or empty.
//  auto add_env_path = [&](const char* base, const char* suffix) {
//    if (base != nullptr) {
//      std::string b(base);
//      if (!b.empty()) paths.push_back(b + "/" + suffix);
//    }
//  };
//
//  // Look in current directory
//  paths.push_back(".");
//
//  // Look in data directories under the home directory
//  add_env_path(std::getenv("XDG_DATA_HOME"), "freeserf");
//  add_env_path(std::getenv("HOME"), ".local/share/freeserf");
//  add_env_path(std::getenv("HOME"), ".local/share/freeserf/custom");
//
//#ifdef _WIN32
//  // Look in the same directory as the freeserf.exe app.
//  char app_path[MAX_PATH + 1];
//  GetModuleFileNameA(NULL, app_path, MAX_PATH);
//  add_env_path(app_path, "../");
//
//  // Look in windows XDG_DATA_HOME equivalents.
//  add_env_path(std::getenv("userprofile"), ".local/share/freeserf");
//  add_env_path(std::getenv("LOCALAPPDATA"), "freeserf");
//#endif
//
//  // Search in global directories.
//#ifdef _WIN32
//  const char *env = std::getenv("PATH");
//  #define PATH_SEPERATING_CHAR ';'
//#else
//  const char *env = std::getenv("XDG_DATA_DIRS");
//  #define PATH_SEPERATING_CHAR ':'
//#endif
//
//  std::string dirs = (env == nullptr) ? std::string() : env;
//  size_t next_path = 0;
//  while (next_path != std::string::npos) {
//    size_t pos = dirs.find(PATH_SEPERATING_CHAR, next_path);
//    std::string dir = dirs.substr(next_path, pos);
//    next_path = (pos != std::string::npos) ? pos + 1 : pos;
//    add_env_path(dir.c_str(), "freeserf");
//  }
//
//#ifndef _WIN32
//  paths.push_back("/usr/local/share/freeserf");
//  paths.push_back("/usr/share/freeserf");
//#endif
//
//  return paths;
//}

//
//Data::Type Data::get_resource_type(Resource resource) {
//  return data_resources[resource].type;
//}
//
//unsigned int Data::get_resource_count(Resource resource) {
//  return data_resources[resource].count;
//}
//
//const std::string Data::get_resource_name(Resource resource) {
//  return data_resources[resource].name;
//}
