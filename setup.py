import os

from Cython.Distutils import build_ext
from setuptools import setup, find_packages
from setuptools.extension import Extension

simphony_include_path = "./ncad/INCLUDE_SIMPHONY"
ncad_include_path = "./ncad/INCLUDE"

with open('README.md', 'r') as readme:
    README_TEXT = readme.read()

VERSION = '0.1.1.dev0'

def write_version_py(filename=None):
    if filename is None:
        filename = os.path.join(
            os.path.dirname(__file__), 'ncad', 'version.py')
    ver = """\
version = '%s'
"""
    fh = open(filename, 'wb')
    try:
        fh.write(ver % VERSION)
    finally:
        fh.close()
write_version_py()

ext_modules = [Extension("ncad.simncad",
                        ["./ncad/c_ncad.pxd", "./ncad/ncad.pyx",
                         "./ncad/src/error_handlers.cpp"],
                        include_dirs = [ncad_include_path, simphony_include_path, "./ncad"],
                        language='c++',
                        extra_objects=["libNCad.dll"])]


setup(
  name = 'ncad wrapper',
  version = VERSION,
  author = 'SimPhoNy, EU FP7 Project (Nr. 604005) www.simphony-project.eu',
  description = 'NCad Adapter for SimPhoNy',
  long_description = README_TEXT,
  # packages = find_packages(),
  packages = ['ncad.auxiliar'],
  install_requires = ['simphony', 'cython'],
  cmdclass = {'build_ext': build_ext},
  ext_modules = ext_modules
)
