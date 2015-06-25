#ifndef __BOND_PARAMETERS
#define __BOND_PARAMETERS

/**@pkg _SERVICE */

#include "Service.h"
//==============================================================================
/**Different types of bonds*/
enum BondType
{
  bondUndefined,
  bondCovalent,
  bondIonic,
  bondMetalic,
};

/**Bond parameters structure*/
struct BondParameters
{
  BondType Type;
  BondParameters();
  void ToStrings(vector<string> &Strings) const;
  void FromStrings(const vector<string> &Strings);
  BOOL operator == (const BondParameters &BP) const;
};

ostream &operator << (ostream &os, const BondParameters &BP);
istream &operator >> (istream &is, BondParameters &BP);


#endif