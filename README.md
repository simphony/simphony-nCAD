# simphony-nCAD
===============

Structure of package
--------------------

This is the branch for the simphony-nCAD engine module.
The structure of the branch is the following:

    - cd: folder for .cd examples.
    - use_cases: folder to save the use cases python scripts.
    - simncad: main package
        - c_ncad.pxd: cython header file for the C++ classes importation.
        - ncad.pyx: cython code main file.
        - ncad.pyd: the generated python dynamic library with setup.py.
        - ncad_types.py: enumerator definitions used by the nCAD module.
        - src: cpp files of the code that is not inside the nCAD dll.
        - tests: folder for the tests created for the nCAD wrapper.
        - INCLUDE: folder containing some C++ header files used by the C++ adapter and the API.
        - INCLUDE_SIMPHONY: folder containing the main C++ header files of the nCAD SimPhoNy adapter.
        - auxiliar: subpackage that contains a celldata parser (internal nCAD simple format for unit cells).
                Also, it contains the cuba.yml file and the generated cuba.py file using that yml that has the CUBA nedeed by the wrapper.
                This file is not used to satisfy the current simphony common version.
    - setup.py: the setup file of the package.

Prerrequisites
--------------
    
    The following files are needed for SimPhoNY-nCad to be installed:
    
        - Python 2.7
        - Cython >= 0.22.1
        - Symphony-common library 0.1.1
        - MinGW gcc and g++ 4.6.2 (http://sourceforge.net/projects/mingw/files/OldFiles/mingw-get-inst/mingw-get-inst-20111118/)
        - nCad dynamic library (not included)
        
    And these are the files that nCad needs to work:
        
        - glew dynamic library 1.9.0 (http://glew.sourceforge.net/)
        - Qt C++ for mingw 4.8.1 (http://download.qt.io/archive/qt/4.8/4.8.1/qt-win-opensource-4.8.1-mingw.exe)
        - OpenGL > 3.0 drivers
        
        For the nCad and glew libraries setup, the user must:
            1. Create a C:/NCad folder
            2. Place the libNCad.dll and glew32.dll in C:/NCad
            3. Register the C:/NCad path in the PATH enviroment variable

Installation
------------

    To build the extension library and install the whole package::
    
        python setup.py install
    
Usage
-----

    ncad library can be imported from the simphony.engine module::
    
        from simphony.engine import ncad_wrapper
    
    To run the tests::
    
        python -m unittest discover
    
Documentation
-------------

    This documentation has been generated using sphinx. There is a Makefile for Unix systems
    and a make.bat file for Windows systems to build automatically this documentation::
    
        make <target>
        
    where target is the desired format (e.g.: html, txt..)

    
    
Note: the nCAD dll is not inside the branch.