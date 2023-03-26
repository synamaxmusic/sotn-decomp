from segtypes.n64.ci4 import N64SegCi4


class PSXSegCi4(N64SegCi4):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    def flip_endian(self, src):
        dst = bytearray(src)
        for i in range(0, len(dst)):
            ch = dst[i]
            dst[i] = (ch >> 4) | ((ch & 0xF) << 4)
        return dst

    def split(self, rom_bytes):
        super().split(self.flip_endian(rom_bytes))
