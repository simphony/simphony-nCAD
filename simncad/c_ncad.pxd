from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp.map cimport map
from libcpp cimport bool 
ctypedef string ID_TYPE

cdef extern from "NCadSimphonyWrapper.h":
    cdef void delete_pointer(void *ptr)

cdef extern from "error_handlers.h":
    cdef int get_error_cpp()
    cdef const char* get_exception_what()

cdef inline int get_error_cython() except *:
    # we cannot get the thrown exception directly from cython so we have to
    # use a c defined function to do that for us.
    cdef const char* what = get_exception_what()
    # raise will set the python error and then the function will return
    # because of the `except *` cython will always check if there
    # is a python error after calling this function and make sure that
    # it is properly propagated.
    raise Exception("{}".format(what))


cdef extern from "NCadSimphonyWrapper.h":
    cdef cppclass CNCadSimphony:
        CNCadSimphony()
        CNCadSimphony(const char * project_name)
        void AddComponent(CNCadParticleContainer *Component, const char * cell_name,
                          CParticleContainerInfo *pc_info) except +get_error_cython
        void AddCell(CNCadParticleContainer *Cell, double a, double b, double c,
                            double alpha, double beta, double gamma,
                            int symmetry_gn) except +get_error_cython
        void CreateNanotubeCell(CNCadParticleContainer *Cell, vector[string] &cells, vector[double] n_values,
                                vector[double] m_values) except +get_error_cython
        void RemoveComponent(string &name) except +get_error_cython
        void RemoveCell(string &name) except +get_error_cython
        void TraceAll()
        void ShowComponent(string &name) except +get_error_cython
        void ShowCell(string &name) except +get_error_cython
        void ProcessAll()
        # CNCadParticleContainer * GetAssembly();
        void GetAssemblyAtoms(CNCadParticleContainer * res) except +get_error_cython
        void GetAssemblyBonds(CNCadParticleContainer * res) except +get_error_cython
        void BeginAssembly()
        void EndAssembly()
        CParticleInfo* GetAssemblyParticleInfo(long long int id) except +get_error_cython
        CBondInfo* GetAssemblyBondInfo(long long int id) except +get_error_cython
        void ProcessAssemblyParticle(CNCadParticle * pParticle, ID_TYPE Simphony_ID) except +get_error_cython 
        void ProcessAssemblyBond(CNCadBond * pBond, ID_TYPE Simphony_ID) except +get_error_cython
        void LoadSession(string session);

cdef extern from "NCadSimphonyWrapper.h":
    CNCadSimphony * pCNCadSimphony


cdef extern from "NCadSimphonyWrapper.h":
    cdef cppclass CNCadParticleContainer:
        CNCadParticleContainer() except +
        string name
        map[ID_TYPE, CNCadParticle*] particles
        map[long long unsigned int, ID_TYPE] particles_reverse_ids
        map[ID_TYPE, CNCadBond*] bonds
        void AddParticle(CNCadParticle *pParticle, ID_TYPE Simphony_ID) except +get_error_cython
        void AddParticle(CParticleInfo &partInfo) except +get_error_cython
        void AddBond(CNCadBond *bond, ID_TYPE Simphony_ID) except +get_error_cython
        void AddBond(CBondInfo &bondInfo) except +get_error_cython
        void UpdateParticle(CNCadParticle *pParticle, ID_TYPE Simphony_ID) except +get_error_cython
        void UpdateParticle(CParticleInfo &partInfo) except +get_error_cython
        void UpdateBond(CNCadBond *bond, ID_TYPE Simphony_ID) except +get_error_cython
        void UpdateBond(CBondInfo &bondInfo) except +get_error_cython
        CParticleInfo * GetParticleInfo(ID_TYPE ParticleID) except +get_error_cython
        CBondInfo * GetBondInfo(ID_TYPE BondID) except +get_error_cython
        void RemoveParticle(ID_TYPE particle_id) except +get_error_cython
        void RemoveBond(ID_TYPE bond_id) except +get_error_cython
        bool HasParticle(ID_TYPE ParticleID)
        bool HasBond(ID_TYPE BondID)
        int GetNParticles()
        int GetNBonds()
        CNCadParticleContainer * GetCopy()
        void Update(CParticleContainerInfo &pc_info) except +get_error_cython
        
cdef extern from "NCadSimphonyWrapper.h":
    cdef cppclass CNCadComponent(CNCadParticleContainer):
        CNCadComponent() except +
        
cdef extern from "NCadSimphonyWrapper.h":
    cdef cppclass CNCadCell(CNCadParticleContainer):
        CNCadCell() except +

cdef extern from "NCadSimphonyWrapper.h":
    cdef cppclass CNCadParticle:
        CNCadParticle() except +
        # ID_TYPE Simphony_ID
        long long int ID
        void SetParametersFull(double ax, double ay, double az,
                               const string& specie, const string&label)
        double GetX()
        double GetY()
        double GetZ()
        CNCadParticle * GetCopy()
        void Trace()

cdef extern from "NCadSimphonyWrapper.h":
    cdef cppclass CNCadBond:
        CNCadBond() except +
        # ID_TYPE Simphony_ID
        long long int ID
        ID_TYPE atom1
        ID_TYPE atom2
        void SetParametersFull(ID_TYPE particle1, ID_TYPE particle2)
        void Trace()

cdef extern from "NCadSimphonyWrapper.h":
    ctypedef struct CParticleInfo:
        ID_TYPE id
        double x
        double y
        double z
        string specie
        string label
        double occupancy
    
cdef extern from "NCadSimphonyWrapper.h":
    ctypedef struct CBondInfo:
        ID_TYPE id
        ID_TYPE atom1
        ID_TYPE atom2
    
cdef extern from "Factory_Shape.h":
    ctypedef struct CShapeInfo:
        int shape
        double centerX, centerY, centerZ
        double lengthX, lengthY, lengthZ
        int lengthXUC, lengthYUC, lengthZUC
        double radius
        double side
        int rotation_axis1, rotation_axis2
        double shape_rotation1[3]
        double shape_rotation2[3]
        string file_stl
        int mode
        float scaling
        float x_neg_padding
        float x_pos_padding
        float y_neg_padding
        float y_pos_padding
        float z_neg_padding
        float z_pos_padding

cdef extern from "NCadSimphonyWrapper.h":
    ctypedef struct CParticleContainerInfo:
        int symmetry_group
        double a, b, c
        double alpha, beta, gamma
        string name_uc
        double crystal_rotation11[3]
        double crystal_rotation12[3]
        double crystal_rotation21[3]
        double crystal_rotation22[3]
        CShapeInfo shape_info

