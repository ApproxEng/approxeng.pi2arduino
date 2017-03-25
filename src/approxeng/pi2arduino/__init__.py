import struct

# noinspection PyUnresolvedReferences
import approxeng.pi2arduino._p2a as p2a_native


def send(address, *sequence):
    packed = ''
    for item in sequence:
        if isinstance(item, int) and 0 <= item <= 255:
            packed += struct.pack('B', item)
        elif isinstance(item, float):
            packed += struct.pack('f', item)
    p2a_native.sendBytes(address, packed)


def read(address, format_string):
    length = 0
    for c in format_string:
        if c == 'B':
            length += 1
        elif c == 'f':
            length += 4
    print p2a_native.getBytes(address, length)
