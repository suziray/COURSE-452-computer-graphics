#pragma once
#include "Shape.h"
#include "vecmath\Vector3.h"
#include "vecmath\Matrix3.h"
#include "../UIInterface.h"
#include "../cse452.h"
#include <FL/Fl_Window.H>
#include <vector>
#include <cmath>

class Cylinder : public Shape
{
public:
	Cylinder();
	~Cylinder();
	void Cylinder::newTess(int, int);
	HitRecord Cylinder::intersect(Point3, Vector3);
};

