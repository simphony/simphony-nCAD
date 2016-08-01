import os

from Cython.Distutils import build_ext
from setuptools import setup, find_packages
from setuptools.extension import Extension

simphony_include_path = "./simncad/INCLUDE_SIMPHONY"
ncad_include_path = "./simncad/INCLUDE"

with open('README.md', 'r') as readme:
    README_TEXT = readme.read()

VERSION = '0.2.1'

def write_version_py(filename=None):
    if filename is None:
        filename = os.path.join(
            os.path.dirname(__file__), 'simncad', 'version.py')
    ver = """\
version = '%s'
"""
    fh = open(filename, 'wb')
    try:
        fh.write(ver % VERSION)
    finally:
        fh.close()
write_version_py()

ext_modules = [Extension("simncad.ncad",
                        ["./simncad/c_ncad.pxd", "./simncad/ncad.pyx",
                         "./simncad/src/error_handlers.cpp"],
                        include_dirs = [ncad_include_path, simphony_include_path, "./simncad"],
                        language='c++',
                        extra_objects=["C:\NCad\libNCad.dll"])]


setup(
  name = 'simncad',
  version = VERSION,
  author = 'SimPhoNy, EU FP7 Project (Nr. 604005) www.simphony-project.eu',
  description = 'NCad Wrapper for SimPhoNy',
  long_description = README_TEXT,
  packages = find_packages(),
  install_requires = ['simphony >= 0.2.0', 'cython >= 0.21', 'numpy == 1.10.1'],
  entry_points = {'simphony.engine': [ 'ncad_wrapper = simncad.plugin']
                  },
  cmdclass = {'build_ext': build_ext},
  ext_modules = ext_modules
)
