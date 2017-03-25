import struct

import approxeng.pi2arduino._p2a


def send(address, *sequence):
    packed = ""
    for item in sequence:
        if isinstance(item, int) and 0 <= item <= 255:
            packed += struct.pack("B", item)
        elif isinstance(item, float):
            packed += struct.pack("f", item)
    approxeng.pi2arduino._p2a.sendBytes(address, packed)
