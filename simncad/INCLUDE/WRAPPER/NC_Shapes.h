#ifndef __NC_SHAPES
#define __NC_SHAPES

/**@pkg _NC_SHAPES*/

/**Class for 3D unit cell block */   
//------------------------------------------------------------------------------
class NC_WRAPPER_EXPORT NC_BlockUC_3D : public NC_ShapeNO_CAD
//------------------------------------------------------------------------------
{
public:
/**Block size along A vector in cells */
  int CellLenX;
/**Block size along B vector in cells */
  int CellLenY;
/**Block size along C vector in cells */
  int CellLenZ;

/**Constructor of NC_BlockUC_3D class  */
  NC_BlockUC_3D(double ax, double ay, double az, double aCellLenX, double aCellLenY, double aCellLenZ, NC_ShapeOrientation *apOrientation = 0);
/**Destructor */
  ~NC_BlockUC_3D();

/**Getter for copy of shape */
  NC_Shape *GetCopy() const;

/**Method for getting shape domain
*   @returns 3D NO_CAD dimension domain
*/
  CompDomain GetCompDomain() const {return domainNoCAD_3D;}

/**Getter for type name of the shape
*   @returns Type name of the shape
*/
  const char *GetTypeName() const;

/**Method for getting parameters of the shape
*   @returns NULL in case of success or pointer to the error string in case of failure
*/
  ERR GetParameters(vector<string> &Parameters) const;
};


/**Class for three-dimensional rectangular block */   
//------------------------------------------------------------------------------
class NC_WRAPPER_EXPORT NC_BlockXYZ_3D : public NC_ShapeNO_CAD
//------------------------------------------------------------------------------
{
public:
/** Length along X aris*/
  double LenX;
/** Length along Y aris*/
  double LenY;
/** Length along Z aris*/
  double LenZ;

/**Constructor of NC_BlockXYZ_3D class  */
  NC_BlockXYZ_3D(double ax, double ay, double az, double aLenX, double aLenY, double aLenZ, NC_ShapeOrientation *apOrientation = 0);
/**Destructor */
  ~NC_BlockXYZ_3D();

/**Getter for copy of shape */
  NC_Shape *GetCopy() const;

/**Method for getting shape domain
*   @returns 3D NO_CAD dimension domain
*/
  CompDomain GetCompDomain() const {return domainNoCAD_3D;}

/**Getter for type name of the shape
*   @returns Type name of the shape
*/
  const char *GetTypeName() const;

/**Method for getting parameters of the shape
*   @returns NULL in case of success or pointer to the error string in case of failure
*/
  ERR GetParameters(vector<string> &Parameters) const;
};

/**Class for sphere shape*/   
//------------------------------------------------------------------------------
class NC_WRAPPER_EXPORT NC_Sphere : public NC_ShapeNO_CAD
//------------------------------------------------------------------------------
{
public:
/**Sphere radius */
  double Radius;

/**Constructor of NC_Sphere class  */
  NC_Sphere(double ax, double ay, double az, double aRadius);
/**Destructor */
  ~NC_Sphere();

/**Getter for copy of shape */
  NC_Shape *GetCopy() const;

/**Method for getting shape domain
*   @returns 3D NO_CAD dimension domain
*/
  CompDomain GetCompDomain() const {return domainNoCAD_3D;}

/**Getter for type name of the shape
*   @returns Type name of the shape
*/
  const char *GetTypeName() const;

/**Method for getting parameters of the shape
*   @returns NULL in case of success or pointer to the error string in case of failure
*/
  ERR GetParameters(vector<string> &Parameters) const;
};

/**Class for cylinder shape */   
//------------------------------------------------------------------------------
class NC_WRAPPER_EXPORT NC_Cylinder : public NC_ShapeNO_CAD
//------------------------------------------------------------------------------
{
public:

/**Cylinder radius */
  double Radius;
/**Cylinder length */
  double Length;

/**Constructor of NC_Cylinder class  */
  NC_Cylinder(double ax, double ay, double az, double aRadius, double aLength, NC_ShapeOrientation *apOrientation = 0);
/**Destructor */
  ~NC_Cylinder();

/**Getter for copy of shape */
  NC_Shape *GetCopy() const;

/**Method for getting shape domain
*   @returns 3D NO_CAD dimension domain
*/
  CompDomain GetCompDomain() const {return domainNoCAD_3D;}

/**Getter for type name of the shape
*   @returns Type name of the shape
*/
  const char *GetTypeName() const;

/**Method for getting parameters of the shape
*   @returns NULL in case of success or pointer to the error string in case of failure
*/
  ERR GetParameters(vector<string> &Parameters) const;
};


/**Class for hexagonal prism shape */   
//------------------------------------------------------------------------------
class NC_WRAPPER_EXPORT NC_HexPrism : public NC_ShapeNO_CAD
//------------------------------------------------------------------------------
{
public:

/**Prism hexagon side */
  double Side;
/**Prism length */
  double Length;

/**Constructor of NC_HexPrism class  */
  NC_HexPrism(double ax, double ay, double az, double aSide, double aLength, NC_ShapeOrientation *apOrientation = 0);
/**Destructor */
  ~NC_HexPrism();

/**Getter for copy of shape */
  NC_Shape *GetCopy() const;

/**Method for getting shape domain
*   @returns 3D NO_CAD dimension domain
*/
  CompDomain GetCompDomain() const {return domainNoCAD_3D;}

/**Getter for type name of the shape
*   @returns Type name of the shape
*/
  const char *GetTypeName() const;

/**Method for getting parameters of the shape
*   @returns NULL in case of success or pointer to the error string in case of failure
*/
  ERR GetParameters(vector<string> &Parameters) const;
};


/**Class for 2D unit cell block */      
//------------------------------------------------------------------------------
class NC_WRAPPER_EXPORT NC_BlockUC_2D : public NC_ShapeNO_CAD
//------------------------------------------------------------------------------
{
public:

/**Block size along A vector in cells */
  int CellLenX;
/**Block size along B vector in cells */
  int CellLenY;

/**Constructor of NC_BlockUC_2D class  */
  NC_BlockUC_2D(double ax, double ay, double az, double aCellLenX, double aCellLenY, NC_ShapeOrientation *apOrientation = 0);
/**Destructor */
  ~NC_BlockUC_2D();

/**Getter for copy of shape */
  NC_Shape *GetCopy() const;

/**Method for getting shape domain
*   @returns 2D NO_CAD dimension domain
*/
  CompDomain GetCompDomain() const {return domainNoCAD_2D;}

/**Getter for type name of the shape
*   @returns Type name of the shape
*/
  const char *GetTypeName() const;

/**Method for getting parameters of the shape
*   @returns NULL in case of success or pointer to the error string in case of failure
*/
  ERR GetParameters(vector<string> &Parameters) const;
};

  
/**Class for two-dimensional rectangular block */   
//------------------------------------------------------------------------------
class NC_WRAPPER_EXPORT NC_BlockXYZ_2D : public NC_ShapeNO_CAD
//------------------------------------------------------------------------------
{
public:

/** Length along X aris*/
  double LenX;
/** Length along Y aris*/
  double LenY;

/**Constructor of NC_BlockXYZ_2D class  */
  NC_BlockXYZ_2D(double ax, double ay, double az, double aLenX, double aLenY, NC_ShapeOrientation *apOrientation = 0);
/**Destructor */
  ~NC_BlockXYZ_2D();

/**Getter for copy of shape */
  NC_Shape *GetCopy() const;

/**Method for getting shape domain
*   @returns 2D NO_CAD dimension domain
*/
  CompDomain GetCompDomain() const {return domainNoCAD_2D;}

/**Getter for type name of the shape
*   @returns Type name of the shape
*/
  const char *GetTypeName() const;

/**Method for getting parameters of the shape
*   @returns NULL in case of success or pointer to the error string in case of failure
*/
  ERR GetParameters(vector<string> &Parameters) const;
};


/**Class for disk shape */   
//------------------------------------------------------------------------------
class NC_WRAPPER_EXPORT NC_Disk : public NC_ShapeNO_CAD
//------------------------------------------------------------------------------
{
public:

/**Disk radius */
  double Radius;

/**Constructor of NC_Disk class  */
  NC_Disk(double ax, double ay, double az, double aRadius, NC_ShapeOrientation *apOrientation = 0);
/**Destructor */
  ~NC_Disk();

/**Getter for copy of shape */
  NC_Shape *GetCopy() const;

/**Method for getting shape domain
*   @returns 2D NO_CAD dimension domain
*/
  CompDomain GetCompDomain() const {return domainNoCAD_2D;}

/**Getter for type name of the shape
*   @returns Type name of the shape
*/
  const char *GetTypeName() const;

/**Method for getting parameters of the shape
*   @returns NULL in case of success or pointer to the error string in case of failure
*/
  ERR GetParameters(vector<string> &Parameters) const;
};


/**Class for hexagon shape */   
//------------------------------------------------------------------------------
class NC_WRAPPER_EXPORT NC_Hexagon : public NC_ShapeNO_CAD
//------------------------------------------------------------------------------
{
public:

/**Hexagon side */
  double Side;

/**Constructor of NC_Hexagon class  */
  NC_Hexagon(double ax, double ay, double az, double aSide, NC_ShapeOrientation *apOrientation = 0);
/**Destructor */
  ~NC_Hexagon();

/**Getter for copy of shape */
  NC_Shape *GetCopy() const;

/**Method for getting shape domain
*   @returns 2D NO_CAD dimension domain
*/
  CompDomain GetCompDomain() const {return domainNoCAD_2D;}

/**Getter for type name of the shape
*   @returns Type name of the shape
*/
  const char *GetTypeName() const;

/**Method for getting parameters of the shape
*   @returns NULL in case of success or pointer to the error string in case of failure
*/
  ERR GetParameters(vector<string> &Parameters) const;
};


/**Class for 1D unit cell block */      
//------------------------------------------------------------------------------
class NC_WRAPPER_EXPORT NC_BlockUC_1D : public NC_ShapeNO_CAD
//------------------------------------------------------------------------------
{
public:

/**Block size along A vector in cells */
  int CellLenX;

/**Constructor of NC_BlockUC_1D class  */
  NC_BlockUC_1D(double ax, double ay, double az, double aCellLenX, NC_ShapeOrientation *apOrientation = 0);
/**Destructor */
  ~NC_BlockUC_1D();

/**Getter for copy of shape */
  NC_Shape *GetCopy() const;

/**Method for getting shape domain
*   @returns 1D NO_CAD dimension domain
*/
  CompDomain GetCompDomain() const {return domainNoCAD_1D;}

/**Getter for type name of the shape
*   @returns Type name of the shape
*/
  const char *GetTypeName() const;

/**Method for getting parameters of the shape
*   @returns NULL in case of success or pointer to the error string in case of failure
*/
  ERR GetParameters(vector<string> &Parameters) const;
};


/**Class for 1D objects with limited length */   
//------------------------------------------------------------------------------
class NC_WRAPPER_EXPORT NC_BlockXYZ_1D : public NC_ShapeNO_CAD
//------------------------------------------------------------------------------
{
public:

/** Length along X aris*/
  double LenX;

/**Constructor of NC_BlockXYZ_1D class  */
  NC_BlockXYZ_1D(double ax, double ay, double az, double aLenX, NC_ShapeOrientation *apOrientation = 0);
/**Destructor */
  ~NC_BlockXYZ_1D();

/**Getter for copy of shape */
  NC_Shape *GetCopy() const;

/**Method for getting shape domain
*   @returns 1D NO_CAD dimension domain
*/
  CompDomain GetCompDomain() const {return domainNoCAD_1D;}

/**Getter for type name of the shape
*   @returns Type name of the shape
*/
  const char *GetTypeName() const;

/**Method for getting parameters of the shape
*   @returns NULL in case of success or pointer to the error string in case of failure
*/
  ERR GetParameters(vector<string> &Parameters) const;
};


/**Class for zero-dimensional object */   
//------------------------------------------------------------------------------
class NC_WRAPPER_EXPORT NC_Object_0D : public NC_ShapeNO_CAD
//------------------------------------------------------------------------------
{
public:

/**Constructor of NC_Object_0D class  */
  NC_Object_0D(double ax, double ay, double az, NC_ShapeOrientation *apOrientation = 0);
/**Destructor */
  ~NC_Object_0D();

/**Getter for copy of shape */
  NC_Shape *GetCopy() const;

/**Method for getting shape domain
*   @returns 0D NO_CAD dimension domain
*/
  CompDomain GetCompDomain() const {return domainNoCAD_0D;}

/**Getter for type name of the shape
*   @returns Type name of the shape
*/
  const char *GetTypeName() const;

/**Method for getting parameters of the shape
*   @returns NULL in case of success or pointer to the error string in case of failure
*/
  ERR GetParameters(vector<string> &Parameters) const;
};

#endif