# Project Zomboid Random World Generator

This programs creates world generation files to be used in the Project Zomboid world editor tools.

It uses Procedural Generation to build unique worlds, re-using assets of the original game.

## Setup dev environnement

* Install [mingw64](https://github.com/niXman/mingw-builds-binaries/releases), then create env variable `MINGW64_HOME` at the root directory
* Add `%MINGW64_HOME%/bin` to the path environnement variable
* Intall vcpkg, then create env variable `VCPKG_HOME` at the root directory
* Build project with [helpers/build.bat](./helpers/build.bat)

## Roadmap

### Files parsing
- [x] Implement game file readers
- [ ] Implement game file writers

### Files Ripping
- [ ] Implement game rooms ripper

### Previewer
- [ ] Setup static cell viewer from gamefiles
- [ ] Setup dynamic world viewer from gamefiles

## Issues

* window title is white, even in dark mode (see https://github.com/godotengine/godot-proposals/issues/1868#issuecomment-1166589949)