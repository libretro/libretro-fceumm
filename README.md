[![Build Status](https://travis-ci.org/libretro/libretro-fceumm.svg?branch=master)](https://travis-ci.org/libretro/libretro-fceumm)
[![Build status](https://ci.appveyor.com/api/projects/status/etk1vcouybahdbkt/branch/master?svg=true)](https://ci.appveyor.com/project/bparker06/libretro-fceumm/branch/master)

# FCE Ultra mappers modified (FCEUmm)
FCEU "mappers modified" is an unofficial build of FCEU Ultra by CaH4e3, which supports a lot of new mappers including some obscure mappers such as one for unlicensed NES ROM's.

Recent Changes (in no particular order):
- additional mappers support
- mapper fixes and updates
- additional input options (SNES Mouse, SNES Gamepad, PowerPad A/B, VirtualBoy Controller etc)
- variable overscan cropping options, including separate top, bottom, left, right overscan cropping
- replaced on/off audio options to volume controls
- add volume controls for expansion audio (FDS, MMC5, VRC6, VRC7, Namco163, Sunsoft5B)
- fix to audio controls not muting some channels when in low quality mode
- add missing state variables, fixing runahead compatibility
- Assign F12 as Hard Reset (PowerNES) hotkey
- Write instructions now update the databus (backport https://github.com/TASEmulators/fceux/pull/659)
- misc changes under the hood

The core can be compiled similarly to libretro-fceumm. and be a drop-in replacement.

NOTE: With the massive changes, savestates are not compatible between versions, and even on possible future commits. SRAM are mostly compatible though.
