#pragma once
#include "Shape.h"
#include "vecmath\Vector3.h"
#include <vector>

class Cube : public Shape
{
public:
	Cube();
	~Cube();
	void Cube::newTess(int, int);
	HitRecord Cube::intersect(Point3, Vector3);
};