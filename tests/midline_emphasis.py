#!/usr/bin/env python3
"""
Test mid-scanline $2001 emphasis rendering via the libretro core.
Fails when emphasis is sampled once per scanline; passes when applied per pixel.
"""

import ctypes
import os
import struct
import sys
import tempfile


RETRO_ENVIRONMENT_SET_PIXEL_FORMAT = 10
RETRO_PIXEL_FORMAT_XRGB8888 = 1


class RetroGameInfo(ctypes.Structure):
    _fields_ = [
        ("path", ctypes.c_char_p),
        ("data", ctypes.c_void_p),
        ("size", ctypes.c_size_t),
        ("meta", ctypes.c_char_p),
    ]


def build_test_rom():
    """
    Build a minimal NROM-128 (16 KiB PRG, 8 KiB CHR-RAM) iNES ROM.
    Sets $3F00 = $30 (white), then during active rendering toggles $2001
    emphasis bits mid-scanline: $1E (none) -> $3E (red) -> $FE (all).
    """
    prg = bytearray([0xEA] * 0x4000)

    def emit(offset, bytes_list):
        prg[offset : offset + len(bytes_list)] = bytearray(bytes_list)

    # Reset vector at $8000:
    #   SEI
    #   CLD
    #   LDX #$FF
    #   TXS
    #   LDA #$00
    #   STA $2000        ; disable NMI
    #   STA $2001        ; disable rendering
    # vwait1:
    #   BIT $2002
    #   BPL vwait1
    # vwait2:
    #   BIT $2002
    #   BPL vwait2
    #   LDA #$3F
    #   STA $2006
    #   LDA #$00
    #   STA $2006
    #   LDA #$30         ; light color so emphasis differences are high-contrast
    #   STA $2007
    #   LDA #$00
    #   STA $2006
    #   STA $2006
    #   LDA #$80         ; enable NMI
    #   STA $2000
    #   LDA #$1E         ; enable BG + sprites, no emphasis
    #   STA $2001
    # forever:
    #   JMP forever
    reset_code = [
        0x78,             # SEI
        0xD8,             # CLD
        0xA2, 0xFF,       # LDX #$FF
        0x9A,             # TXS
        0xA9, 0x00,       # LDA #$00
        0x8D, 0x00, 0x20, # STA $2000
        0x8D, 0x01, 0x20, # STA $2001
        0x2C, 0x02, 0x20, # BIT $2002
        0x10, 0xFB,       # BPL -5
        0x2C, 0x02, 0x20, # BIT $2002
        0x10, 0xFB,       # BPL -5
        0xA9, 0x3F,       # LDA #$3F
        0x8D, 0x06, 0x20, # STA $2006
        0xA9, 0x00,       # LDA #$00
        0x8D, 0x06, 0x20, # STA $2006
        0xA9, 0x30,       # LDA #$30
        0x8D, 0x07, 0x20, # STA $2007
        0xA9, 0x00,       # LDA #$00
        0x8D, 0x06, 0x20, # STA $2006
        0x8D, 0x06, 0x20, # STA $2006
        0xA9, 0x80,       # LDA #$80
        0x8D, 0x00, 0x20, # STA $2000
        0xA9, 0x1E,       # LDA #$1E
        0x8D, 0x01, 0x20, # STA $2001
        0x4C, 0x36, 0x80, # JMP $8036
    ]
    emit(0x0000, reset_code)

    # NMI handler at $8100:
    # Wait past VBlank + ~60 visible scanlines (~9,500 cycles), then run a loop
    # that switches $2001 emphasis three times within each scanline (113.67 cycles):
    #   Left:   $1E (emp = 0)
    #   Middle: $3E (emp = 1)
    #   Right:  $FE (emp = 7)
    nmi_code = [
        0xA9, 0x1E,       # LDA #$1E
        0x8D, 0x01, 0x20, # STA $2001
        0xA2, 0xC8,       # LDX #200 (~1000 cycles outer)
        0xA0, 0x09,       # LDY #9   (~45 cycles inner)
        0x88,             # DEY
        0xD0, 0xFD,       # BNE -3
        0xCA,             # DEX
        0xD0, 0xF8,       # BNE -8
        # Active scanline loop (64 lines, ~114 cycles per iteration)
        0xA2, 0x40,       # LDX #64
        # line_loop ($8111):
        0xA9, 0x1E,       # LDA #$1E        (2)
        0x8D, 0x01, 0x20, # STA $2001       (4) -> left span
        0xA0, 0x07,       # LDY #7          (2)
        0x88,             # DEY             (2*7 - 1 = 13)
        0xD0, 0xFD,       # BNE -3          (2*6 = 12) -> +27 cyc (~81 dots)
        0xA9, 0x3E,       # LDA #$3E        (2)
        0x8D, 0x01, 0x20, # STA $2001       (4) -> middle span (~dot 100)
        0xA0, 0x06,       # LDY #6          (2)
        0x88,             # DEY             (11)
        0xD0, 0xFD,       # BNE -3          (10) -> +23 cyc (~69 dots)
        0xA9, 0xFE,       # LDA #$FE        (2)
        0x8D, 0x01, 0x20, # STA $2001       (4) -> right span (~dot 185)
        0xA0, 0x06,       # LDY #6          (2)
        0x88,             # DEY             (11)
        0xD0, 0xFD,       # BNE -3          (10) -> +23 cyc (~69 dots)
        0xEA, 0xEA, 0xEA, # 3x NOP          (6)
        0xCA,             # DEX             (2)
        0xD0, 0xD4,       # BNE line_loop   (3) -> total ~114 cycles/line
        0xA9, 0x1E,       # LDA #$1E
        0x8D, 0x01, 0x20, # STA $2001
        0x40,             # RTI
    ]
    emit(0x0100, nmi_code)

    # Vectors at $FFFA-$FFFF (offset $3FFA-$3FFF in 16 KiB PRG)
    struct.pack_into("<HHH", prg, 0x3FFA, 0x8100, 0x8000, 0x8100)

    header = bytearray(b"NES\x1a") + bytearray([1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0])
    return bytes(header + prg)


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

    frame_buf = ctypes.create_string_buffer(256 * 240 * 4)
    frame_meta = {"width": 0, "height": 0, "pitch": 0}

    ENV_CB = ctypes.CFUNCTYPE(ctypes.c_bool, ctypes.c_uint, ctypes.c_void_p)
    VIDEO_CB = ctypes.CFUNCTYPE(
        None, ctypes.c_void_p, ctypes.c_uint, ctypes.c_uint, ctypes.c_size_t
    )
    AUDIO_CB = ctypes.CFUNCTYPE(None, ctypes.c_int16, ctypes.c_int16)
    AUDIO_BATCH_CB = ctypes.CFUNCTYPE(
        ctypes.c_size_t, ctypes.POINTER(ctypes.c_int16), ctypes.c_size_t
    )
    INPUT_POLL_CB = ctypes.CFUNCTYPE(None)
    INPUT_STATE_CB = ctypes.CFUNCTYPE(
        ctypes.c_int16, ctypes.c_uint, ctypes.c_uint, ctypes.c_uint, ctypes.c_uint
    )

    def on_env(cmd, data):
        if cmd == RETRO_ENVIRONMENT_SET_PIXEL_FORMAT and data:
            fmt = ctypes.cast(data, ctypes.POINTER(ctypes.c_int)).contents
            fmt.value = RETRO_PIXEL_FORMAT_XRGB8888
            return True
        return False

    def on_video(data, width, height, pitch):
        if not data:
            return
        frame_meta["width"] = width
        frame_meta["height"] = height
        frame_meta["pitch"] = pitch
        for y in range(height):
            src = ctypes.c_void_p(data + y * pitch)
            dst = ctypes.byref(frame_buf, y * 256 * 4)
            ctypes.memmove(dst, src, width * 4)

    def on_audio(left, right):
        pass

    def on_audio_batch(data, frames):
        return frames

    def on_input_poll():
        pass

    def on_input_state(port, device, index, id_):
        return 0

    cb_env = ENV_CB(on_env)
    cb_video = VIDEO_CB(on_video)
    cb_audio = AUDIO_CB(on_audio)
    cb_audio_batch = AUDIO_BATCH_CB(on_audio_batch)
    cb_input_poll = INPUT_POLL_CB(on_input_poll)
    cb_input_state = INPUT_STATE_CB(on_input_state)

    lib.retro_set_environment(cb_env)
    lib.retro_set_video_refresh(cb_video)
    lib.retro_set_audio_sample(cb_audio)
    lib.retro_set_audio_sample_batch(cb_audio_batch)
    lib.retro_set_input_poll(cb_input_poll)
    lib.retro_set_input_state(cb_input_state)

    lib.retro_init()

    rom_bytes = build_test_rom()
    with tempfile.NamedTemporaryFile(suffix=".nes", delete=False) as tf:
        tf.write(rom_bytes)
        rom_path = tf.name

    try:
        info = RetroGameInfo()
        info.path = rom_path.encode("utf-8")
        info.data = ctypes.cast(ctypes.c_char_p(rom_bytes), ctypes.c_void_p)
        info.size = len(rom_bytes)
        info.meta = None

        lib.retro_load_game.argtypes = [ctypes.POINTER(RetroGameInfo)]
        lib.retro_load_game.restype = ctypes.c_bool
        if not lib.retro_load_game(ctypes.byref(info)):
            raise RuntimeError("retro_load_game failed")

        for _ in range(6):
            lib.retro_run()

        pixels = struct.unpack("<" + "I" * (256 * 240), frame_buf.raw)

        # Find a scanline in the split region (around y = 75..110) and verify
        # left (x=16), middle (x=128), and right (x=224) have 3 distinct colors.
        found_split_line = False
        for y in range(100, 120):
            row = pixels[y * 256 : (y + 1) * 256]

            c_left = row[16] & 0x00FFFFFF
            c_mid = row[128] & 0x00FFFFFF
            c_right = row[224] & 0x00FFFFFF

            # Preserve left-to-right order of unique colors on this scanline
            unique_colors = list(dict.fromkeys(p & 0x00FFFFFF for p in row))
            hex_list = [f"#{c:06X}" for c in unique_colors]
    
            if len({c_left, c_mid, c_right}) == 3:
                found_split_line = True
                break

        assert found_split_line, (
            "Expected 3 distinct emphasis colors across a single scanline, "
            "but found uniform/non-split scanlines"
            print("FAIL")
        )
        print("PASS: mid-scanline $2001 emphasis rendered distinct spans")
        print("PASS")
    finally:
        lib.retro_unload_game()
        lib.retro_deinit()
        if os.path.exists(rom_path):
            os.remove(rom_path)


if __name__ == "__main__":
    main()
