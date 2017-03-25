import struct

import approxeng.pi2arduino._p2a


class I2CHelper(object):
    def __init__(self):
        approxeng.pi2arduino._p2a.init()

    def send(self, address, *sequence):
        packed = ''
        for item in sequence:
            if isinstance(item, int) and 0 <= item <= 255:
                packed += struct.pack('B', item)
            elif isinstance(item, float):
                packed += struct.pack('f', item)
        approxeng.pi2arduino._p2a.sendBytes(address, packed)

    def read(self, address, format_string):
        length = 0
        for token in format_string:
            if token == 'B':
                length += 1
            elif token == 'f':
                length += 4
        data = approxeng.pi2arduino._p2a.getBytes(address, length)
        print data
