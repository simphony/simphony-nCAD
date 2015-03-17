# simphony-nCAD
===============

This is the branch for the simphony-nCAD engine module.
The structure of the branch is the following:

    - INCLUDE: folder containing some C++ header files used by the C++ adapter and the API.
    - INCLUDE_SIMPHONY: folder containing the main C++ header files of the nCAD SimPhoNy adapter.
    - auxiliar: subpackage that contains a celldata parser (internal nCAD simple format for unit cells).
                Also, it contains the cuba.yml file and the generated cuba.py file using that yml that has the CUBA nedeed by the wrapper. This cuba
                file is not used directly by the wrapper; if anyone wants to use the wrapper he must replace the original cuba file of simphony-common
                with this file.
    - cd: folder for .cd examples.
    - src: cpp files of the code that is not inside the nCAD dll.
    - tests: folder for the tests created for the nCAD wrapper.
    - use_cases: folder to save the use cases python scripts.
    
    - c_ncad.pxd: cython header file for the C++ classes importation.
    - ncad.pyx: cython code main file.
    - ncad_types.py: enumerator definitions used by the nCAD module.
    - setup.py: the setup file of the package.
    
Note: the nCAD dll is not inside the branch.