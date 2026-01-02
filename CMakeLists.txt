cmake_minimum_required(VERSION 3.21.0)
project("Chocolate QuakeWorld" VERSION 1.0.0 LANGUAGES C)
set(CMAKE_C_STANDARD 99)

string(TIMESTAMP CURRENT_YEAR "%Y")
set(PACKAGE_NAME ${PROJECT_NAME})
set(PACKAGE_TARNAME "chocolate-quakeworld")
set(PACKAGE_VERSION ${PROJECT_VERSION})
set(PACKAGE_STRING "${PROJECT_NAME} ${PROJECT_VERSION}")
set(PACKAGE_COPYRIGHT "Copyright (C) 1996-${CURRENT_YEAR}")
set(PACKAGE_LICENSE "GNU General Public License, version 3")

# Find math library
find_library(MATH m)
if(MATH)
    link_libraries(${MATH})
endif()

find_package(SDL2 2.26.5 REQUIRED)
find_package(SDL2_net CONFIG REQUIRED)
if(UNIX AND NOT APPLE)
	find_library(VORBIS_LIBRARY vorbis REQUIRED)
	find_library(VORBISFILE_LIBRARY vorbisfile REQUIRED)
	find_library(MAD_LIBRARY mad REQUIRED)
	find_library(FLAC_LIBRARY FLAC REQUIRED)
else()
	find_package(Vorbis CONFIG REQUIRED)
	find_package(mad CONFIG REQUIRED)
	find_package(FLAC CONFIG REQUIRED)
endif()

configure_file(config.h.in config.h)

add_subdirectory("src")
