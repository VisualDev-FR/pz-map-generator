# Project Zomboid Random World Generator

This programs creates world generation files to be used in the Project Zomboid world editor tools.

It uses Procedural Generation to build unique worlds, re-using assets of the original game.

## Setup dev environnement

* Install msvc, then create env variable `MSVC_HOME` at location `Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207`
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