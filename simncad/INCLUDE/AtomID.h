#ifndef __ATOM_ID
#define __ATOM_ID

/**@pkg _SERVICE*/

#include "Service.h"

//STUB!!! For 10 bits
#define UNKNOWN_COMPONENT_ID 0X3FF
//STUB!!! Necessary deside correct value
#define MANUAL_COMPONENT_ID  0X3FE
//STUB!!! For 10 bits
#define UNKNOWN_ATOM_ID      0X3FF
//STUB!!! For 12 bits of Plane only Better change for Plane, Col, Row alltogether
#define UNKNOWN_CELL_ID 0XFFF

//STUB!!! 32 / 64 dependent
// 32bits limits
// Col, Row, Plane - 4K
// Atom            - 1K
// Components      - 1K
// Reserve         - 8bits
//
// 64bits limits
// Col, Row, Plane - 4K
// Atom            - 4K
// Components      - 4K
// Reserve         - 4bits

/**Union AtomID for 32/64 bits*/
//This is for 32/64 bits
  typedef struct
  {
    DWORD Atom : 10;
    DWORD Col : 12;
    DWORD Reserve : 10;
    DWORD Row : 11;
    DWORD Plane : 11;
    DWORD Component : 10;
  } AtomIDIndexes;

/*
//This is for 64 bits 
  struct
  {
    DWORD Plane : 12;
    DWORD Row : 12;
    DWORD Col : 12;
    DWORD Atom : 12;
    DWORD Component : 12;
    DWORD Reserve : 4;
  } Indexes;
*/

union AtomID
{
  DWORD64 ID;
  AtomIDIndexes Indexes;

  AtomID(DWORD64 val) {ID = val;}
  AtomID() {ID = 0;}

  BOOL IsUnknownComponent() const;
  void SetUnknownComponent();
  BOOL IsManualComponent() const;
  void SetManualComponent();
  BOOL IsUnknownCell() const;
  void SetUnknownCell();
  BOOL IsUnknownAtom() const;
  void SetUnknownAtom();
};

BOOL operator == (AtomID ID1, AtomID ID2);

//For debugging!!!
DWORD64 GetAtomID(int Component, int Col, int Raw, int Plane, int Atom);

#define ATOM_ID_TYPE AtomID
//#define ATOM_ID_TYPE DWORD64

string AsString(AtomIDIndexes val);

//============================================================================

typedef struct
{
  DWORD Position;
  DWORD Atom;
} CellAtomIDIndexes;

union CellAtomID
{
  DWORD64 ID;
  CellAtomIDIndexes Indexes;

  CellAtomID(DWORD64 val) {ID = val;}
  CellAtomID(DWORD aPosition, DWORD aAtom);
  CellAtomID() {ID = 0;}
};

//For debugging
DWORD64 GetCellAtomID(DWORD Position, DWORD Atom = 0);

#define CELL_ATOM_ID_TYPE CellAtomID
//#define CELL_ATOM_ID_TYPE DWORD64

string AsString(CellAtomIDIndexes val);

//============================================================================
typedef struct
{
  DWORD Batch;
  char CellShiftA;
  char CellShiftB;
  char CellShiftC;

} CellBondIDIndexes;

union CellBondID
{
  DWORD64 ID;
  CellBondIDIndexes Indexes;

  CellBondID(DWORD64 val) {ID = val;}
  CellBondID(DWORD aBatch, char aCellShiftA, char aCellShiftB, char aCellShiftC);
  CellBondID() {ID = 0;}
};

//For debugging
DWORD64 GetCellBondID(DWORD Batch, char CellShiftA = 0, char CellShiftB = 0, char CellShiftC = 0);

#define CELL_BOND_ID_TYPE CellBondID
#define CELL_BOND_ID_TYPE_EXT DWORD64

string AsString(CellBondIDIndexes val);

/**Class for bond by pair of atoms  */
class BondAtomPair : public pair<DWORD64, DWORD64>
{
public:
  BondAtomPair(DWORD64 ID1, DWORD64 ID2) : pair<DWORD64, DWORD64>(ID1, ID2) {}

  BOOL operator == (const BondAtomPair &p) const;
};

/**Class for bond by pair of atom IDs  */
class BondAtomIDPair : public pair<ATOM_ID_TYPE, ATOM_ID_TYPE>
{
public:
  BondAtomIDPair(ATOM_ID_TYPE ID1, ATOM_ID_TYPE ID2) : pair<ATOM_ID_TYPE, ATOM_ID_TYPE>(ID1, ID2) {}

  BOOL operator == (const BondAtomIDPair &p) const;
};

BOOL IsEqualAtomPairs(ATOM_ID_TYPE Atom1ID1, ATOM_ID_TYPE Atom1ID2, ATOM_ID_TYPE Atom2ID1, ATOM_ID_TYPE Atom2ID2);
BOOL IsEqualCellAtomPairs(DWORD Atom1ID1, DWORD Atom1ID2, DWORD Atom2ID1, DWORD Atom2ID2);


#endif
