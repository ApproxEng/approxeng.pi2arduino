from distutils.core import setup, Extension

extension_mod = Extension("_p2a", ["_p2a.c", "p2a.c", "pigpio.c", "command.c"],
                          extra_compile_args=['-Wno-strict-prototypes', '-Wno-error=format-security'])

setup(name="p2a", ext_modules=[extension_mod])
