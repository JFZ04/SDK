# cmake_uninstall.cmake.in

# Copyright (C) 2023 The C++ Plus Project.
# This file is part of the build-aux library.
#
# The build-aux is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 3 of the
# License, or (at your option) any later version.
#
# The build-aux is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.    See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with the build-aux; see the file COPYING.    If not,
# see <https://www.gnu.org/licenses/>.

# Uninstall script.

if(NOT EXISTS "D:/Project/cppp-reiconv-v2.1.0/cmake-build-debug/install_manifest.txt")
    message(FATAL_ERROR "Cannot find install manifest: \"D:/Project/cppp-reiconv-v2.1.0/cmake-build-debug/install_manifest.txt\"")
endif()

file(READ "D:/Project/cppp-reiconv-v2.1.0/cmake-build-debug/install_manifest.txt" files)
string(REGEX REPLACE "\n" ";" files "${files}")
foreach(file ${files})
    message(STATUS "Uninstalling $ENV{DESTDIR}${file}")
    if(IS_SYMLINK "$ENV{DESTDIR}${file}" OR EXISTS "$ENV{DESTDIR}${file}")
        exec_program(
            "D:/Toolsbox/CLion/bin/cmake/win/x64/bin/cmake.exe" ARGS "-E remove \"$ENV{DESTDIR}${file}\""
            OUTPUT_VARIABLE rm_out
            RETURN_VALUE rm_retval
            )
        if(NOT "${rm_retval}" STREQUAL 0)
            message(FATAL_ERROR "Problem when removing $ENV{DESTDIR}${file}")
        endif()
    else(IS_SYMLINK "$ENV{DESTDIR}${file}" OR EXISTS "$ENV{DESTDIR}${file}")
        message(STATUS "File $ENV{DESTDIR}${file} does not exist.")
    endif()
endforeach()
