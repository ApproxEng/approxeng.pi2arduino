from setuptools import setup, Extension, find_packages

extension_mod = Extension("approxeng.pi2arduino.p2a",
                          sources=["linux/_p2a.c",
                                   "linux/p2a.c",
                                   "linux/pigpio.c",
                                   "linux/command.c"],
                          extra_compile_args=['-Wno-strict-prototypes',
                                              '-Wno-error=format-security'])

setup(name="approxeng.pi2arduino",
      version='0.1',
      description='Python support for Pi to Arduino bi-directional communication over I2C',
      classifiers=['Programming Language :: Python :: 2.7'],
      url='https://github.com/ApproxEng/approxeng.pi2arduino/',
      author='Tom Oinn',
      author_email='tomoinn@gmail.com',
      license='ASL2.0',
      packages=find_packages(),
      ext_modules=[extension_mod])
