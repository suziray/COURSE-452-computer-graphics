#include "SpecialShape1.h"

SpecialShape1::SpecialShape1()
{
}

SpecialShape1::~SpecialShape1()
{
}

void SpecialShape1::newTess(int t1, int t2)
{
	ts = vector<Triangle>();

	double r = 2 * M_PI / 5;
	Matrix3 m = rotater(r); // for rotation
	Matrix3 m1 = rotater(r / 2);
	double e = sqrt(1.0 - 1.0 / (4 * sin(M_PI / 5) * sin(M_PI / 5))); // length of the icosahedron's edge
	Vector3 vU = Vector3(0, .5, 0);
	Vector3 vU1 = Vector3(e / 2 / sin(M_PI / 5), .5 - e*e, 0);
	Vector3 vL = Vector3(0, -.5, 0);
	Vector3 vL1 = m1 * Vector3(e / 2 / sin(M_PI / 5), e*e - .5, 0);
	for (int i = 0; i < 5; ++i){
		addTri(vU, vU1, m*vU1);
		addTri(vL1, vL, m*vL1);
		addTri(vU1, vL1, m*vU1);
		addTri(m*vU1, vL1, m*vL1);
		vU1 = m * vU1;
		vL1 = m * vL1;
	}
	for (int i = 1; i < min(4, t1); ++i){
		bally();
	}
	sharpy();
}

HitRecord SpecialShape1::intersect(Point3 p, Vector3 d){
	HitRecord hr = HitRecord();

	return hr;
}