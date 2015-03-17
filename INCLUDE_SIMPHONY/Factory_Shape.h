#ifndef __FACTORY_SHAPE__H__
#define __FACTORY_SHAPE__H__

#include <map>
#include <string>
#define INVALID -999999

using namespace std;

class NC_Shape;

/**Enumerator type to specifie the dimension and shape of components.*/
typedef enum {
    DIM_3D_BLOCK_UC    = 1,
    DIM_3D_BLOCK_XYZ   = 2,
    DIM_3D_SPHERE      = 3,
    DIM_3D_CYLINDER    = 4,
    DIM_3D_HEXPRISM    = 5,
    DIM_2D_BLOCK_UC    = 6, 
    DIM_2D_BLOCK_XYZ   = 7,
    DIM_2D_DISK        = 8,
    DIM_2D_HEXAGON     = 9,
    DIM_1D_BLOCK_UC    = 10, 
    DIM_1D_BLOCK_XYZ   = 11,
    DIM_0D_ATOM_LIST   = 12
} SHAPETYPE;

/**Struct to pass information from Cython side to C++ side.*/
struct shape_info
{
    int shape;
    double centerX, centerY, centerZ;
    double lengthX, lengthY, lengthZ;
    int lengthXUC, lengthYUC, lengthZUC;
    double radius;
    double side; //for hexagon
    int rotation_axis1, rotation_axis2;
    double shape_rotation1[3], shape_rotation2[3];
    shape_info();
};
/**Type for shape_info struct.*/
typedef shape_info CShapeInfo;

/**Functions type for the map of the factory class for shapes.*/
typedef NC_Shape* (*newShape)(CShapeInfo&);

class CFactory_Shape
/**Factory class for shapes.*/
{
private:
    static map<int, newShape> shapes;
    static NC_Shape * CreateBlockUC_3D(CShapeInfo &shape);
    static NC_Shape * CreateBlockXYZ_3D(CShapeInfo &shape);
    static NC_Shape * CreateSphere(CShapeInfo &shape);
    static NC_Shape * CreateCylinder(CShapeInfo &shape);
    static NC_Shape * CreateHexPrism(CShapeInfo &shape);
    static NC_Shape * CreateBlockUC_2D(CShapeInfo &shape);
    static NC_Shape * CreateBlockXYZ_2D(CShapeInfo &shape);
    static NC_Shape * CreateDisk(CShapeInfo &shape);
    static NC_Shape * CreateHexagon(CShapeInfo &shape);
    static NC_Shape * CreateBlockUC_1D(CShapeInfo &shape);
    static NC_Shape * CreateBlockXYZ_1D(CShapeInfo &shape);
    static NC_Shape * CreateObject_0D(CShapeInfo &shape);
public:
    /**Constructor.*/
    CFactory_Shape();
    /**Static method for creating the shapes.
    @param shape the shape information used to create the shape.
    @returns the corresponding new allocated NC_Shape pointer.*/
    static NC_Shape * CreateShape(CShapeInfo &shape);
};

#endif /*__FACTORY_SHAPE__H__*/

