#ifndef __NC_WRAPPER
#define __NC_WRAPPER

/**@pkg _NC_WRAPPER*/

#define NC_WRAPPER_BUILT_AS_STATIC
#include "NC_WRAPPER_Export.h"

#include "Service.h"
#include "Geometry.h"
#include "Nanocae.h"
#include "Domain.h"
#include "BondParameters.h"
//TMP!!!
//#include "ShapeConsts.h"

#include <utility>
//#include <iostream>
//==============================================================================
class NC_Cell;

//typedef long id_t;
typedef DWORD64 id_t;

typedef struct 
{
    string cell;
    double n;
    double m;
} NC_NanotubeLayer;


/**Class that represents an atom */
//------------------------------------------------------------------------------
class NC_WRAPPER_EXPORT NC_Atom
//------------------------------------------------------------------------------
{
/**Identification number of the atom */  
  id_t ID;
/**Pointer on unit Cell. NULL for the component atoms*/  
  NC_Cell *pCell; 

public:
//  static id_t GetPositionID(id_t FullAtomID);
/**Name of the element */  
  string Element;
/**Label of the element */  
  string Label;
//  Vector3D fract;
/**Cartesian coordinates */   
  Vector3D xyz;
/**Atom occupancy */  
  double Occupancy;

/**Constructor of NC_Atom class with atom ID */  
  NC_Atom(id_t aID, const string &aLabel, const string &aElement, double aOccupancy, double x, double y, double z);
/**Constructor of NC_Atom class with atom Label */  
  NC_Atom(const string &aLabel, const string &aElement, double aOccupancy, double x, double y, double z);
/**Destructor */  
  ~NC_Atom();

/**Method for getting a copy of atom
*   @returns Pointer on atom copy 
*/  
  NC_Atom *GetCopy() const; 
/**Getter for identification number of the atom
*   @returns Identification number of the atom
*/  
  id_t GetID() const {return ID;}
};

/**Stream output operator for NC_Atom class */
ostream & NC_WRAPPER_EXPORT operator << (ostream &os, const NC_Atom &Atom);

class NC_CellBondAction;

/**Class that represents a bond */
//------------------------------------------------------------------------------
class NC_WRAPPER_EXPORT NC_Bond
//------------------------------------------------------------------------------
{
  friend class NC_Cell;
  friend class NC_CellBondAction;
  friend class NC_CrystalCellBondAction;

/**Batch ID for CellData bond, 0 for component bonds*/
  id_t ID;

/**Setter for the batch ID
*   @param aID Bond batch identification number
*/
  void SetID(id_t aID) {ID = aID;}

/**Pointer on unit Cell. NULL for the component bond*/  
  NC_Cell *pCell;

private:
/**aID1, aID2 - Position IDs for CellData bonds, Atom IDs for Component bonds*/
  NC_Bond(id_t aID1, id_t aID2, const BondParameters *pBondParams = NULL);
public:
/**Parameters of the bond */
  BondParameters BondParams;

/**NC_Bond object creator for unit cell 
*   @param aID1, aID2 Position IDs for CellData bonds
*   @param pBondParams Parameters of the bond
*   @returns Pointer on bond
*/
  static NC_Bond *CreateBondForCell(id_t aID1, id_t aID2, const BondParameters *pBondParams = NULL);

/**NC_Bond object creator for component
*   @param aID1, aID2 Atom IDs for component bonds
*   @param pBondParams Parameters of the bond
*   @returns Pointer on bond
*/
  static NC_Bond *CreateBondForComponent(id_t aID1, id_t aID2, const BondParameters *pBondParams = NULL);

//  NC_Bond(const string aLabel1, const string aLabel2);

/**Destructor */  
  ~NC_Bond();

/**Method for getting a copy of bond
*   @returns Pointer on bond copy
*/  
  NC_Bond *GetCopy() const;

/**Getter for the Batch ID
*   @returns Bond batch ID 
*/
  id_t GetID() const {return ID;}

/**Atomic Position ID1 for CellData / Atom ID for component or assembly*/
  id_t ID1;
/**Atomic Position ID2 for CellData / Atom ID for component or assembly*/
  id_t ID2;
};

/**Stream output operator for NC_Bond class */
ostream & NC_WRAPPER_EXPORT operator << (ostream &os, const NC_Bond &Bond);


/**Action objects base class for atoms iteration*/
//------------------------------------------------------------------------------
class NC_AtomAction
//------------------------------------------------------------------------------
{
public:
/**Principal method of the AtomAction objects. Iterating procedure calls this method for each atom
* if DoAction method returns not zero - iteration procedure will stop.
* Atom Reference on currently iterating atom
* @returns NULL in case of success or pointer to the error string in case of failure    
*/  
  virtual ERR DoAction(const NC_Atom &Atom) = 0;
};

/**Action objects base class for bonds iteration*/
//------------------------------------------------------------------------------
class NC_BondAction
//------------------------------------------------------------------------------
{
public:
/**Principal method of the BondAction objects. Iterating procedure calls this method for each bond
* if DoAction method returns not zero - iteration procedure will stop.
* Bond Reference on currently iterating bond
* @returns NULL in case of success or pointer to the error string in case of failure    
*/  
  virtual ERR DoAction(const NC_Bond  &Bond) = 0;
};
//------------------------------------------------------------------------------
class NC_WRAPPER_EXPORT NC_Nanotube
//------------------------------------------------------------------------------
{
public:
    NC_Nanotube();
    ~NC_Nanotube();

    void AddLayer(const string& cell_name, double n, double m);
    NC_Cell * BuildNanotubeCell();
    NC_Cell * BuildTestNanotubeCell();

private:
    vector<NC_NanotubeLayer> layers;
};
//------------------------------------------------------------------------------
class CellData;

/**Class that represents a unit cell */
//------------------------------------------------------------------------------
class NC_WRAPPER_EXPORT NC_Cell
//------------------------------------------------------------------------------
{
/**Scope enumerator */
  enum ScopeType Scope;
public:
/**Pointer on CellData */  
  CellData *pCell;
/**Unit cell name */
  string CellName;
/**Reference string of the unit cell */
  string Reference;

/**Constructor of NC_Cell class */  
  NC_Cell();
/**Destructor */  
  ~NC_Cell();

/**Method for getting a copy of unit cell
*   @returns Pointer on unit cell copy
*/  
  NC_Cell *GetCopy() const;

/**Getter for the scope type
*   @returns Scope type 
*/  
  enum ScopeType GetScope() const {return Scope;}

/**Getter for the alpha angle of the unit cell (in grad)
*   @returns Cell alpha 
*/
  double GetAlpha() const;
/**Getter for the beta angle of the unit cell (in grad)
*   @returns Cell beta 
*/
  double GetBeta() const;
/**Getter for the gamma angle of the unit cell (in grad)
*   @returns Cell gamma 
*/
  double GetGamma() const;
/**Getter for the a length of the unit cell (in Angstrom)
*   @returns Cell a
*/
  double GetA() const;
/**Getter for the b length of the unit cell (in Angstrom)
*   @returns Cell b
*/
  double GetB() const;
/**Getter for the c length of the unit cell (in Angstrom)
*   @returns Cell c
*/
  double GetC() const;
/**Getter for the volume of the unit cell 
*   @returns Total volume of the cell
*/
  double GetVolume() const;

/**Setter for a new geometry of unit cell
*   @param a, b, c Size of the unit cell
*   @param alpha, beta, gamma Angles of the unit cell 
*   @returns NULL in case of success or pointer to the error string in case of failure 
*/
  ERR SetNewGeometry(double a, double b, double c, double alpha, double beta, double gamma);

/**Conversion from fractional coordinates to Cartesian ones
*   @param axyz_fract Reference to the Vector3D object in fractional coordinates 
*   @returns Vector3D object in Cartesian coordinates
*/
  Vector3D GetXYZFromFract(const Vector3D &axyz_fract) const;

/**Conversion from Cartesian coordinates to fractional ones
*   @param axyz Reference to the Vector3D object in Cartesian coordinates
*   @returns Vector3D object in fractional coordinates
*/
  Vector3D GetFractFromXYZ(const Vector3D &axyz) const;

/**Access to the atom by Label
*    @param Label Label of the atom to search for
*    @returns Pointer on the NC_Atom object if atom exists or NULL if not
*/
  NC_Atom *GetAtomByLabel(const string &Label) const;

/**Getter for the atom ID by Label
*    @param Label Label of the atom to search for
*    @returns Atom identification number
*/
  id_t GetAtomIDByLabel(const string &Label) const;
//...
/**Access to the atom by ID
*    @param ID ID of the atom to search for
*    @returns Pointer on the NC_Atom object if atom exists or NULL if not
*/
  NC_Atom *GetAtomByID(id_t ID) const;

//???  id_t AddAtom(const NC_Atom &Atom);
/**Creates new position if it doesn't exist and adds atom to this position
*    @param pAtom NC_Atom to be added 
*    @returns Atom ID in the unit cell
*/ 
  id_t AddAtom(NC_Atom *pAtom);

//???  ERR UpdateAtom(id_t ID, const NC_Atom &Atom);
/**Method for update atom 
*    @paramID Atom ID
*    @param pAtom NC_Atom to be updated
*    @returns NULL in case of success or pointer to the error string in case of failure
*/ 
  ERR UpdateAtom(id_t ID, NC_Atom *pAtom);

/**Erase specified atom site
*    @param Label Label of the atom to be removed
*    @returns NULL in case of success or pointer to the error string in case of failure
*/
  ERR DeleteAtomByLabel(const string &Label);

/**Delete atom by ID
*    @param ID ID of the atom to search for
*    @returns NULL in case of success or pointer to the error string in case of failure
*/
  ERR DeleteAtomByID(id_t ID);

//  NC_Bond *GetBond(int ID) const;
/**Access to the bond by batch ID
*    @param ID ID of the bond to search for
*    @returns Pointer on the NC_Bond object if bond exists or NULL if not
*/
  NC_Bond *GetBondByBatchID(id_t ID) const;

/**Getting bond batch ID by AtomID1, AtomID2 - full atom IDs. AtomIDs are full IDs Position + Atom. Should be truncated to Position only 
*    @param AtomID1, AtomID2 AtomID1, AtomID2 - full atom IDs
*    @returns Bond batch ID if specified bond exists or -1 if not
*/
  id_t GetBondBatchID(id_t AtomID1, id_t AtomID2) const;

/**Getting bond batch ID by two labels 
*    @param Label1, Label2 Label of the atom 
*    @returns Bond batch ID if specified bond exists or -1 if not
*/
  id_t GetBondBatchID(const string &Label1, const string &Label2) const;
//...
//???  int AddBond(const NC_Bond &Bond);

/**Add new bond object into the cell
*    @param Bond NC_Bond object to be added
*    @returns Bond ID 
*/
  int AddBond(NC_Bond *pBond);

/**Removes a bond from the cell through the bond ID
*    @param Bond ID
*/
  void DeleteBond(int ID);


/**Iterates all atoms in the cell. During this procedure DoAction method of the AtomAction object
* will be called each atom
*   @param AtomAction Reference to the action object derived from base class NC_AtomAction
*   @returns NULL in case of success or pointer to the error string in case of failure 
*/
  ERR ForEachAtom(NC_AtomAction &AtomAction) const;

/**Iterates all bonds. During this procedure DoAction method of the BondAction object will be called each bond.
*   @param BondAction Reference to the action object derived from base class NC_BondAction
*   @param Mode Algorithm of bonds generation
*   @param pParams Pointer on bonds generation algorithm parameters 
*   @returns NULL in case of success or pointer to the error string in case of failure 
*/
  ERR ForEachBond(NC_BondAction &BondAction, DWORD Mode = 0, void *pParams = 0) const;

/**Iterates all bonds batch. Iterating procedure calls this method for each bonds batch.  
*   @param BondAction - Reference to the action object derived from base class NC_BondAction
*   @param Mode Algorithm of bonds generation
*   @param pParams Pointer on bonds generation algorithm parameters
*   @returns NULL in case of success or pointer to the error string in case of failure 
*/
  ERR ForEachBondBatch(NC_BondAction &BondAction, DWORD Mode = 0, void *pParams = 0) const;

/**Automatically creates all bonds in the cell 
*   @param Distance Maximal distance between two atoms linked by the bond
*   @returns NULL in case of success or pointer to the error string in case of failure 
*/
  ERR Autobond(double Distance);

/**Creates surface unit cell for the plane with Miller index h, k, l
*   @param h, k, l Miller index of the plane
*/
  void Reconstruct(int h, int k, int l);

/**Method for applying symmetry group operation with the name in Hermann-Mauguin notation 
*   @param GroupName Symmetry group name in Hermann-Mauguin notation    
*   @returns NULL in case of success or pointer to the error string in case of failure 
*/
  ERR ApplySymmetryHM(const string GroupName = "");

/**Method for apply symmetry group operation for specified group number  
*   @param GroupNumber Symmetry group number 
*   @returns NULL in case of success or pointer to the error string in case of failure 
*/
  ERR ApplySymmetryNumber(int GroupNumber);

/**Load unit cell data from the library
*   @param Name Unit cell name
*   @param dim Cell data dimension
*   @returns NULL in case of success or pointer to the error string in case of failure 
*/
  ERR LoadLib(const string &Name, enum Dimension dim = dim3D);

/**Load unit cell data from the project scope 
*   @param Name Unit cell name
*   @returns NULL in case of success or pointer to the error string in case of failure 
*/
  ERR LoadProj(const string &Name);

/**Saves the unit cell with its current name 
*   @returns NULL in case of success or pointer to the error string in case of failure 
*/
  ERR Save() const;

/**Saves the unit cell into the Library with specified name and dimension 
*   @param Name Unit cell name
*   @param dim Dimension
*   @returns NULL in case of success or pointer to the error string in case of failure 
*/
  ERR SaveAsLib(const string &Name, enum Dimension dim = dim3D);

/**Saves the unit cell into project scope 
*   @param Name Project name
*   @returns NULL in case of success or pointer to the error string in case of failure 
*/
  ERR SaveAsProj(const string &Name);

//  ERR Finalize(); 

/**Writes debug information into the stream 
*   @param os debug output stream
*/
  void Trace(ostream &os) const;

/**Writes debug information about cell geometry
*   @param pCell Pointer on CellData object
*/
  void TraceCellGeom(const NC_Cell *pCell); //static 
};

/**Class that represents first and second rotations of the Shape */
//------------------------------------------------------------------------------
class NC_WRAPPER_EXPORT NC_ShapeRotation
//------------------------------------------------------------------------------
{
public:
/**Character specifying the axis to be oriented into To direction*/
  char Axis;
/**Specified axis will be oriented along this vector*/ 
  Vector3D To;

/**Constructor of NC_ShapeRotation class */  
  NC_ShapeRotation(char aAxis, const Vector3D &aTo) :
    Axis(toupper(aAxis)),
    To(aTo)
  {}

/**Method for getting a copy of shape rotation 
*   @returns Pointer on shape rotation copy  
*/  
  NC_ShapeRotation *GetCopy() const;

/**Vector corresponding to selected Axis
*   @returns Return the vector corresponding to selected Axis
*/
  Vector3D GetFrom() const;
};

/**Class that represents a shape orientation three-dimensional objects */
//------------------------------------------------------------------------------
class NC_WRAPPER_EXPORT NC_ShapeOrientation
//------------------------------------------------------------------------------
{
public:
/**First exact rotation*/
  NC_ShapeRotation *pFirst;
/**Second rotation that tries to set specified axis to the plane produced by its To vector 
* and the To vector of the first rotation*/
  NC_ShapeRotation *pSecond;

/**Constructor of NC_ShapeOrientation class */ 
  NC_ShapeOrientation() {}
/**Constructor of NC_ShapeOrientation class with first and second rotation */
  NC_ShapeOrientation(NC_ShapeRotation *apFirst, NC_ShapeRotation *apSecond = NULL) :
    pFirst(apFirst),
    pSecond(apSecond)
  {}
//  NC_ShapeOrientation(const string &LibName);
/**Destructor */  
  ~NC_ShapeOrientation();

/**Method for getting a copy of shape orientation 
*   @returns Pointer on shape orientation copy  
*/  
  NC_ShapeOrientation *GetCopy() const; 
};

/**Specialization of NC_ShapeOrientation class for the case of two-dimensional objects */
//------------------------------------------------------------------------------
class NC_WRAPPER_EXPORT NC_ShapeOrientation2D : public NC_ShapeOrientation
//------------------------------------------------------------------------------
{
public:
/**NC_ShapeOrientation2D constructor
*   @param Normal Normal vector to the plane of two dimensional objects will be oriented along this vector
*   @param apSecond Optional pointer on second rotation
*/
  NC_ShapeOrientation2D(const Vector3D &Normal, NC_ShapeRotation *apSecond = NULL) :
    NC_ShapeOrientation(new NC_ShapeRotation('Z', Normal), apSecond)
  {}
};

/**Specialization of NC_ShapeOrientation class for the case of one-dimensional objects */
//------------------------------------------------------------------------------
class NC_WRAPPER_EXPORT NC_ShapeOrientation1D : public NC_ShapeOrientation
//------------------------------------------------------------------------------
{
public:
/**NC_ShapeOrientation1D constructor
*   @param Axial Axial vector of one dimensional objects will be oriented along this vector
*   @param apSecond Optional pointer on second rotation
*/
  NC_ShapeOrientation1D(const Vector3D &Axial, NC_ShapeRotation *apSecond = NULL) :
    NC_ShapeOrientation(new NC_ShapeRotation('X', Axial), apSecond)
  {}
};

/**Base class for all shapes*/
//------------------------------------------------------------------------------
class NC_WRAPPER_EXPORT NC_Shape
//------------------------------------------------------------------------------
{
public:
/**Constructor */  
  NC_Shape();

/**Destructor */  
  virtual ~NC_Shape();

/**Method for getting a copy of the shape
*   @returns Pointer on the shape copy 
*/ 
  virtual NC_Shape *GetCopy() const {return 0;}

/**Method for getting type name of derived classes
*   @returns Returns type name of derived classes
*/
  virtual const char *GetTypeName() const {return "";}

/**Method for getting domain of derived classes
*   @returns Returns domain of derived classes
*/
  virtual CompDomain GetCompDomain() const {return domainUnknown;}
};

/**Base class specialization for all NO_CAD shapes*/
//------------------------------------------------------------------------------
class NC_WRAPPER_EXPORT NC_ShapeNO_CAD : public NC_Shape 
//------------------------------------------------------------------------------
{
public:
/**Cartesian X coordinate of the shape center*/ 
  double x;
/**Cartesian Y coordinate of the shape center*/ 
  double y;
/**Cartesian Z coordinate of the shape center*/ 
  double z;
/**Shape orientation*/
  NC_ShapeOrientation *pOrientation;

/**NC_ShapeNO_CAD constructor
*   @param ax, ay, az Cartesian coordinates of the shape center
*   @param apSecond Optional pointer on the shape orientation
*/  NC_ShapeNO_CAD(double ax, double ay, double az, NC_ShapeOrientation *apOrientation = 0);

/**Destructor */  
  virtual ~NC_ShapeNO_CAD();

/**Gets parameters of derived shape classes
*   @param Parameters Parameters of derived shape classes
*   @returns NULL in case of success or pointer to the error string in case of failure 
*/
  virtual ERR GetParameters(vector<string> &Parameters) const {return 0;}
};
//------------------------------------------------------------------------------
#include "WRAPPER\NC_Shapes.h"

/**Class that represents a material*/  
//------------------------------------------------------------------------------
class NC_WRAPPER_EXPORT NC_Material
//------------------------------------------------------------------------------
{
public:
/**Pointer on bulk unit cell*/	
  NC_Cell *pBulkUC;
/**Material name*/
  string MaterialName;

/**Constructor of NC_Material class */
  NC_Material(NC_Cell *apBulkUC);
/**Destructor */  
  ~NC_Material();

/**Method for getting the bulk unit cell 
*   @returns Pointer on the bulk unit cell
*/
  NC_Cell *GetBulkCell() const;

/**Method for setting the bulk unit cell 
*   @param pCell Pointer on unit cell
*/
  void SetBulkCell(NC_Cell *pCell);

/*
In future versions 
  NC_Material(const string &LibName);
  int AddPointDefect(NC_PointDefect *pDefect);
  NC_PointDefect *GetPointDefect(int ID) const;
  void DeletePointDefect(int ID);

  int AddPairDefect(NC_PairDefect *pDefect);
  NC_PairDefect *GetPairDefect(int ID) const;
  void DeletePairDefect(int ID);

  ERR SaveAsLib(const string &FileName) const;
  ERR LoadLib(const string &FileName);
*/

/**Method for getting a copy of the material
*   @returns Pointer on the material copy 
*/  
  NC_Material *GetCopy() const;       
};

/**Class that represents rotation of crystal lattice*/  
//------------------------------------------------------------------------------
class NC_WRAPPER_EXPORT NC_CrystalRotation
//------------------------------------------------------------------------------
{
public:
/**Miller index of the plane which normal vector will be oriented along To*/
  MillerIndex Miller;
/**Fractional vector that will be oriented along To*/
  Vector3D From;
/**Vector where From vector or normal to plane with Miller will be oriented along*/
  Vector3D To;
/**Crystal rotation angle*/
  double Angle;

/**Constructor of NC_CrystalRotation 
*   @param aMiller Miller index of the plane that normal will be oriented along aTo
*   @param aTo Cartesian vector plane normal will be oriented along
*/
  NC_CrystalRotation(const MillerIndex &aMiller, const Vector3D &aTo) : 
    Miller(aMiller),
    To(aTo),
    Angle(0)
  {}

/**Constructor of NC_CrystalRotation 
*   @param aFrom Fractional vector that will be oriented along aTo
*   @param aTo Cartesian vector aFrom will be oriented along
*/
  NC_CrystalRotation(const Vector3D &aFrom, const Vector3D &aTo) :
    From(aFrom),
    To(aTo),
    Angle(0)
  {}

/**Constructor of NC_CrystalRotation 
*   @param aAngle Angle of clockwise rotation in degrees
*/
	NC_CrystalRotation(double aAngle) :
    Angle(aAngle)
  {}

/**Method for getting a copy of crystal rotation
*   @returns Pointer on copy of crystal rotation
*/  
  NC_CrystalRotation *GetCopy() const;
};

/**Class that represents a crystal first and second rotations */
//------------------------------------------------------------------------------
class NC_WRAPPER_EXPORT NC_CrystalOrientation
//------------------------------------------------------------------------------
{
public:
/**Pointer on first crystal rotation*/
  NC_CrystalRotation *pFirst;
/**Pointer on second crystal rotation*/
  NC_CrystalRotation *pSecond;

/**Constructor of NC_CrystalOrientation */
  NC_CrystalOrientation() {}
/**Constructor of NC_CrystalOrientation class with first and second rotation */
  NC_CrystalOrientation(NC_CrystalRotation *apFirst, NC_CrystalRotation *apSecond = NULL) :
    pFirst(apFirst),
    pSecond(apSecond)
  {}

/**Destructor */  
  ~NC_CrystalOrientation();

/**Method for getting a copy of crystal orientation
*   @returns Pointer on the copy of crystal orientation 
*/  
  NC_CrystalOrientation  *GetCopy() const;

  NC_CrystalOrientation(const string &LibName);
//  ERR SaveAsLib(const string &FileName) const;
//  ERR LoadLib(const string &FileName);
};

/**Class that represents a crystal orientation for two dimensional objects*/
//------------------------------------------------------------------------------
class NC_WRAPPER_EXPORT NC_CrystalOrientation2D : public NC_CrystalOrientation
//------------------------------------------------------------------------------
{
public:
/**Constructor of NC_CrystalRotation2D 
*   @param apSecond Optional pointer on second rotation of crystal lattice 
*   First rotation is taken from the shape orientation
*/  
  NC_CrystalOrientation2D(NC_CrystalRotation *apSecond = NULL);
};

/**Class that represents a crystal orientation for one dimensional objects*/
//------------------------------------------------------------------------------
class NC_WRAPPER_EXPORT NC_CrystalOrientation1D : public NC_CrystalOrientation
//------------------------------------------------------------------------------
{
public:
/**Constructor of NC_CrystalRotation1D 
*   @param apSecond Optional pointer on second rotation of crystal lattice 
*   First rotation is taken from the shape orientation
*/
  NC_CrystalOrientation1D(NC_CrystalRotation *apSecond = NULL);
};

class NC_Wrapper;

/**Class that represents a component*/
//------------------------------------------------------------------------------
class NC_WRAPPER_EXPORT NC_Component
//------------------------------------------------------------------------------
{
  friend class NC_Wrapper; 
/**Identification number of the component*/  
  int CompID;
public:
/**Name of the component*/   
  string Name;
/**Pointer on the component shape*/  
  NC_Shape *pShape;
/**Pointer on the component material*/  
  NC_Material *pMaterial;
/**Pointer on the component crystal orientation*/  
  NC_CrystalOrientation *pOrientation; //3D, 2D, 1D, 0D ??

/**NC_Component class constructor */
  NC_Component(
     const string &aName,
     NC_Shape *apShape,
     NC_Material *apMaterial,
     NC_CrystalOrientation *apOrientation = NULL); 

/**Destructor */  
  ~NC_Component();

/**Method for getting a copy of the component
*   @returns Pointer on the component copy 
*/  
  NC_Component *GetCopy() const;

/**Method for update the identification number of the component*/
  void UpdateID();

/**Getter for identification number of the component
*  @returns Identification number of the component
*/
  int GetID() const {return CompID;}

/**Getter for material of the component
*  @returns Pointer on component material
*/
  NC_Material *GetMaterial() const;

/**Method for setting the component material 
*   @param apMaterial Pointer on the component material
*/
  void SetMaterial(NC_Material *apMaterial);

/**Method for getting the component domain 
*   @returns Component domain
*/
  virtual CompDomain GetCompDomain() const;

/**Transfer of data from nCAD wrapper to the nCAD core
*   @returns NULL in case of success or pointer to the error string in case of failure 
*/
  ERR TransferData() const;
// ----------------------------------------
/**Getter for the atom in component 
*   @param ID Atom identification number. Atom ID should belong to the component otherwise NULL will be returned
*   @returns Pointer on the atom in component or NULL in the case of failure 
*/
  NC_Atom *GetAtomByID(id_t ID) const;

/**Add atom in the component 
*   @param pAtom Pointer on atom in the component
*   @returns Atom identification number or -1 in the case of failure
*/
  id_t AddAtom(NC_Atom *pAtom);

/**Update atom in the component 
*   @param ID Atom identification number. Atom ID should belong to the component otherwise error will be returned
*   @param pAtom Pointer on atom in the component
*   @returns NULL in case of success or pointer to the error string in case of failure 
*/
  ERR UpdateAtom(id_t ID, NC_Atom *pAtom);

/**Delete atom from the component 
*   @param ID Atom identification number. Atom ID should belong to the component otherwise error will be returned
*   @returns NULL in case of success or pointer to the error string in case of failure 
*/
  ERR DeleteAtomByID(id_t ID);
// ----------------------------------------
// New Bond Manipulation methods

/**Method for getting the bond
*   @param AtomID1, AtomID2 Atoms identification number
*   @returns Pointer on the bond or NULL in the case of failure
*/
  NC_Bond *GetBond(id_t AtomID1, id_t AtomID2) const;

/**Method for setting the bond in the component  
*   @param pBond Pointer on the bond
*   @returns NULL in case of success or pointer to the error string in case of failure 
*/
  ERR SetBond(NC_Bond *pBond);

/**Delete bond from the component  
*   @param AtomID1, AtomID2 Atoms identification number
*/
  void DeleteBond(id_t AtomID1, id_t AtomID2);

/**Delete all individually added or deleted bonds from the component */
  void ClearIndividualBonds();
// ----------------------------------------

/**Method for component shape cutting
*   @returns NULL in case of success or pointer to the error string in case of failure 
*/
  ERR Process();

/**Iterates all atoms in the component. During this procedure DoAction method of the AtomAction object
* will be called each atom
*   @param AtomAction Reference to the action object derived from base class NC_AtomAction
*   @returns NULL in case of success or pointer to the error string in case of failure 
*/
  ERR ForEachAtom(NC_AtomAction &AtomAction) const;
      
/**Iterates all bonds. During this procedure DoAction method of the BondAction object will be called each bond.
*   @param BondAction Reference to the action object derived from base class NC_BondAction
*   @param Mode Algorithm of bonds generation
*   @param pParams Pointer on bonds generation algorithm parameters 
*   @returns NULL in case of success or pointer to the error string in case of failure 
*/
  ERR ForEachBond(NC_BondAction &BondAction, DWORD Mode = modeBondUseCellDataBonds, void *pParams = 0) const;     
}; 

// NC_Component3D, 2D, 1D, 0D ??

class AllApplicationBase;
class BaseAtomicEditor1_0;
class CrystalApplicationForEditor1_0;
class CrystalApplicationForVisualizer1_0;

/**Class that represents a whole wrapper of the nCAD. Only one instance of this class is allowed*/
//------------------------------------------------------------------------------
class NC_WRAPPER_EXPORT NC_Wrapper
//------------------------------------------------------------------------------
{
public:
/**Pointer on whole nCAD application*/
  AllApplicationBase *pAll;
/**Pointer on Atomic Editor inside nCAD*/
  static BaseAtomicEditor1_0 *pEditor;
/**Pointer on Atomic Editor interface inside nCAD*/
  static CrystalApplicationForEditor1_0 *pAppEdit;
/**Pointer on Visualizer interface inside nCAD*/
  static CrystalApplicationForVisualizer1_0 *pAppVis; 

/**Components container*/
  vector<NC_Component *> Components;

/**Constructor */ 
  NC_Wrapper();
/**Destructor */  
  ~NC_Wrapper();

/**Method for clean and release all data both in wrapper and nCAD */
  void Clear();

/**Method for load the project 
*   @param Reference to the project name 
*   @returns NULL in case of success or pointer to the error string in case of failure 
*/
  ERR LoadProject(const string &Name);

/**Save the project with its current name  
*   @returns NULL in case of success or pointer to the error string in case of failure 
*/
  ERR SaveProject() const;

/**Save current project with specified name
*   @param Reference to the project name 
*   @returns NULL in case of success or pointer to the error string in case of failure 
*/
  ERR SaveProjectAs(const string &Name);

/**Method for opening new project 
*   @param Name Reference to the project name
*   @returns NULL in case of success or pointer to the error string in case of failure 
*/
  ERR NewProject(const string &Name);

//  BOOL CheckCADConnection() const;
//  ERR SetCADAssembly(const string &AssemblyName);

/**Method for component list getting 
*   @param Components Reference to the components
*/
  void GetComponentsList(vector<string> &Components) const;

/**Method for getting of component name by atom identification number
*   @param CompAtomID Atom identification number
*   @returns Component name
*/
  string GetComponentNameByAtomID(id_t CompAtomID) const;

/**Method for getting of component identification number by atom ID
*   @param CompAtomID Atom identification number 
*   @returns Component ID
*/
  int GetComponentIDByAtomID(id_t CompAtomID) const;

/**Method for getting component identification number by component name
*   @param ComponentName Reference to the component name 
*   @returns Component ID
*/
  int GetComponentIDByName(const string &ComponentName) const;

/**This method collects all information accessible through the NC_Component object, communicates with nCAD and creates respective XML node in Atomic editor 
*   @param pComp Pointer on the component
*   @returns NULL in case of success or pointer to the error string in case of failure 
*/
  ERR AddComponent(NC_Component *pComp);

/**Method for getting component by component name
*   @param ComponentName Reference to the component name 
*   @returns Pointer on the component copy
*/   
  NC_Component *GetComponent(const string &ComponentName);

/**Replace component
*   @param OldComponentName Component name before changing
*   @param pComp Pointer on the component
*   @returns NULL in case of success or pointer to the error string in case of failure 
*/
  ERR ReplaceComponent(const string &OldComponentName, NC_Component *pComp); 

/**Delete component 
*   @param ComponentName Reference to the component name
*   @returns NULL in case of success or pointer to the error string in case of failure 
*/
  ERR DeleteComponent(const string &ComponentName); 

/**Cutting of all components in the assembly
*   @returns NULL in case of success or pointer to the error string in case of failure 
*/
  ERR ProcessAll();                       
/**Cutting of modified components  
*   @returns NULL in case of success or pointer to the error string in case of failure 
*/
  ERR ProcessModified();                       

// ----------------------------------------
/**Getting the atom in the assembly by its ID
*   @param ID Identification number of atom
*   @returns Pointer on NC_Atom object or NULL in the case of failure 
*/
  NC_Atom *GetAtomByID(id_t ID) const;
/**Adding the atom into the assembly
*   @param pAtom Pointer on NC_Atom
*   @returns Atom Identification number or -1 in the case of failure 
*/
  id_t AddAtom(NC_Atom *pAtom);

/**Update atom in the assembly
*   @param ID Identification number of atom
*   @param pAtom Pointer on the atom
*   @returns NULL in case of success or pointer to the error string in case of failure 
*/
  ERR UpdateAtom(id_t ID, NC_Atom *pAtom);

/**Delete atom from the assembly by atom ID
*   @param ID Atom ID 
*   @returns NULL in case of success or pointer to the error string in case of failure 
*/
  ERR DeleteAtomByID(id_t ID);

/**Getting bond by pair of atom IDs 
*   @param aAtomID1, aAtomID1 Atom ID
*   @returns Pointer on NC_Bond object or NULL in the case of failure 
*/
  NC_Bond *GetBond(id_t aAtomID1, id_t aAtomID2) const;

/**Method for setting the bond in the assembly 
*   @param pBond Pointer on the bond
*   @returns NULL in case of success or pointer to the error string in case of failure 
*/
  ERR SetBond(NC_Bond *pBond);

/**Delete bond from the assembly 
*   @param aAtomID1, aAtomID2 Atoms identification number
*/
  void DeleteBond(id_t aAtomID1, id_t aAtomID2);

/**Clear all individually added or deleted bonds from the assembly 
*/
  void ClearIndividualBonds();

// ----------------------------------------

/**Iterates all atoms in the assembly. During this procedure DoAction method of the AtomAction object
* will be called each atom
*   @param AtomAction Reference to the action object derived from base class NC_AtomAction
*   @returns NULL in case of success or pointer to the error string in case of failure 
*/
  ERR ForEachAtom(NC_AtomAction &AtomAction) const; 

/**Iterates all bonds. During this procedure DoAction method of the BondAction object will be called each bond
*   @param BondAction Reference to the action object derived from base class NC_BondAction
*   @param Mode Algorithm of bonds generation
*   @param pParams Pointer on bonds generation algorithm parameters 
*   @returns NULL in case of success or pointer to the error string in case of failure 
*/
  ERR ForEachBond(NC_BondAction &BondAction, DWORD Mode = modeBondUseCellDataBonds, void *pParams = 0) const;     

/**Export all assembly into the file of XYZ format
*   @param FileName File Name
*   @returns NULL in case of success or pointer to the error string in case of failure 
*/
  ERR ExportToXYZ(const string &FileName) const;    
};

/**Function for error processing
*   @param err Error message or NULL if there is no error
*   @returns TRUE if error has been processed or FALSE if there is no error 
*/
BOOL ProcessError(ERR err);

#endif
