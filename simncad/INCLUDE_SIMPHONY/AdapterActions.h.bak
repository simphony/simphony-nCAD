#ifndef __ADAPTERACTIONS__H__
#define __ADAPTERACTIONS__H__

/**@pkg _SIMPHONY_ADAPTER*/

#include "WRAPPER/NC_Wrapper.h"
#include "NCadSimphonyWrapper.h"
#include <map>
using namespace std;

class CNCadParticleContainer;

class NC_Wrapper_GetAssemblyAtoms : public NC_AtomAction
/**Action class to retrieve all the atoms from the processed assembly.*/
{
    /**Particle container to fill.*/
    CNCadParticleContainer * pPc;
    
public:
    /**Auxiliar debug counter.*/
    int debug_cnt;
    /**Constructor.
    @param aPc the particle container address.
    */
    NC_Wrapper_GetAssemblyAtoms(CNCadParticleContainer * aPc) : pPc(aPc) {debug_cnt = 0;}
    /**The action method to perform.
    @param Atom the particle processed each step.
    */
    ERR DoAction (const NC_Atom &Atom);
};

class NC_Wrapper_GetAssemblyBonds : public NC_BondAction
/**Action class to retrieve all the bonds from the processed assembly.*/
{
    /**Particle container to fill.*/
    CNCadParticleContainer * pPc;
    /**Auxiliar table to optimize things.
    We need to fill each new processed bond with the simphony id's of their corresponding atoms.
    Since the mapping in the nCad wrapper is Simphony_ID ---> Internal_ID, trying to do the opposite is quite expensive,
    so we do a linear search in the original map of the nCad wrapper, and we store temporally the Internal_ID --- Simphony_ID
    correspondance in this temporal map, so if we processed a bond with internal atom ids that appeared previously, we can convert them
    faster.*/
    map<id_t, ID_TYPE> id_table;
    /**We use this to generate a temporal fake id, so the bonds are retrieved and stored properly. The ids of the bonds will be replaced for
    Simphony ids (uuid.UUID).*/
    id_t current_id;
public:
    /**Auxiliar debug counter.*/
    int debug_cnt;
    /**Constructor.
    @param aPc the particle container address.
    */
    NC_Wrapper_GetAssemblyBonds(CNCadParticleContainer * aPc) : pPc(aPc) {debug_cnt = 0; current_id = 0;}
    /**Method to retrieve the Simphony ID of a given internal id.
    @param id the internal id of the atom.
    */
    ID_TYPE GetAtomID(id_t id);
    /**The action method to perform.
    @param Bond the particle processed each step.
    */
    ERR DoAction (const NC_Bond &Bond);
};

#endif /*__ADAPTERACTIONS__H__*/