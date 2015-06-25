#ifndef __NCAD_SIMPHONY_WRAPPER__H__
#define __NCAD_SIMPHONY_WRAPPER__H__

/**@pkg _SIMPHONY_ADAPTER*/

#include <vector>
#include <map>
#include <iostream>
#include <string>
#include "WRAPPER/NC_Wrapper.h"
#include "VisualizerSimphony.h"
#include "Factory_Shape.h"
using namespace std;

/**The Simphony ID's type (currently string representation of the UUIDs.*/
typedef string ID_TYPE;

/**Deletes a generic pointer (using this sometimes due to problems with pointers to structs).*/
void delete_pointer(void * ptr);

/**Struct type to pass information of particles from Cython side to C++ side of Simphony adapter.*/
typedef struct {
    /**Simphony id of the atom.*/
    ID_TYPE id;
    /**x coordinate of the atom.*/
    double x;
    /**y coordinate of the atom.*/
    double y;
    /**z coordinate of the atom.*/
    double z;
    /**Chemical specie of the atom.*/
    string specie;
    /**Label of the atom.*/
    string label;
    /**Occupancy of the atom.*/
    double occupancy;
} CParticleInfo;

/**Struct type to pass information of bonds from Cython side to C++ side of Simphony adapter.*/
typedef struct {
    /**Simphony id of the bond.*/
    ID_TYPE id;
    /**Simphony id of the first atom.*/
    ID_TYPE atom1;
    /**Simphony id of the second bond.*/
    ID_TYPE atom2;
} CBondInfo;

/**Struct type to pass information of particle containers from Cython side to C++ side of Simphony adapter.*/
typedef struct pc_info{
    //For cell ------------------
    /**Symmetry group of the cell.*/
    int symmetry_group;
    /**a length of the cell.*/
    double a;
    /**b length of the cell.*/
    double b;
    /**c length of the cell.*/
    double c;
    /**alpha angle of the cell.*/
    double alpha;
    /**beta angle of the cell.*/
    double beta;
    /**gamma angle of the cell.*/
    double gamma;
    // --------------------------
    //For component -------------
    /**Name of the attached unit cell to the component.*/
    string name_uc;
    /**Miller index of the first rotation of the crystal component.*/
    double crystal_rotation11[3];
    /**second vector of the first rotation of the crystal component.*/
    double crystal_rotation12[3];
    /**first vector of the second rotation of the crystal component.*/
    double crystal_rotation21[3];
    /**second vector of the second rotation of the crystal component.*/
    double crystal_rotation22[3];
    /**Shape info of the component.*/
    CShapeInfo shape_info;
    // --------------------------
    pc_info();
} CParticleContainerInfo;

class CNCadParticle
/**Class that represents a single particle / atom in the particle container.*/
{
public:
    /**Internal ID of the particle.*/
    id_t ID;
    /**Constructor.*/
    CNCadParticle();
    /**Method to get a full independent copy of the particle.*/
    CNCadParticle * GetCopy();
};

class CNCadBond
/**Class that represents a single bond / atom in the bond container.*/
{
public:
    /**Internal ID of the bond.*/
    id_t ID;
    /**Simphony ID of the first particle of the bond.*/
    ID_TYPE atom1;
    /**Simphony ID of the second particle of the bond.*/
    ID_TYPE atom2;
    /**Constructor.*/
    CNCadBond();
    /**Constructor given an internal ID.*/
    CNCadBond(ID_TYPE aID);
    /**Method to get a full independent copy of the bond.*/
    CNCadBond * GetCopy();
};

class CNCadParticleContainer
/**Class that represents the particle container entity inside ncad for
   Simphony usage.*/
{
public:
    /**Name of the particle container (unique within nCad adapter).*/
    string name;

    /**map that keeps the Simphony_ID ---> Internal_ID correspondance for particles.*/
    map<ID_TYPE, CNCadParticle*> particles;
    /**map that keeps the Simphony_ID ---> Internal_ID correspondance for bonds.*/
    map<ID_TYPE, CNCadBond*> bonds;

    /*This maps are implemented temporary to make faster the operation of retrieving bonds
    from nCad when processing.*/
    map<id_t, ID_TYPE> particles_reverse_ids;
    // map<id_t, ID_TYPE> bonds_reverse_ids;
    
    /**Constructor.*/
    CNCadParticleContainer();
    /**Destructor.*/
    virtual ~CNCadParticleContainer();
    
    /**Adds a particle to the particles map.
    @param pParticle particle pointer to add.    */
    virtual void AddParticle(CNCadParticle *pParticle, ID_TYPE Simphony_ID);
    /**Adds a particle to the particles map.
    @param partInfo particle to add.    */
    virtual void AddParticle(CParticleInfo &partInfo);
    /**Adds a bond to the particles map.
    @param pBond bond pointer to add.    */
    virtual void AddBond(CNCadBond *pBond, ID_TYPE Simphony_ID);
    /**Adds a bond to the particles map.
    @param bondInfo bond pointer to add.    */
    virtual void AddBond(CBondInfo &bondInfo);
    
    /**Updates a particle inside the particle container.
    @param pParticle particle to replace.*/
    virtual void UpdateParticle(CNCadParticle *pParticle, ID_TYPE Simphony_ID);
    /**Updates a particle inside the particle container.
    @param partInfo particle to replace.*/
    virtual void UpdateParticle(CParticleInfo &partInfo);
    /**Updates a bond inside the particle container.
    @param pBond bond to replace.*/
    virtual void UpdateBond(CNCadBond *pBond, ID_TYPE Simphony_ID);
    /**Updates a bond inside the particle container.
    @param bondInfo bond to replace.*/
    virtual void UpdateBond(CBondInfo &bondInfo);
    
    /**Deletes a particle from the particle container.
    @param ParticleID the simphony id of the particle to erase.*/
    virtual void RemoveParticle(ID_TYPE ParticleID);
    /**Deletes a bond from the particle container.
    @param BondID the simphony id of the bond to erase.*/
    virtual void RemoveBond(ID_TYPE BondID);

    /**Obtains the particle with the corresponding ParticleID.
    @param ParticleID the simphony ID of the particle to retrieve.
    @returns the CNCadParticle pointer of the particle.*/
    virtual CNCadParticle * GetParticle(ID_TYPE ParticleID);
    /**Obtains the bond with the corresponding BondID.
    @param BondID the simphony ID of the bond to retrieve.
    @returns the CNCadParticle pointer of the bond.*/
    virtual CNCadBond * GetBond(ID_TYPE BondID);
    
    /**Method to retrieve the information of the specified particle from nCad.
    @param ParticleID the id of the particle.
    @returns CParticleInfo pointer with the information.*/
    virtual CParticleInfo * GetParticleInfo(ID_TYPE ParticleID){ return NULL;}
    /**Method to retrieve the information of the specified particle from nCad.
    @param ParticleID the id of the particle.
    @returns CParticleInfo pointer with the information.*/
    virtual CBondInfo * GetBondInfo(ID_TYPE BondID){ return NULL; }
    
    /**Gets the Simphony ID using linear search of the particle with the given internal id.
    @param id the internal id of the particle.
    @returns the Simphony ID of the particle.*/
    virtual ID_TYPE GetParticleID(id_t id);
    virtual ID_TYPE GetParticleIDByInternalID(id_t ID);
    virtual ID_TYPE GetBondIDByInternalID(id_t ID){}
    
    /**Indicates whether a particle exists in the container.
    @param ParticleID particle id to check.
    @returns True if the particle is inside, False otherwise.*/
    virtual bool HasParticle(ID_TYPE ParticleID);
    /**Indicates whether a particle exists in the container.
    @param BondID bond id to check.
    @returns True if the bond is inside, False otherwise.*/
    virtual bool HasBond(ID_TYPE BondID);

    /**Creates a full copy of the particle container.
    @returns a new allocated CNCadParticleContainer pointer.*/
    virtual CNCadParticleContainer * GetCopy();

    /**Updates de information of the particle container inside nCad.
    @param pc_info the new information of the particle container.*/
    void Update(CParticleContainerInfo &pc_info);
    
    /**Clears all the particle container.*/
    void ClearAll();
};


class CNCadComponent : public CNCadParticleContainer
/**Class that represents the component entity inside ncad for
   Simphony usage.*/
{
public:
    /**Pointer to a NC_Component, holding the component information for further usage.*/
    NC_Component *pComponent;
    /**Constructor.*/
    CNCadComponent();
    /**Destructor.*/
    ~CNCadComponent(){}
    
    /**Adds a particle to the particles map.
    @param pParticle particle pointer to add.*/
    void AddParticle(CNCadParticle *pParticle, ID_TYPE Simphony_ID);
    /**Adds a particle to the particles map.
    @param partInfo particle to add.*/
    void AddParticle(CParticleInfo &partInfo);
    /**Adds a bond to the particles map.
    @param pBond bond pointer to add.*/
    void AddBond(CNCadBond *pBond, ID_TYPE Simphony_ID);
    /**Adds a bond to the particles map.
    @param bondInfo bond pointer to add.*/
    void AddBond(CBondInfo &bondInfo);
    
    /**Updates a particle inside the particle container.
    @param pParticle particle to replace.*/
    void UpdateParticle(CNCadParticle *pParticle, ID_TYPE Simphony_ID);
    /**Updates a particle inside the particle container.
    @param partInfo particle to replace.*/
    void UpdateParticle(CParticleInfo &partInfo);
    /**Updates a bond inside the particle container.
    @param pBond bond to replace.*/
    void UpdateBond(CNCadBond *pBond, ID_TYPE Simphony_ID);
    /**Updates a bond inside the particle container.
    @param bondInfo bond to replace.*/
    void UpdateBond(CBondInfo &bondInfo);
    
    /**Deletes a particle from the particle container.
    @param ParticleID the simphony id of the particle to erase.*/
    void RemoveParticle(ID_TYPE ParticleID);
    /**Deletes a bond from the particle container.
    @param BondID the simphony id of the bond to erase.*/
    void RemoveBond(ID_TYPE BondID);

    /**Obtains the particle with the corresponding ParticleID.
    @param ParticleID the simphony ID of the particle to retrieve.
    @returns the CNCadParticle pointer of the particle.*/
    CNCadParticle * GetParticle(ID_TYPE ParticleID);
    /**Obtains the bond with the corresponding BondID.
    @param BondID the simphony ID of the bond to retrieve.
    @returns the CNCadParticle pointer of the bond.*/
    CNCadBond * GetBond(ID_TYPE BondID);
    
    /**Method to retrieve the information of the specified particle from nCad.
    @param ParticleID the id of the particle.
    @returns CParticleInfo pointer with the information.*/
    CParticleInfo * GetParticleInfo(ID_TYPE ParticleID);
    /**Method to retrieve the information of the specified particle from nCad.
    @param ParticleID the id of the particle.
    @returns CParticleInfo pointer with the information.*/
    CBondInfo * GetBondInfo(ID_TYPE BondID);
    
    /**Gets the Simphony ID using linear search of the particle with the given internal id.
    @param id the internal id of the particle.
    @returns the Simphony ID of the particle.*/
    ID_TYPE GetParticleID(id_t id);
    
    /**Indicates whether a particle exists in the container.
    @param ParticleID particle id to check.
    @returns True if the particle is inside, False otherwise.*/
    bool HasParticle(ID_TYPE ParticleID);
    /**Indicates whether a particle exists in the container.
    @param BondID bond id to check.
    @returns True if the bond is inside, False otherwise.*/
    bool HasBond(ID_TYPE BondID);

    /**Creates a full copy of the particle container.
    @returns a new allocated CNCadParticleContainer pointer.*/
    CNCadComponent * GetCopy();
    
    /**Updates de information of the particle container inside nCad.
    @param pc_info the new information of the particle container.*/
    void Update(CParticleContainerInfo &pc_info);
};

class CNCadCell : public CNCadParticleContainer
/**Class that represents the cell entity inside ncad for
   Simphony usage.*/
{
public:
    /**Pointer to a NC_Component, holding the component information for further usage.*/
    NC_Cell *pCell;
    /**Constructor.*/
    CNCadCell();
    /**Destructor.*/
    ~CNCadCell(){}
    
    /**Adds a particle to the particles map.
    @param pParticle particle pointer to add.*/
    void AddParticle(CNCadParticle *pParticle, ID_TYPE Simphony_ID){}
    /**Adds a particle to the particles map.
    @param partInfo particle to add.*/
    void AddParticle(CParticleInfo &partInfo);
    /**Adds a bond to the particles map.
    @param pBond bond pointer to add.*/
    void AddBond(CNCadBond *pBond, ID_TYPE Simphony_ID);
    /**Adds a bond to the particles map.
    @param bondInfo bond pointer to add.*/
    void AddBond(CBondInfo &bondInfo);
    
    /**Updates a particle inside the particle container.
    @param pParticle particle to replace.*/
    void UpdateParticle(CNCadParticle *pParticle, ID_TYPE Simphony_ID){}
    /**Updates a particle inside the particle container.
    @param partInfo particle to replace.*/
    void UpdateParticle(CParticleInfo &partInfo);
    /**Updates a bond inside the particle container.
    @param pBond bond to replace.*/
    void UpdateBond(CNCadBond *pBond, ID_TYPE Simphony_ID);
    /**Updates a bond inside the particle container.
    @param bondInfo bond to replace.*/
    void UpdateBond(CBondInfo &bondInfo);
    
    /**Deletes a particle from the particle container.
    @param ParticleID the simphony id of the particle to erase.*/
    void RemoveParticle(ID_TYPE ParticleID);
    /**Deletes a bond from the particle container.
    @param BondID the simphony id of the bond to erase.*/
    void RemoveBond(ID_TYPE BondID);

    /**Obtains the particle with the corresponding ParticleID.
    @param ParticleID the simphony ID of the particle to retrieve.
    @returns the CNCadParticle pointer of the particle.*/
    CNCadParticle * GetParticle(ID_TYPE ParticleID);
    /**Obtains the bond with the corresponding BondID.
    @param BondID the simphony ID of the bond to retrieve.
    @returns the CNCadParticle pointer of the bond.*/
    CNCadBond * GetBond(ID_TYPE BondID);
    
    /**Method to retrieve the information of the specified particle from nCad.
    @param ParticleID the id of the particle.
    @returns CParticleInfo pointer with the information.*/
    CParticleInfo * GetParticleInfo(ID_TYPE ParticleID);
    /**Method to retrieve the information of the specified particle from nCad.
    @param ParticleID the id of the particle.
    @returns CParticleInfo pointer with the information.*/
    CBondInfo * GetBondInfo(ID_TYPE BondID);
    
    /**Gets the Simphony ID using linear search of the particle with the given internal id.
    @param id the internal id of the particle.
    @returns the Simphony ID of the particle.*/
    ID_TYPE GetParticleID(id_t id);
    
    /**Indicates whether a particle exists in the container.
    @param ParticleID particle id to check.
    @returns True if the particle is inside, False otherwise.*/
    bool HasParticle(ID_TYPE ParticleID);
    /**Indicates whether a particle exists in the container.
    @param BondID bond id to check.
    @returns True if the bond is inside, False otherwise.*/
    bool HasBond(ID_TYPE BondID);

    /**Creates a full copy of the particle container.
    @returns a new allocated CNCadParticleContainer pointer.*/
    CNCadCell * GetCopy();
    
    /**Updates de information of the particle container inside nCad.
    @param pc_info the new information of the particle container.*/
    void Update(CParticleContainerInfo &pc_info);
};

class CNCadSimphony
/**The wrapper class to interact with NCad API from Simphony.*/
{
private:
    /**The unique instance of the API wrapper.*/
    static NC_Wrapper *pWP;
    
    /**Map containing the cells inside nCad.*/
    vector<CNCadParticleContainer*> cells;
    /**Map containing the components inside nCad.*/
    vector<CNCadParticleContainer*> components;

    /**The ODT recorder for debugging purposes.*/
    LOGRecorderCreator LRC;
    
    /**Factory shape instance to generate the shapes.*/
    CFactory_Shape factory_shape;
    
public:
    /**Constructor.*/
    CNCadSimphony();
    /**Constructor given a name of session/project.
    @param project_name the name of the session.*/
    CNCadSimphony(const char * project_name);
    
    /**Method to obtain the NC_Wrapper interface.
    @returns NC_Wrapper pointer of the API interface.*/
    static NC_Wrapper * GetWrapperInterface();
    
    /**Adds a particle container as a component to the internal map and to nCad through the API.
    @param Component the particle container to add.
    @param cell_name name of the attached unit cell to the component. It must be a cell already added to the nCad session.
    @pc_info additional info of the particle container.*/
    void AddComponent(CNCadParticleContainer *Component, const char * cell_name, CParticleContainerInfo *pc_info);
    /**Adds a particle container as a component to the internal map and to nCad through the API.
    @param Cell the particle container to add-
    @param a a length of the cell.
    @param b b length of the cell.
    @param c c length of the cell.
    @param alpha alpha angle of the cell.
    @param beta beta angle of the cell.
    @param gamma gamma angle of the cell.
    @param symmetry_gn symmetry group number of the cell.*/
    void AddCell(CNCadParticleContainer *Cell, double a, double b, double c,
                            double alpha, double beta, double gamma,
                            int symmetry_gn);
    /**Deletes the specified component from nCad.
    @param name the name of the component.*/
    void RemoveComponent(string &name);
    /**Deletes the specified cell from nCad.
    @param name the name of the cell.*/
    void RemoveCell(string &name);
    
    /**Updates the information of the specified component inside nCad.
    @param Component the particle container to update.*/
    void UpdateComponent(CNCadParticleContainer &Component);
    /**Updates the information of the specified cell inside nCad.
    @param cell the particle container to update.*/
    void UpdateCell(CNCadParticleContainer &Cell);
    
    // CNCadParticleContainer GetComponent(string &name);
    // CNCadParticleContainer GetCell(string &name);
    
    /**Process a single component.
    Note: not implemented ftm.
    @param name name of the component to process.*/
    void ProcessComponent(string &name);
    
    /**Opens a visualizer with the specified component. The component must be added in nCad previously.
    @param name the name of the component.*/
    void ShowComponent(string &name);
    /**Opens a visualizer with the specified cell. The cell must be added in nCad previously.
    @param name the name of the cell.*/
    void ShowCell(string &name);
    
    /**Process all the components of the current assembly.*/
    void ProcessAll();
    /**Clear all the components of the current assembly.*/
    void ClearComponents();
    /**Clear all the cells of the current assembly.*/
    void ClearCells();
    // CNCadParticleContainer * GetAssembly();
    /**Obtains the assembly atoms from nCad and fills the given particle container with them.
    @param res the particle container to enter the atoms.*/
    void GetAssemblyAtoms(CNCadParticleContainer * res);
    /**Obtains the assembly bonds from nCad and fills the given particle container with them.
    @param res the particle container to enter the bonds.*/
    void GetAssemblyBonds(CNCadParticleContainer * res);
    /**Method that prepare things for the assembly processing.*/
    void BeginAssembly();
    /**Method that finish assembly processing.*/
    void EndAssembly();
    
    /**Special method to take a particle and insert a copy in the correspondent component after processing the assembly.
    @param pParticle the particle to process.*/
    void ProcessAssemblyParticle(CNCadParticle * pParticle, ID_TYPE Simphony_ID);
    /**Special method to take a bond and insert a copy in the correspondent component after processing the assembly.
    @param pBond the bond to process.*/
    void ProcessAssemblyBond(CNCadBond * pBond, ID_TYPE Simphony_ID);
    /**Retrieves info of a particle of the assembly.
    @param id the internal id of the particle.
    @returns a CParticleInfo with the attributes of the particle.*/
    CParticleInfo* GetAssemblyParticleInfo(id_t id);
    /**Retrieves info of a bond of the assembly.
    @param atom1 the internal id of the first atom.
    @param atom2 the internal id of the second atom.
    @returns a CBondInfo with the attributes of the bond.*/
    CBondInfo* GetAssemblyBondInfo(id_t atom1, id_t atom2);
    /* NOT FOR THE MOMENT ---- WE RETURN A SIMPHONY PC!!!
    id_t AddAtom(NC_Atom *pAtom);
    ERR UpdateAtom(id_t ID, NC_Atom *pAtom);
    ERR DeleteAtomByID(id_t ID);*/
    /**Retrieves the specified cell of the current assembly.
    @param cell_name name of the cell.
    @returns the CNCadParticleContainer of the corresponding cell.*/
    CNCadParticleContainer * getCell(const char * cell_name);
    /**Retrieves the specified component of the current assembly.
    @param component_name name of the component.
    @returns the CNCadParticleContainer of the corresponding component.*/
    CNCadParticleContainer * getComponent(const char * component_name);
    
    /**Loads a new project, clearing the previous session.*/
    void LoadSession(string session);
};

/**The unique instance of the Simphony nCad adapter.*/
extern CNCadSimphony * pCNCadSimphony;

#endif /*__NCAD_SIMPHONY_WRAPPER__H__*/
