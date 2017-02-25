import struct

import approxeng.pi2arduino._p2a


def send(address, *args):
    bytes = []
    for arg in args:
        if isinstance(arg, int):
            if arg < 0 | arg > 255:
                raise ValueError('Integer values must be between 0 and 255!')
            else:
                bytes.append(arg)
        elif isinstance(arg, float):
            for c in struct.pack('F', arg):
                bytes.append(c)
    print ''.join(bytes)
    approxeng.pi2arduino._p2a.sendBytes(address, ''.join(bytes))
