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

# Enable all warnings as errors
add_definitions(-Wall -Wextra -Werror)


# ╻┏┓╻┏━╸╻  ╻ ╻╺┳┓┏━╸┏━┓
# ┃┃┗┫┃  ┃  ┃ ┃ ┃┃┣╸ ┗━┓
# ╹╹ ╹┗━╸┗━╸┗━┛╺┻┛┗━╸┗━┛
include_directories(./)
include_directories(../external/)
include_directories(${NCURSES_INCLUDE_DIRS})


# ╻  ╻┏┓╻╻┏ ┏━┓
# ┃  ┃┃┗┫┣┻┓┗━┓
# ┗━╸╹╹ ╹╹ ╹┗━┛
link_directories(${NCURSES_LIBRARY_DIRS})


# ╺┳╸┏━┓┏━┓┏━╸┏━╸╺┳╸┏━┓
#  ┃ ┣━┫┣┳┛┃╺┓┣╸  ┃ ┗━┓
#  ╹ ╹ ╹╹┗╸┗━┛┗━╸ ╹ ┗━┛
add_executable(bluenoise)


# ┏━┓┏━┓╻ ╻┏━┓┏━╸┏━╸┏━┓
# ┗━┓┃ ┃┃ ┃┣┳┛┃  ┣╸ ┗━┓
# ┗━┛┗━┛┗━┛╹┗╸┗━╸┗━╸┗━┛
target_sources(bluenoise PRIVATE
    bluenoise.cpp
)

target_link_libraries(bluenoise ${NCURSES_LIBRARIES})
