from distutils.core import setup, Extension

extension_mod = Extension("approxeng.pi2arduino._p2a", sources=["linux/_p2a.c", "linux/p2a.c", "linux/pigpio.c", "linux/command.c"],
                          extra_compile_args=['-Wno-strict-prototypes', '-Wno-error=format-security'])

setup(name="p2a", ext_modules=[extension_mod])
