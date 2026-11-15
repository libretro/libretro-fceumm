# AGENTS.md

Rules for anyone, human or agent, changing this core. Read the section
that covers the code you touch before touching it.

Accuracy comes first: the same guest-visible bytes, order and timing.
Within that, the goal is the least work the NES needs done, on the
slowest machine the core ships to. Those machines include a PS2, a PSP,
a 3DS, a GameCube, DOS and MIPS handhelds, not only the desktop the change
was written on.

## Ground rules

- **MSVC C89.** All code is C89 that MSVC compiles: `/* */` comments,
  declarations at the top of a block, no declarations in `for`, no
  designated initializers, compound literals or VLAs. The bar is MSVC's
  C89, not strict ISO: extensions every target compiler accepts, such as
  anonymous unions, stay. The GitLab CI's `windows-i686-msvc05` lane is
  the real check; `gcc -std=c89 -pedantic` on the touched files is the
  quick proxy.
- **Single-threaded.** The core has no threads, and it keeps none: no
  worker, no lock, no atomic. Speed comes from doing less work on the one
  thread, never from moving it to another.
- **Stack frames.** At most 4 KiB in the core's own code and 2 KiB in
  libretro-common.
- **libretro-common is a copy** of RetroArch's `libretro-common/`. Files
  under `src/drivers/libretro/libretro-common/` are plain syncs of it:
  what the core needs changed there goes into RetroArch first.
- **The libretro API is not refactored.** Its ABI is sacrosanct;
  everything the core owns is fair game.
- **Every platform branch.** A shared file is read and updated in every
  branch it has before the change ships: the big-endian `MSB_FIRST`
  builds (Wii, GameCube, Wii U, PS3, Xenon, PowerPC Mac), the PSP GU and
  PS2 gsKit paths in `retro_run_blit`, the 256-entry palette on PSP and
  PS2, and the RGB565/RGB888/ABGR1555 pixel formats.
- **Optional features are neutral when off.** HD packs, the NTSC filter,
  the stereo delay and Game Genie cost nothing on the hot paths when
  disabled beyond one predictable test. A feature that adds per-pixel or
  per-tile work for users who never enabled it is a regression.
- **Dead code is deleted.** Code nothing reaches is removed in the change
  that finds it. A table or function the compiler happens to drop as dead
  still goes from the source: the next reader believes the comment
  describing it.
- **Keep comments true** and short. Comments and commit messages
  describe what the code does now, never what it used to do or cost.

## The frame path

The frame path is `retro_run` and everything under it:
- `FCEUI_Emulate`: the 6502 interpreter (`x6502.c`, `ops.h`), mapper
  read and write handlers and IRQ hooks, `PPU_hook`, `GameHBIRQHook`, and
  the MMC5 paths;
- the PPU (`DoLine`, `RefreshLine`, `pputile.h`, `FetchSpriteData`,
  `CopySprites`);
- the APU (`sound.c`) and its filters;
- `retro_run_blit` and the audio hand-off.

With run-ahead, `retro_serialize` and `retro_unserialize` run every frame
too, so they are frame path.

- **One frame per run.** Each `retro_run` hands the frontend exactly one
  video frame and a gapless, non-duplicated run of audio samples. The
  frontend owns pacing; the core declares the console's refresh rate and
  never sleeps or reads a clock to pace itself.
- **No allocation mid-game.** From the first `retro_run` until unload,
  nothing allocates or frees: no `malloc`, `calloc`, `realloc` or `free`,
  and no buffer that grows when it first runs short. Storage is sized at
  content load or when an option changes, from the most that can be live
  at once. A full buffer takes a defined path (drop, reset, recycle); it
  never grows. "Only the first time" is still a stall at that frame.
- **No file I/O, no logging, no wall-clock reads.** Not even behind a
  setting. Counters for an investigation live in an uncommitted build.
- **No new indirection per byte, pixel or sample.** No function pointer,
  callback or switch added inside an inner loop that a per-line or
  per-frame decision could replace.

Not there yet. These are debts, not precedent; a change that touches one
removes it:

1. `memstream_open` (libretro-common) allocates its `memstream_t` on every
   `FCEUSS_Save_Mem` and `FCEUSS_Load_Mem` (`state.c`), so run-ahead
   allocates every frame. The fix is a caller-owned stream in RetroArch's
   libretro-common, then synced here.
2. `stereo_filter_apply_delay` grows its delay buffer during play
   (`malloc`, `memcpy`, `free`) when a batch exceeds its capacity.
3. `retro_serialize_size` allocates 1 MB and saves a full state into it on
   its first call.
4. The HD pack replacement cache (`hd_add_cache_build`, `hdpack_render.c`)
   grows its map and node array during play.

## Speed of light

Compare a path with what the NES needs done, not with the last build.

An NTSC frame is about 29,781 CPU cycles and 89,342 PPU dots. It produces
61,440 visible pixels written once into the frontend's buffer, and one
frame's worth of audio samples. That is the floor. Every byte moved,
cache line touched, instruction run, branch missed and register spilled
above it is a defect to remove, or a cost with a named reason.

Work in this order:

1. **Remove the work.** No representation, copy, table rebuild or pass
   without a consumer. A table that depends on state is rebuilt when that
   state changes, not every line or call. The background pair LUT that
   was rebuilt on every `RefreshLine` cost more than the per-tile work it
   saved.
2. **Keep data where it is.** Mutate in place, write the final form once,
   lend instead of copying.
3. **Prepare at load** what cannot be removed: lookup tables are built at
   load or on a palette or option change, never on the frame path.
4. **Only then tune instructions.**

Acceptance is exact output first, then a measured gain. A smaller copy
count, a rewrite or an instruction count alone does not prove a faster
core. Any measured or provable gain is welcome however small, and only
win-win changes ship: no trading speed on one target or pixel format for
speed on another.

## Zero-copy and borrowed lends

Every change looks for copies to remove in the code it touches, and every
delivery says what it found and removed. A copy that stays has a reason a
reader can see.

Classify a copy before touching it:
- format conversion;
- immutable snapshot;
- ownership handoff;
- API staging;
- redundant duplicate.

Only the last is simply deleted. Find out why the others exist first.

- **Write the final representation once**, straight into its
  destination. The blit converts XBuf indices into the frontend's lent
  framebuffer; a new stage between the two is a regression.
- **A lend outlives its last consumer.** State who ends it. The frontend's
  buffers are valid only for the call they come with.
- **Reset counts rather than clearing payloads.** Do not clear bytes about
  to be fully overwritten; prove the full overwrite first.
- **Validate before writing.** Reject bad offsets and sizes before
  touching a destination. Use half-open, overflow-safe ranges; a pitch is
  a row stride, not an offset.
- **No slurps.** Files the core reads go through libretro-common's
  filestream/VFS, read in place or streamed, never read whole into a fresh
  allocation only to be parsed and freed.

Lends in place, which must stay:

- `retro_run_blit` writing into the buffer from
  `RETRO_ENVIRONMENT_GET_CURRENT_SOFTWARE_FRAMEBUFFER`, with
  `fceu_video_out` as the fallback.
- The NTSC filter's output handed to `video_cb` with its own wide pitch,
  with no repack.
- `WaveFinal` packed in place into interleaved 16-bit stereo and handed to
  `audio_batch_cb` (`stereo_filter_apply_null`).
- The HD pack's composed frame handed to `video_cb` directly.
- `retro_serialize` and `retro_unserialize` reading and writing the
  frontend's buffer in place.

Intermediates that stay, and why:

- **`XBuf` and `XDBuf`.** These hold the PPU's 8-bit palette index per
  pixel and the emphasis bits per scanline. Sprites composite into
  `XBuf`, and the NTSC filter and HD packs read the indices. The blit
  reads one byte ahead past a row's last 8-pixel block, which relies on
  the 8 spare lines `video.c` allocates past the visible area; keep them.

## Codegen, caches and registers

Read what the compiler made for any hot path you change. Checking
codegen on one compiler and target says nothing about the others.

- **Check the shipping flags and the outliers.** gcc `-O2` is what most
  platforms build with. The classic ARM platforms
  (`classic_armv7_a7`, `classic_armv8_a35`) build with `-Ofast`. Apple,
  Android and iOS build with clang. A change is a win on all of them or
  on none.
- **`uint8_t` stores alias everything.** A store through a byte pointer
  may alias every other pointer, so loads after it are reloaded. In a
  loop over `XBuf`, `PALRAM` or a pixel row, fetch a group of values,
  then store the group.
- **Keep scalar loops scalar.** At `-O3`/`-Ofast`, gcc vectorizes table
  lookups into emulated gathers (scalar loads shuffled into vector
  registers). On x86 SSE2 and ARMv7 NEON that is slower than the scalar
  loop and several times the code. Check with `-fopt-info-vec`. A small
  loop-carried value, such as an index read one block ahead, keeps the
  loop out of the vectorizer at no cost.
- **Registers are scarce.** i686 has 7 usable general registers and the
  32-bit ARM and MIPS targets few more. A loop with more live values than
  that spills on every iteration. Clang also reorders loads freely, so a
  shape that is fine on gcc can spill on clang: look at both.
- **64-bit arithmetic costs extra on 32-bit targets.** Shifts, multiplies
  and `uint64_t` packing that are one instruction on x86-64 are several
  on armv7, MIPS and PowerPC 32. Prefer 32-bit and byte-wide work on hot
  paths.
- **Count cache lines.** The data caches the core has to fit in are
  small: 8 KiB on the PS2's EE, 16 KiB on the PSP and the 3DS's ARM11,
  32 KiB on the Wii. A table on a hot path is sized against them, and
  what an access touches is part of its cost.
  - `ARead` and `BWrite` are 64 K function pointers each (1 MiB together
    on 64-bit hosts), indexed on every CPU memory access.
  - `ppulut1`/`ppulut2`/`ppulut3` and `PALRAM` are hot every tile.
  - Do not add a table where a few instructions do.
- **Trivial helpers are macros.** A one-expression helper on a hot path
  is a macro: there is no guarantee `INLINE` is honoured on every
  compiler.
- **Struct members** are sorted by alignment, largest first, bools last.
  Width and height pairs the core owns are packed into one value where
  they are touched.

## Quantization outside emulation

What the NES computes is emulated as it computes it. Everything the host
adds around it loses no precision the NES did not lose: palette
conversion to the frontend's pixel format, deemphasis, the NTSC filter,
audio filtering, stereo and mixing.

- **Round, don't floor.** A value cut to fewer bits rounds to the nearest
  step. A shift floors: that is a darker picture, and a DC offset in
  audio.
- **Widen by replicating bits.** 5 bits become 8 as `(v << 3) | (v >> 2)`,
  so white stays 255.
- **Round once, at the end.** Intermediates keep their fraction bits;
  audio sums in 32 bits and clamps to 16 only where it stores.

Not there yet: `FCEUD_SetPalette` builds RGB565 and 1555 pixels with
`r >> RED_EXPAND` and the like, which floors.

## Accuracy and guest progress

- **Faster by doing less is a regression.** Frame count, audio totals,
  IRQ and NMI timing, and the picture must match.
- **Same frames.** A performance change shows byte-identical frames
  against its parent on a set of ROMs that reach the changed path. Cover
  the cases it branches on: NSF, MMC5, emphasis, raw palette, crop
  settings, both pixel formats, and HD packs if touched.
- **Savestates stay compatible.** A state an older build saved loads the
  same. SFORMAT chunk changes keep older states loading
  (`retro_unserialize` accepts sizes other than its own), and a value
  saved in an old form is still read in that form.

## Measuring honestly

- **Do the measuring yourself**, with a headless frontend that loads the
  core and runs ROMs with no input. The `tests/` harnesses show the shape.
- **Time separately from correctness.** Hashing frames in a timing run
  adds work to what is being timed.
- **Clean runs.** Time on an idle machine, alternate parent and candidate
  (ABBA), and report several runs with their spread, not a single run.
  This container-class noise is a few percent: a smaller difference is
  not a result.
- **Count instructions for small differences.** `valgrind
  --tool=callgrind` counts are exact per run and show what changed in a
  function. They are work results, not time: say which you are reporting.
- **Give the workload and the build.** ROM, frame count, compiler, flags,
  pixel format. Keep bad results visible, including a regression on one
  compiler or flag set.
- **Synthetic or public test content only.** Generated images, or public
  homebrew and test ROMs. No game names and no commercial ROMs in files,
  comments or messages.

## Tests and delivery

- **A bug fix ships with its test** in `tests/`, in the same patch. It
  fails on the old code and passes on the fix: show it failing. Tests use
  generated images, like `tests/onebus_chr.py`, and build and run on
  Windows and Linux. They are run by hand; adding or changing CI lanes is
  the developer's call.
- **Build what you touched, everywhere it ships**, before delivery:
  - `make -f Makefile.libretro -j$(nproc)` builds clean, and the build log
    is read for warnings in the touched files, not only errors.
  - Touched C files pass `-std=c89 -pedantic` with the build's own flags.
  - A touched shared file also gets a syntax pass with `-DMSB_FIRST`, and
    with `PSP` and `PS2` defined where it has those branches.
  - Memory and pointer changes run under `-fsanitize=address,undefined`.
    UBSan already reports a few signed shifts on master (`ppu.c`,
    `libretro.c`, `boards/mmc5.c`): a change adds none.
- **Patches.**
  - Deliver `git format-patch` mailbox patches, cut after fetching and
    rebasing onto the current `origin/master`, and checked with
    `git am --keep-cr` on a clean checkout.
  - Fixes to a contributor's pull request are stacked on its head commit
    and must still merge clean onto `origin/master`.
  - Author: `LibretroAdmin <libretroadmin@users.noreply.github.com>`. No
    AI attribution trailers.
  - Commit messages are short and sized to the change.
- **Core options** are connected to something. Do not add one the core
  does not read; remove one that does nothing.

## What does not carry over from the other cores

The parallel cores' rules on shaders, pipelines, pipeline caches, GPU
resources, JIT code caches, worker threads and lock-free queues have
nothing to apply to here: this core renders in software on one thread.
What carries over is the discipline:
- the speed of light;
- no allocation in play;
- zero-copy and borrowed lends;
- codegen read, not assumed;
- precision kept;
- honest measurement.

## These files

Only the developer writes to or deletes `AGENTS.md`. An agent does not
change it on its own; it proposes the change to the developer instead.
