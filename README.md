# FCEUmm (libretro)

FCEUmm is a libretro core based on FCE Ultra "mappers modified", an
unofficial build of FCE Ultra by CaH4e3 that adds support for a large
number of extra mappers, including many obscure and unlicensed boards.
It emulates the NES, Famicom, Famicom Disk System and Dendy, including
the VS. System arcade hardware.

Licensed under the GNU GPL v2. See the source headers for details.

## Supported content

| Extension | Content |
|---|---|
| `.nes` | iNES / NES 2.0 ROM images |
| `.fds` | Famicom Disk System images (requires BIOS, see below) |
| `.unf` / `.unif` | UNIF ROM images |

Region (NTSC / PAL / Dendy) is detected automatically from the ROM
database and header, and can be forced with the **Region** core option.

## BIOS

| File | Description | Required |
|---|---|---|
| `disksys.rom` | Famicom Disk System BIOS (8 KB), placed in the frontend's system directory | For FDS content only |
| `gamegenie.nes` | Game Genie ROM, placed in the system directory | Only when the **Game Genie Add-On** option is enabled |
| `nes.pal` | Custom 64-entry palette (192 bytes RGB), placed in the system directory | Optional |

## Features

- Wide mapper coverage, including hundreds of multicart, pirate and
  homebrew boards on top of the standard licensed set.
- Full expansion audio: FDS, MMC5, Namco 163, Sunsoft 5B, VRC6 and
  VRC7, each with an individual volume/enable option, plus per-channel
  toggles for the five internal APU channels.
- Selectable output samplerate with an **Auto** mode that matches the
  frontend's preferred rate, quality levels, stereo delay effect,
  low-pass filter, square-duty swap for famiclone-style sound, and DMC
  pop reduction.
- 22 built-in colour palettes (Wavebeam, Royaltea, the FBX set, Sony
  CXA, RGB PPU and more), a raw palette mode, custom palette support
  (`nes.pal`) and automatic per-game palettes from the internal
  database.
- Blargg NTSC filter (composite / S-Video / RGB / monochrome).
- Overscan cropping with independent top/bottom/left/right controls.
- Aspect ratio options: 8:7 PAR, 4:3, and PP (pixel perfect).
- CPU overclocking (2x, with post-render or vblank timing) for
  reducing slowdown in demanding games.
- Optional removal of the 8-sprites-per-scanline limit.
- Turbo buttons with configurable period, and an option to allow
  simultaneous opposing d-pad directions.
- VS. System DIP switches and coin input via the frontend's core
  options.
- Game Genie add-on emulation, savestates, configurable RAM power-on
  state ($FF / $00 / per-game deterministic random), runahead support, and
  in-game saves for battery-backed boards and the FDS.
- **Mesen-format HD texture packs** with custom backgrounds and
  OGG/WAV music and sound effects (see below).

## Input devices

Selectable per port from the frontend's controller menu:

| Device | Notes |
|---|---|
| Gamepad | Standard controller; turbo A/B available |
| 4-Player Adaptor | Famicom 4-player adaptor / NES Four Score (auto-enabled for known games) |
| Zapper | Light gun; can be driven by mouse, pointer/touch, or a lightgun device. Both classic and Sequential Targets light guns are supported ("Gun Aux A" serves as the light-sensor input in Sequential Targets mode) |
| Arkanoid paddle | Mouse, pointer, absolute-mouse and Stelladaptor modes |
| Power Pad A/B | Both button layouts |
| Family Trainer A/B | Famicom mat controller |
| Family Keyboard / Subor keyboard / PEC-586 keyboard | Requires frontend keyboard focus (Game Focus mode) |
| Oeka Kids tablet | Pointer-driven |
| Space Shadow gun | Hyper Shot light gun |
| Hyper Shot pads | Konami Hyper Shot |
| Mouse | With adjustable sensitivity |
| Crosshair | On-screen crosshair for gun games can be toggled |

## HD texture packs

The core can replace a game's graphics with high-resolution artwork
and its music with streamed audio using HD packs in the format
established by the Mesen emulator (`hires.txt`, format versions up to
109). Existing Mesen packs work as-is; the core also accepts some
additional file formats (see below).

### Setting up a pack

1. Make sure the **HD Texture Packs (Restart)** core option is enabled
   (it is by default).
2. Place the pack in the frontend's **system directory** under
   `HdPacks/<rom name>/`, where `<rom name>` is the content file name
   without its extension. The pack's `hires.txt` must sit directly in
   that folder:

   ```
   <system>/
     HdPacks/
       Mega Man (USA)/
         hires.txt
         tiles.png
         music_01.ogg
         ...
   ```

   For example, loading `Mega Man (USA).nes` looks for
   `<system>/HdPacks/Mega Man (USA)/hires.txt`.

3. Load (or restart) the game. When a pack is found the core switches
   to XRGB8888 output at the pack's scale factor; the log will show
   `HD pack loaded: scale Nx, ...`.

If nothing changes on screen, check that the folder name matches the
ROM file name exactly and that the frontend supports 32-bit
(XRGB8888) video output.

### Supported pack features

- Replacement tiles for both CHR ROM and CHR RAM games, at any integer
  scale from 1x to 10x, with per-tile brightness and transparency.
- All Mesen condition types: `hmirror`, `vmirror`, `bgpriority`,
  `sppalette0-3`, `tileAtPosition`, `tileNearby`, `spriteAtPosition`,
  `spriteNearby`, `memoryCheck`, `memoryCheckConstant`,
  `ppuMemoryCheck`, `ppuMemoryCheckConstant`, `frameRange`,
  `positionCheckX/Y` and `originPositionCheckX/Y`, including the `!`
  inverted forms.
- Custom background layers with scroll ratios, 40 priority levels
  across four compositing depths, and Alpha / Add / Subtract blend
  modes.
- Additional sprites (`<addition>`), fallback tiles (`<fallback>` and
  the `automaticFallbackTiles` option), custom palettes
  (`palette.dat`), and the `<options>` flags `disableSpriteLimit`,
  `alternateRegisterRange`, `disableCache` and `disableOriginalTiles`.
- Background music and sound effects (`<bgm>` / `<sfx>`) driven by the
  game through the HD audio device at `$4100-$4106` (or the alternate
  `$3002 + N*$10` write / `$4018-$4019` read range), with BGM looping,
  loop positions, 16 simultaneous sound-effect voices, and volume
  registers. The currently playing BGM track and position are saved in
  savestates and resume on load.

### File formats

Images referenced by `<img>` and `<background>` may be:

| Format | Notes |
|---|---|
| `.png` | As in Mesen |
| `.dds` | libretro extension; uncompressed and BC1-BC5 block-compressed |
| `.webp` | libretro extension; lossless (VP8L) and lossy (VP8) |

Audio referenced by `<bgm>` and `<sfx>` may be:

| Format | Notes |
|---|---|
| `.ogg` | Ogg Vorbis, as in Mesen |
| `.wav` | libretro extension; 16-bit PCM, mono or stereo, any samplerate |

### Differences from Mesen

- `<patch>` tags are parsed but not applied. To soft-patch a ROM,
  place an IPS file next to it and use FCEU's regular auto-patching.
- A pack's `<overscan>` setting is parsed but the core's own overscan
  crop options control the visible area.
- The Blargg NTSC filter is bypassed while a pack is active (the HD
  frame is composed directly).
- Mid-scanline palette changes are approximated per scanline.
- The PEC-586 board's interleaved CHR fetch layout is not supported.
- HD pack support is compiled out on very constrained targets
  (PS2, PSP, 3DS, DOS, GameCube, Wii).

## Building

```
make -f Makefile.libretro
```

Cross builds use the usual `platform=` targets (`win`, `osx`, `ios`,
`android`, `emscripten`, `wiiu`, `psl1ght`, `vita`, `libnx`, the
`windows_msvc*` toolchains, and others - see `Makefile.libretro`).
HD texture pack support is controlled by `HAVE_HDPACK` (enabled by
default on capable platforms) and bundles its own prefixed zlib, so no
external zlib is required on any target.
