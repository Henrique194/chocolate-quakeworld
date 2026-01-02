# Chocolate QuakeWorld

Chocolate QuakeWorld is a minimalist source port of QuakeWorld focused on
preserving the game original experience. Inspired by the philosophy of
Chocolate Doom, this project is dedicated to offering an authentic experience
that stays true to the game's roots, avoiding modern features or upgrades.
The focus is on preserving the classic QuakeWorld experience exactly as it was,
without introducing any contemporary enhancements.

# Philosophy

Like [Chocolate Quake](https://github.com/Henrique194/chocolate-quake),
Chocolate QuakeWorld is for players who value the classic QuakeWorld experience,
untouched by fancy modern upgrades. If you're seeking high-end visuals or
modern features, this port might not be for you. But if you want the game to
feel exactly like it did in the '90s, this is the place to be.

# Build Instructions

Chocolate QuakeWorld uses CMake (>= 3.21) and is built as a C99 project.
Predefined CMake presets are provided to simplify building across platforms.

The repository includes vcpkg as a Git submodule, which is the recommended way
to build Chocolate QuakeWorld on Windows and macOS.

## Requirements

* C compiler with C99 support
* CMake 3.21 or newer
* Ninja (recommended)
* Git
* SDL2 >= 2.26.5
* SDL2_net
* Audio libraries:
    * libvorbis + libvorbisfile
    * libflac
    * libmad (MP3)

## Cloning the repository

If you are not using vcpkg:
> git clone https://github.com/Henrique194/chocolate-quakeworld.git

If you plan to use vcpkg to manage dependencies:
> git clone
> --recurse-submodules https://github.com/Henrique194/chocolate-quakeworld.git

If you already cloned the repository without submodules and want to fetch
vcpkg later:
> git submodule update --init --recursive

## Windows and macOS (using bundled vcpkg)

The project provides CMake presets to build with vcpkg.
From the repository root:
> cmake --preset release-vcpkg
>
> cmake --build --preset release-vcpkg

For a debug build:
> cmake --preset debug-vcpkg
>
> cmake --build --preset debug-vcpkg

## Linux

On Linux, Chocolate QuakeWorld is typically built against system-provided
libraries.

Install the required dependencies using your distribution's package manager,
then build using the provided presets:
> cmake --preset release
>
> cmake --build --preset release

For a debug build:
> cmake --preset debug
>
> cmake --build --preset debug

## Build output

Once compilation is complete, you should have two executables: one to
run the client and other for the server. These executables can be found at:

- **Server (release)**: `cmake-build-release/src/server/Release`
- **Client (release)**: `cmake-build-release/src/client/Release`
- **Server (debug)**: `cmake-build-debug/src/server/Debug`
- **Client (debug)**: `cmake-build-debug/src/client/Debug`

# Running The Game

Both the Chocolate QuakeWorld client and server require an `qw` directory
containing the game data (PAK files) located alongside the executable.
You can customize the directory location using the `-basedir` command-line
parameter. Most IDEs also allow you to set a working directory, so you don't
need to manually input command-line parameters. Additionally, the server
requires the file `qwprogs.dat` to be present in the `qw` directory in order
to start correctly. Any custom skin packs should be placed in `qw/skins`.

## Music

Chocolate QuakeWorld supports external music playback in MP3, OGG, FLAC and
WAV formats. To enable it:

* Create a directory named `music` inside your `id1` game folder.
* Place your music tracks in this directory.

Tracks should follow the naming convention track02.ogg through track11.ogg,
matching the original CD audio.

# Supported Platforms

| Platform | is supported? |
|:--------:|:-------------:|
| Windows  |      yes      |
|  Linux   |      yes      |
|  MacOS   |      yes      |

# Credits

Chocolate QuakeWorld builds upon the work of the Quake community and
open-source contributors. Special thanks to:

* [QuakeSpasm Spiked](https://github.com/Shpoike/Quakespasm) - Portions of the
  sound and input subsystems are adapted from QuakeSpasm Spiked. Thanks to the
  authors for their solid groundwork.

Additional thanks to the broader Quake modding and source port community for
maintaining an ecosystem that made this project possible.
