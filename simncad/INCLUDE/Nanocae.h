#ifndef __NANOCAE_ALL
#define __NANOCAE_ALL

/**@pkg _NANOCAE*/

#include <string>
using namespace std;
//==============================================================================
enum PathType
{
  pathLIB_UC,
  pathLIB_UC3D,
  pathLIB_UC2D,
  pathLIB_UC1D,
  pathLIB_UC0D,
  pathD0,
  pathCAD,
  pathJOBS,
  pathEXPORT,
  pathLIB_MATERIAL_3D,
  pathLIB_MATERIAL_2D,
  pathLIB_MATERIAL_1D,
  pathLIB_MATERIAL_0D,
  pathLIB_ORIENTATION_3D,
  pathLIB_ORIENTATION_2D,
  pathLIB_ORIENTATION_1D,
//NOT_NOW  pathLIB_ORIENTATION_0D,
  pathLIB_NANOTUBE_LAYOUT,
  pathMAX
};

enum ExportType
{
  exportAssembly,
//  exportUnitCell,
};

enum UCExportType
{
  exportUCLib,
  exportUCTmp,
};
/*
enum
{
  domainDialogWidgets,
  domainMainWidgets,
};
*/
#define USE_DEFAULT_EXT ""

#define CS_FRACTIONAL "Fractional"
#define CS_CARTESIAN  "Cartesian XYZ"

#define USE_DEFAULT_UC_EXT "cd"

void InitGlobals();
void InitAtomizerJobXML();

PathType GetMaterialPathType(DWORD Domain);
PathType GetOrientationPathType(DWORD Domain);
PathType GetNanotubeLayoutPathType(DWORD Domain);


enum
{
  modeBondUseCellDataBonds,
  modeBondMaxLen,
};

/**Structure containing the component information*/
struct ComponentInfo
{
  string MaterialName;
  string BulkUCName;
  string Orientation;
};

/**Structure containing the material information*/
struct MaterialInfo
{
  string MaterialName;
  string BulkUCName;
};

enum GeomType
{
  geomTypeInitial,
  geomTypeModification,
  geomTypeResulted,
};

enum ScopeType
{
  scopeComponent,
  scopeProject,
  scopeLibrary,
};


#define PROJECT_NONAME "NONAME"

#endif

