# I2CHelper library

This library handles communication from the Arduino end under conditions where errors are likely. In
general the I2C bus is reliable, but when combined with some libraries which disable interrupts, or
in cases where it's talking to a device like the Pi which doesn't handle clock skew particularly well,
there may be a need for some extra help. In addition, this library supports passing float and other
kinds of data, managing checksums etc.

To use, either point your IDE at this directory and tell it to import a library, or symlink this 
directory into ~/Arduino/libraries/I2CHelper. The I2CHelper.ino is just there to make it easy to open
the library files in the Arduino IDE and doesn't really actually do anything.
