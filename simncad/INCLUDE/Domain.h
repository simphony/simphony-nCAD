#ifndef __DOMAIN111
#define __DOMAIN111

#include "Service.h"

#define USE_DEFAULT_DOMAIN domainUnknown

enum Dimension
{
  dim3D,
  dim2D,
  dim1D,
  dim0D,
  dimUnknown,
};

enum CompDomain
{
  domain3D = 100,
  domain2D,
  domain1D,
  domain0D,

  domainNoCAD_3D,
  domainNoCAD_2D,
  domainNoCAD_1D,
  domainNoCAD_0D,

  domainUnknown = 1000, 
};

Dimension GetDimensionType(const string &Type);

CompDomain GetDomainComponentType(const string &Type);
Dimension GetDimensionComponentType(const string &Type);
Dimension GetDimensionComponentDomain(DWORD CompDomain);

string GetTypeNameDimension(Dimension dim);
string GetTypeNameCompDomain(CompDomain dim);


#define domainVolumeDefect 200 

#endif