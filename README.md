# approxeng.pi2arduino

Bi-directional communication over I2C between a Raspberry Pi and Arduino, where the Pi acts as the master.

Normally I2C should be easy enough, but under some conditions (most particularly with the Arduino running timing
critical code such as the FastLED library) you'll get errors with the raw SMBus transactions. These libraries, one for
the Arduino and one for Python on the Pi, handle multi-byte raw transfers by sending a byte at a time, along with a
checksum which is verified before the Arduino processes the request. Failed individual sends are retried from the Pi
side of the system; this doesn't eliminate the potential for all sends to succeed but still to have a confused message
on the Arduino but the checksum at least means you can safely ignore the invalid data. It's not perfect as there's no
way for the Arduino to directly notify that it's failed a checksum, but you could build that on top of what there is
here if that was necessary.

The Python API is very simple, allowing you to send a sequence of either byte (integer between 0 and 255) or floating
point values to a particular device address, or to request a number of bytes from an address. The Arduino API is also
very simple, wrapping the appropriate calls to the Wire library and allowing your code to pull the same data types as
used on the Python side out of the request message.