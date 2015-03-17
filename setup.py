from Cython.Distutils import build_ext
from setuptools import setup
from setuptools.extension import Extension

simphony_include_path = "./INCLUDE_SIMPHONY"
ncad_include_path = "./INCLUDE"

ext_modules = [Extension("ncad",
                        ["c_ncad.pxd", "ncad.pyx", "./src/error_handlers.cpp"],
                        include_dirs = [ncad_include_path, simphony_include_path, "./"],
                        language='c++',
                        extra_objects=["libNCad.dll"])]


setup(
  name = 'Ncad Wrapper',
  version = '0.1.1.0',
  author = 'SimPhoNy, EU FP7 Project (Nr. 604005) www.simphony-project.eu',
  description = 'NCad Adapter for SimPhoNy',
  install_requires = ['simphony', 'cython'],
  cmdclass = {'build_ext': build_ext},
  ext_modules = ext_modules,
  py_modules = ['auxiliar.celldata_parser', 'auxiliar.symmetry', 'ncad_types']
)
