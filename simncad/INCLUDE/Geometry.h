#ifndef __GEOMETRY
#define __GEOMETRY

/**@pkg _GEOMETRY*/

#include "Service.h"
//#include "RandomGenerator.h"

class RandomGenerator;

#include <math.h>

#define PI2 (2 * M_PI)

double TO_RAD(double alpha);
double TO_GRAD(double alpha);
double AtanXY2PI(double y, double x);

/**
Represents three-dimensional vector.

Provides functionality for adding / substracting, scaling,
normalizing, comparison (taking in account GravityEps), making projections ant perpendiculars to the axis,
scalar and vector products, getting random vector, etc.
*/
class Vector3D
{
public:
  static double GravityEps;
  static const Vector3D Zero;

  double x,y,z;

  Vector3D(const Vector3D &v2) : x(v2.x),y(v2.y),z(v2.z) {}
  Vector3D(double ax = 0, double ay = 0, double az = 0);
  Vector3D &Set(double ax, double ay, double az);
  double &X(DWORD i);
  const double &X(DWORD i) const;
  BOOL operator == (const Vector3D &v2) const;
  BOOL operator != (const Vector3D &v2) const {return !operator == (v2);}
  Vector3D &operator = (const Vector3D &v2);
  Vector3D &operator += (const Vector3D &v2);
  Vector3D &operator -= (const Vector3D &v2);
  Vector3D &operator *= (double v2);
  Vector3D &operator /= (double v2);
  BOOL IsEqual(const Vector3D &v2, double eps = GravityEps) const;
//???  Vector3D &Round(double ax, double ay, double az, double eps = GravityEps);
  Vector3D &TryRound(double eps = GravityEps);
  Vector3D &RoundToInt();
  Vector3D &RoundToIntUp();
  Vector3D &RoundToIntDown();
  BOOL IsZero() const;
  double Len() const;
  double Len2() const;
  Vector3D &Normalize();
  void Trace(ostream &os) const;
};

Vector3D operator - (const Vector3D &v1);
Vector3D operator + (const Vector3D &v1, const Vector3D &v2);
Vector3D operator - (const Vector3D &v1, const Vector3D &v2);
Vector3D operator * (const Vector3D &v1, double v2);
Vector3D operator * (double v2, const Vector3D &v1);
Vector3D operator / (const Vector3D &v1, double v2);

double ScalarProduct(const Vector3D &v1, const Vector3D &v2);
Vector3D VectorProduct(const Vector3D &v1, const Vector3D &v2);
double GetAngle(const Vector3D &VA, const Vector3D &VB);
BOOL IsColinear(const Vector3D &VA, const Vector3D &VB);

Vector3D GetProjection(const Vector3D &v, const Vector3D &axis);
double GetProjectionLen(const Vector3D &v, const Vector3D &axis);
double GetProjectionValueSign(const Vector3D &v, const Vector3D &axis);
double GetProjectionValueNormalized(const Vector3D &v, const Vector3D &axis);
//STUB!!! Change name
Vector3D GetOrthogonal(const Vector3D &v1, const Vector3D &axis);

Vector3D ToPolarCoord(const Vector3D &v); // R, Fi, z
Vector3D FromPolarCoord(const Vector3D &v);
Vector3D ToSphericalCoord(const Vector3D &v); // R, Tetta, Fi
Vector3D FromSphericalCoord(const Vector3D &v);

ostream &operator << (ostream &os, const Vector3D &v);
istream &operator >> (istream &is, Vector3D &v);

Vector3D GetRandomVectorNormalized(RandomGenerator *apGenerator = 0);
BOOL IsInsideBlockXYZ(const Vector3D &Pos, const Vector3D &From, const Vector3D &To);

string AsString(const Vector3D &Vect);
ERR FromString(Vector3D &Vect, const string &SVect);
//==============================================================================
/**
Service class for determining of surrounding box for several vectors
*/
class Vector3DBox
{
//  vector<const Vector3D *> Vectors;
  Vector3D CornerMin;
  Vector3D CornerMax;
  BOOL IsFirst;
public:
  Vector3DBox() : IsFirst(TRUE) {}
  void Clear();
  void AddVector(const Vector3D &aVector);
  void AddVector(double x, double y, double z);
  void Shift(const Vector3D &aVector);
  void Shift(double x, double y, double z);
  Vector3D GetBoxCornerMin() const {return CornerMin;}
  Vector3D GetBoxCornerMax() const {return CornerMax;}
  Vector3D GetBoxCornerDiff() const {return CornerMax - CornerMin;}
  Vector3D GetBoxCenter() const {return (CornerMax + CornerMin) / 2;}
  BOOL IsEmpty() const {return IsFirst;}
  BOOL IsTrivial() const {return CornerMin == CornerMax;}
  BOOL IsContainPoint(double x, double y, double z) const;
  BOOL IsContainPoint(const Vector3D &Pos) const;
  Vector3DBox &operator *= (double v2);
  Vector3DBox &operator /= (double v2);
  Vector3DBox &TryRound(double eps = Vector3D::GravityEps);
  void Trace(ostream &os) const;
};
ostream &operator << (ostream &os, const Vector3DBox &box);
//istream &operator >> (istream &is, Vector3DBox &box);
//==============================================================================
/**
Represents functionality of linear matrix operator in three dimensions.

Supports scaling, product of operator * vector, getting determinant and invert operator.

Intended for coordinate rotation, conversion of Fractional / Cartesian coordinates, etc.
*/
class Operator3D
{
//STUB!!!
public:
//protected:
  double VA[9];
  void SetRow(DWORD row,const Vector3D &vRow);
  Vector3D GetRow(DWORD row) const;
  void SetCol(DWORD col,const Vector3D &vCol);
  Vector3D GetCol(DWORD col) const;
public:
  static double GravityEps;
  void Reset();
  Operator3D() {Reset();}
  Operator3D(const Operator3D &op);
  const Operator3D &operator = (const Operator3D &op);

  double &A(DWORD row, DWORD col);
  const double &A(DWORD row, DWORD col) const;
  double &V(DWORD pos);
  const double &V(DWORD pos) const;

  BOOL IsEmpty() const;
  void SetBasisVector(DWORD row,const Vector3D &vRow);
  Vector3D GetBasisVector(DWORD row) const;

  Operator3D GetInvert() const;
  double GetDeterminant() const;
  double GetVolume() const;

  Operator3D &operator *= (double val);
  Operator3D &operator *= (const Operator3D &op);
  Operator3D &operator /= (double val);

  void TryRound();

  void Trace(ostream &os) const;
};

string AsString(const Operator3D &Vect);

ostream &operator << (ostream &os, const Operator3D &op);
istream &operator >> (istream &is, Operator3D &op);

/**
  Returns Vector3DBox enveloping given box in another coordinate system.
  Operator transforms initial coordinates into new CS
*/
Vector3DBox GetEnvelopeBox(const Vector3DBox &Box, const Operator3D &Operator);

Operator3D GetReciptorialBasis(const Operator3D &XYZToFractOperator);
Operator3D GetBasisVectorsOrtogonalToCoordinatePlanesNorm(const Operator3D &XYZToFractOperator);

/**
Applies operator to the vector and returns result vector
*/
Vector3D operator * (const Operator3D &op, const Vector3D &v);
Operator3D operator * (const Operator3D &op1, const Operator3D &op2);
Operator3D operator * (const Operator3D &op1, double val);
Operator3D operator / (const Operator3D &op1, double val);

void TestOperator(const Operator3D &Operator, const string &FileName);

//==============================================================================
/**
Represents line by normalized axial vector and one point on the line
*/
class Line
{
//STUB!!!
public:
  Vector3D Axial;
  Vector3D Point;
//  double D;
public:
  Line(const Vector3D &aAxial, const Vector3D &aPoint);
  Line(const Line &aLine);
  const Vector3D &GetAxial() const {return Axial;}
  const Vector3D &GetPoint() const {return Point;}
  double GetDistance(const Vector3D &From) const;
  double GetAngleTo(const Vector3D &Pos, const Vector3D &ZeroAngleDir) const;
  double GetAngleToTraversal(const Vector3D &Pos, const Vector3D &ZeroAngleDirTraversal) const;
  Vector3D GetAxialProjection(const Vector3D &From) const;
  Vector3D GetTraversalProjection(const Vector3D &From) const;
  BOOL IsColinearToLine(const Vector3D &V) {return IsColinear(Axial, V);}
  void RoundToInt();
};

//==============================================================================
/**
Represents plane by normalized normal vector and one point on plane
*/
class Plane
{
  Vector3D Normal;
  Vector3D Point;
  double D;
public:
//TO_IMPROVE!!! Add constructors in different forms
  Plane(const Vector3D &aNormal, const Vector3D &aPoint);
  Plane(const Vector3D &Point1, const Vector3D &Point2, const Vector3D &Point3);
  Plane(const Plane &aPlane);
  double GetDistance(const Vector3D &From) const;
  const Vector3D &GetNormal() const {return Normal;}
  void SetNormal(const Vector3D &NewNormal);
  const Vector3D &GetPoint() const {return Point;}
  void SetPoint(const Vector3D &NewPoint);
  void ShiftPoint(const Vector3D &Shift);
  void RoundToInt();
};

ERR GetCrossectionPointPlaneLine(const Plane &plane, const Line &line, Vector3D &Res);
//==============================================================================

/**
Returns minimal or maximal value of projection on Axis for array of vectors
*/
double GetMinInDirection(const vector<const Vector3D *> &Vectors, const Vector3D &Dir);
double GetMaxInDirection(const vector<const Vector3D *> &Vectors, const Vector3D &Dir);

//==============================================================================
/**
* Represents Miller index for crystallographic planes
*/
class MillerIndex
{
public:
  static const MillerIndex Zero;
  int h, k, l;
  MillerIndex();
  MillerIndex(int ah, int ak, int al);
  MillerIndex(const MillerIndex &m2);

  void Set(int ah, int ak, int al);
  BOOL IsZeroZero() const;
  BOOL IsZero() const;
  BOOL IsEmpty() const;

  MillerIndex &operator = (const MillerIndex &m2);
  BOOL operator == (const MillerIndex &m2);
};


string AsString(const MillerIndex &Miller);
ERR FromString(MillerIndex &Miller, const string &SMillerIndex);
//==============================================================================

BOOL IsInsideHexagon(double Side, const Vector3D &Point);
Vector3D GetAxisVector(const string &Axis);


#endif
