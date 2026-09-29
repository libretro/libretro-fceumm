#!/usr/bin/env python3
"""Test OneBus CHR mapping with generated NES 2.0 images (no external ROMs).

Usage: python3 tests/onebus_chr.py /path/to/fceumm_libretro.so
The same command accepts a .dylib or .dll on its native platform.
"""

import ctypes as C
from pathlib import Path
import sys
import tempfile


class Variable(C.Structure):
    _fields_ = [('key', C.c_char_p), ('value', C.c_char_p)]


class GameInfo(C.Structure):
    _fields_ = [('path', C.c_char_p), ('data', C.c_void_p),
                ('size', C.c_size_t), ('meta', C.c_char_p)]


def make_rom(mapper, submapper, separate_chr):
    # Distinct markers in every 1 KiB bank reveal which backing store is read.
    prg = bytearray().join(bytes([0x10 + bank]) * 1024 for bank in range(32))
    chr_rom = b''.join(bytes([0x80 + bank]) * 1024 for bank in range(8))
    code = bytearray.fromhex('78 d8 a2 ff 9a')  # SEI; CLD; LDX #$ff; TXS

    def write(address, value):
        code.extend([0xa9, value, 0x8d, address & 255, address >> 8])

    write(0x2000, 0)
    write(0x2001, 0)
    # Wait for two vblanks before writing PPU registers.
    code.extend(bytes.fromhex('2c 02 20 2c 02 20 10 fb 2c 02 20 10 fb'))

    def read_ppu(address, result):
        code.extend(bytes.fromhex('2c 02 20'))  # reset address latch
        write(0x2006, address >> 8)
        write(0x2006, address & 255)
        code.extend(bytes.fromhex('ad 07 20 ad 07 20'))  # discard buffered read
        code.extend([0x8d, result, 0x06])

    read_ppu(0x0000, 0)
    read_ppu(0x0400, 1)
    write(0x2016, 2)  # switch the first 2 KiB CHR bank
    read_ppu(0x0000, 2)
    read_ppu(0x0400, 3)
    write(0x0604, 0xa5)  # completion marker
    loop = 0xe000 + len(code)
    code.extend([0x4c, loop & 255, loop >> 8])
    prg[-8192:-8192 + len(code)] = code
    prg[-6:] = bytes.fromhex('00 e0 00 e0 00 e0')
    header = bytearray(b'NES\x1a' + bytes(12))
    header[4:9] = bytes([2, int(separate_chr), (mapper & 15) << 4,
                         (mapper & 0xf0) | 8, (submapper << 4) | (mapper >> 8)])
    return bytes(header + prg) + (chr_rom if separate_chr else b'')


def run(core_path):
    lib = C.CDLL(str(Path(core_path).resolve()))
    variables = {b'fceumm_ramstate': b'fill $00'}
    failures = []
    with tempfile.TemporaryDirectory(prefix='onebus-chr-') as directory:
        encoded_directory = directory.encode()

        @C.CFUNCTYPE(C.c_bool, C.c_uint, C.c_void_p)
        def environment(command, data):
            if command in (9, 30, 31):  # system, assets, save directories
                C.cast(data, C.POINTER(C.c_char_p))[0] = encoded_directory
                return True
            if command == 3:  # GET_CAN_DUPE
                C.cast(data, C.POINTER(C.c_bool))[0] = True
                return True
            if command == 15:  # GET_VARIABLE
                var = C.cast(data, C.POINTER(Variable)).contents
                var.value = variables.get(var.key)
                return var.value is not None
            if command == 16:  # SET_VARIABLES (legacy options)
                entries = C.cast(data, C.POINTER(Variable))
                i = 0
                while entries[i].key:
                    default = entries[i].value.split(b'; ', 1)[-1].split(b'|')[0]
                    variables.setdefault(entries[i].key, default)
                    i += 1
                return True
            if command == 17:  # GET_VARIABLE_UPDATE
                C.cast(data, C.POINTER(C.c_bool))[0] = False
                return True
            if command == 52:  # GET_CORE_OPTIONS_VERSION
                C.cast(data, C.POINTER(C.c_uint))[0] = 0
                return True
            return command in (10, 11, 18, 32, 35, 36, 37, 44)

        callbacks = {
            'environment': environment,
            'video_refresh': C.CFUNCTYPE(None, C.c_void_p, C.c_uint, C.c_uint,
                                        C.c_size_t)(lambda *_: None),
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
        lib.retro_get_memory_data.argtypes = [C.c_uint]
        lib.retro_get_memory_data.restype = C.c_void_p
        lib.retro_get_memory_size.argtypes = [C.c_uint]
        lib.retro_get_memory_size.restype = C.c_size_t
        lib.retro_init()
        try:
            # Also exercise the other mappers sharing UNLOneBusPower.
            for mapper, submapper in [(256, 0), (256, 2), (270, 0), (408, 0), (436, 0)]:
                for separate_chr in (False, True):
                    label = f'{mapper}.{submapper} ' + ('CHR-ROM' if separate_chr else 'PRG-backed CHR')
                    rom = make_rom(mapper, submapper, separate_chr)
                    (Path(directory) / 'test.nes').write_bytes(rom)
                    buf = C.create_string_buffer(rom)
                    info = GameInfo(str(Path(directory) / 'test.nes').encode(),
                                    C.cast(buf, C.c_void_p), len(rom), None)
                    if not lib.retro_load_game(C.byref(info)):
                        failures.append(label + ': load failed')
                        continue
                    try:
                        base = 0x80 if separate_chr else 0x10
                        expected = bytes([base, base + 1, base + 2, base + 3, 0xa5])
                        for phase in ('power', 'reset'):
                            if phase == 'reset':
                                ram = lib.retro_get_memory_data(2)
                                C.memset(ram + 0x600, 0, 5)
                                lib.retro_reset()
                            for _ in range(10):
                                lib.retro_run()
                            ram = lib.retro_get_memory_data(2)  # RETRO_MEMORY_SYSTEM_RAM
                            assert ram and lib.retro_get_memory_size(2) >= 0x605
                            actual = C.string_at(ram + 0x600, 5)
                            if actual != expected:
                                failures.append(f'{label} {phase}: {actual.hex()} != {expected.hex()}')
                        print(label + ': checked power and reset')
                    finally:
                        lib.retro_unload_game()
        finally:
            lib.retro_deinit()
    if failures:
        raise SystemExit('\n'.join(failures))
    print('All OneBus CHR mapping checks passed.')


if __name__ == '__main__':
    if len(sys.argv) != 2:
        raise SystemExit(__doc__)
    run(sys.argv[1])
