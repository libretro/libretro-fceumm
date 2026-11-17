#!/usr/bin/env python3
"""Test mid-scanline $2001 greyscale changes with a generated ROM.

Usage: python3 tests/ppu_midline_greyscale.py /path/to/fceumm_libretro.so
The same command accepts a .dylib or .dll on its native platform.
"""

import ctypes as C
from pathlib import Path
import struct
import sys
import tempfile
import ctypes
import os


class Variable(C.Structure):
    _fields_ = [('key', C.c_char_p), ('value', C.c_char_p)]


class GameInfo(C.Structure):
    _fields_ = [('path', C.c_char_p), ('data', C.c_void_p),
                ('size', C.c_size_t), ('meta', C.c_char_p)]


def make_rom():
    prg = bytearray(16384)
    chr_rom = bytes(8192)  # All tile 0 pixels = color index 0 (backdrop)

    def write(buf, address, value):
        buf.extend([0xa9, value, 0x8d, address & 255, address >> 8])

    # Reset handler at $C000
    reset = bytearray.fromhex('78 d8 a2 ff 9a')  # SEI; CLD; LDX #$ff; TXS
    write(reset, 0x2000, 0x00)
    write(reset, 0x2001, 0x00)
    # Wait two vblanks for PPU warmup.
    reset.extend(bytes.fromhex('2c 02 20 2c 02 20 10 fb 2c 02 20 10 fb'))
    # Set backdrop color ($3F00) to $16 (chromatic red).
    write(reset, 0x2006, 0x3f)
    write(reset, 0x2006, 0x00)
    write(reset, 0x2007, 0x16)
    # Reset scroll ($2005 x2) and enable NMI + background rendering ($2000/$2001).
    write(reset, 0x2005, 0x00)
    write(reset, 0x2005, 0x00)
    write(reset, 0x2000, 0x80)
    write(reset, 0x2001, 0x08)
    loop = 0xc000 + len(reset)
    reset.extend([0x4c, loop & 255, loop >> 8])
    prg[0:len(reset)] = reset

    # NMI handler at $C100:
    # VBlank is 20 scanlines (~2273 CPU cycles) + ~91 visible scanlines (~11367 cycles)
    # = ~13640 CPU cycles from NMI entry to mid-scanline 91.
    nmi = bytearray()
    nmi.extend(bytes.fromhex('a2 0a a0 ff 88 d0 fd ca d0 f8'))  # outer/inner delay
    # Shifts span from [168, 256) to ~[63, 151)
    nmi.extend(bytes.fromhex('a0 a5 88 d0 fd'))
    # Toggle greyscale ($01) on with no emphasis ($00) while keeping BG on ($08) -> $09.
    write(nmi, 0x2001, 0x09)
    # Hold for ~28 CPU cycles (~84 PPU dots) across scanline 91.
    nmi.extend(bytes.fromhex('a0 05 88 d0 fd'))
    # Restore normal color ($00) + BG on ($08) -> $08.
    write(nmi, 0x2001, 0x08)
    nmi.append(0x40)  # RTI
    prg[0x0100:0x0100 + len(nmi)] = nmi

    # Vectors at $FFFA: NMI=$C100, RESET=$C000, IRQ=$C000
    prg[-6:] = bytes.fromhex('00 c1 00 c0 00 c0')

    header = bytearray(b'NES\x1a' + bytes(12))
    header[4] = 1  # 1 x 16 KiB PRG-ROM
    header[5] = 1  # 1 x 8 KiB CHR-ROM
    return bytes(header + prg + chr_rom)


def is_neutral_grey(px):
    r = (px >> 16) & 0xFF
    g = (px >> 8) & 0xFF
    b = px & 0xFF
    return r == g == b


def find_core():
    if len(sys.argv) > 1:
        return sys.argv[1]
    for name in ("fceumm_libretro.so", "fceumm_libretro.dll", "fceumm_libretro.dylib"):
        if os.path.exists(name):
            return os.path.abspath(name)
        parent = os.path.join("..", name)
        if os.path.exists(parent):
            return os.path.abspath(parent)
    raise FileNotFoundError("Could not find fceumm_libretro shared library")


def main():
    core_path = find_core()
    lib = ctypes.CDLL(core_path)
    variables = {b'fceumm_ramstate': b'fill $00'}
    frame_holder = [None]
    failures = []

    with tempfile.TemporaryDirectory(prefix='ppu-greyscale-') as directory:
        encoded_directory = directory.encode()

        @C.CFUNCTYPE(C.c_bool, C.c_uint, C.c_void_p)
        def environment(command, data):
            if command in (9, 30, 31):
                C.cast(data, C.POINTER(C.c_char_p))[0] = encoded_directory
                return True
            if command == 3:
                C.cast(data, C.POINTER(C.c_bool))[0] = True
                return True
            if command == 10:  # SET_PIXEL_FORMAT -> XRGB8888 (1)
                C.cast(data, C.POINTER(C.c_int))[0] = 1
                return True
            if command == 15:
                var = C.cast(data, C.POINTER(Variable)).contents
                var.value = variables.get(var.key)
                return var.value is not None
            if command == 16:
                entries = C.cast(data, C.POINTER(Variable))
                i = 0
                while entries[i].key:
                    default = entries[i].value.split(b'; ', 1)[-1].split(b'|')[0]
                    variables.setdefault(entries[i].key, default)
                    i += 1
                return True
            if command == 17:
                C.cast(data, C.POINTER(C.c_bool))[0] = False
                return True
            if command == 52:
                C.cast(data, C.POINTER(C.c_uint))[0] = 0
                return True
            return command in (11, 18, 32, 35, 36, 37, 44)

        @C.CFUNCTYPE(None, C.c_void_p, C.c_uint, C.c_uint, C.c_size_t)
        def video_refresh(data, width, height, pitch):
            if not data:
                return
            rows = []
            for y in range(height):
                raw = C.string_at(data + y * pitch, width * 4)
                rows.append(struct.unpack(f'<{width}I', raw))
            frame_holder[0] = rows

        callbacks = {
            'environment': environment,
            'video_refresh': video_refresh,
            'audio_sample': C.CFUNCTYPE(None, C.c_int16, C.c_int16)(lambda *_: None),
            'audio_sample_batch': C.CFUNCTYPE(C.c_size_t, C.c_void_p,
                                            C.c_size_t)(lambda _, count: count),
            'input_poll': C.CFUNCTYPE(None)(lambda: None),
            'input_state': C.CFUNCTYPE(C.c_int16, C.c_uint, C.c_uint, C.c_uint,
                                      C.c_uint)(lambda *_: 0),
        }
        for name, callback in callbacks.items():
            setter = getattr(lib, 'retro_set_' + name)
            setter.argtypes = [type(callback)]
            setter(callback)

        lib.retro_load_game.argtypes = [C.POINTER(GameInfo)]
        lib.retro_load_game.restype = C.c_bool
        lib.retro_init()
        try:
            rom = make_rom()
            rom_path = Path(directory) / 'greyscale.nes'
            rom_path.write_bytes(rom)
            buf = C.create_string_buffer(rom)
            info = GameInfo(str(rom_path).encode(),
                            C.cast(buf, C.c_void_p), len(rom), None)
            if not lib.retro_load_game(C.byref(info)):
                raise SystemExit('load failed')
            try:
                for phase in ('power', 'reset'):
                    if phase == 'reset':
                        lib.retro_reset()
                    for _ in range(6):
                        lib.retro_run()

                    rows = frame_holder[0]
                    assert rows and len(rows) >= 120
                    normal_px = rows[50][128] & 0x00FFFFFF
                    if is_neutral_grey(normal_px):
                        failures.append(
                            f'{phase}: baseline color 0x{normal_px:06X} is unexpectedly grey'
                        )
                        continue

                    hit_y = None
                    line = None
                    grey_xs = []

                    for y in range(80, 120):
                        candidate = [px & 0x00FFFFFF for px in rows[y]]
                        xs = [x for x, px in enumerate(candidate) if px != normal_px]
                        if xs:
                            hit_y = y
                            line = candidate
                            grey_xs = xs
                            break

                    if not grey_xs:
                        failures.append(
                            f'{phase}: no mid-scanline greyscale applied on lines 98..102'
                        )
                        continue

                    left_end = grey_xs[0]
                    right_start = grey_xs[-1] + 1
                    if left_end < 16 or right_start > 240:
                        failures.append(
                            f'{phase}: unexpected greyscale span [{left_end}, {right_start}) on line {hit_y}'
                        )

                    span = line[left_end:right_start]
                    if any(px == normal_px for px in span):
                        failures.append(f'{phase}: gap inside greyscale span on line {hit_y}')
                    if not all(is_neutral_grey(px) for px in span):
                        bad = next(px for px in span if not is_neutral_grey(px))
                        failures.append(
                            f'{phase}: non-grey pixel 0x{bad:06X} inside greyscale span on line {hit_y}'
                        )

                    print(
                        f'{phase}: line {hit_y} greyscale span [{left_end}, {right_start}) '
                        f'verified (0x{normal_px:06X} -> 0x{span[0]:06X})'
                    )
            finally:
                lib.retro_unload_game()
        finally:
            lib.retro_deinit()

    if failures:
        print("FAIL");
        raise SystemExit('\n'.join(failures))
    print('All mid-scanline greyscale checks passed.')
    print("PASS");


if __name__ == '__main__':
    main()
