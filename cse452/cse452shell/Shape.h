#pragma once
#include "vecmath\Vector3.h"
#include "vecmath\Matrix3.h"
#include <vector>
#include "intersection\HitRecord.h"

using namespace std;

struct Triangle
{
	Vector3 v1;
	Vector3 v2;
	Vector3 v3;
	Vector3 n1;
	Vector3 n2;
	Vector3 n3;
};


class Shape
{
public:
	Shape();
	~Shape();
	vector<Triangle> ts;
	void addTri(Vector3, Vector3, Vector3, Vector3);
	void addTri(Vector3, Vector3, Vector3);
	void drawTriangles();
	void bally();
	void sharpy();
	void ellipsy(double, double);
	Vector3 elly(double, double, Vector3);
	Matrix3 rotater(double);
	virtual void newTess(int, int) = 0;
	virtual HitRecord intersect(Point3, Vector3) = 0;
};

