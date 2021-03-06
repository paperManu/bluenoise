#
# Copyright (C) 2019 Emmanuel Durand
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Splash is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this software.  If not, see <http://www.gnu.org/licenses/>.
#

cmake_minimum_required(VERSION 3.12)
project(
    bluenoise 
    VERSION 0.0.1
    LANGUAGES C CXX
)

# Set default build type to Release
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# Force color output (useful when building with Ninja)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
   add_compile_options (-fdiagnostics-color=always)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
   add_compile_options (-fcolor-diagnostics)
endif ()


# ┏━╸┏━┓┏┓╻┏━╸╻┏━╸╻ ╻┏━┓┏━┓╺┳╸╻┏━┓┏┓╻
# ┃  ┃ ┃┃┗┫┣╸ ┃┃╺┓┃ ┃┣┳┛┣━┫ ┃ ┃┃ ┃┃┗┫
# ┗━╸┗━┛╹ ╹╹  ╹┗━┛┗━┛╹┗╸╹ ╹ ╹ ╹┗━┛╹ ╹
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# ╺┳┓┏━╸┏━┓┏━╸┏┓╻╺┳┓┏━╸┏┓╻┏━╸╻┏━╸┏━┓
#  ┃┃┣╸ ┣━┛┣╸ ┃┗┫ ┃┃┣╸ ┃┗┫┃  ┃┣╸ ┗━┓
# ╺┻┛┗━╸╹  ┗━╸╹ ╹╺┻┛┗━╸╹ ╹┗━╸╹┗━╸┗━┛
find_package(PkgConfig REQUIRED)
pkg_search_module(NCURSES REQUIRED ncurses)


# ┏━┓┏━┓╻ ╻┏━┓┏━╸┏━╸┏━┓
# ┗━┓┃ ┃┃ ┃┣┳┛┃  ┣╸ ┗━┓
# ┗━┛┗━┛┗━┛╹┗╸┗━╸┗━╸┗━┛
add_subdirectory(src)
