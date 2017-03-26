import struct

import approxeng.pi2arduino._p2a


class I2CHelper(object):
    """
    Class to handle communication via the p2a.c native code. Packages and unpackages sequences of
    numeric data and initialises the I2C device on construction.
    """

    def __init__(self):
        """
        Initialise - this will require root access after which it can be dropped.
        """

        approxeng.pi2arduino._p2a.init()

    def send(self, address, *sequence):
        """
        Send a message consisting of a sequence of integers (between 0 and 255) and floats to 
        a device on the I2C bus
        
        :param address: 
            The I2C address of the receiver
        :param sequence: 
            An arbitrary number of ints and floats to send
        """
        packed = ''
        for item in sequence:
            if isinstance(item, int) and 0 <= item <= 255:
                packed += struct.pack('B', item)
            elif isinstance(item, float):
                packed += struct.pack('f', item)
        approxeng.pi2arduino._p2a.sendBytes(address, packed)

    def read(self, address, format_string):
        """
        Read data from a device on the I2C bus
        
        :param address: 
            The I2C address of the node to query
        :param format_string: 
            A format string, currently only supporting 'f' for floats and 'B' for byte values, this
            is used both to determine the size of the request, and then to unpack the request into
            a sequence of numeric values of the specified type or types
        :return: 
            A sequence of numeric data from the I2C device
        """
        length = 0
        for token in format_string:
            if token == 'B':
                length += 1
            elif token == 'f':
                length += 4
        data = approxeng.pi2arduino._p2a.getBytes(address, length)
        return struct.unpack(format_string, data)
