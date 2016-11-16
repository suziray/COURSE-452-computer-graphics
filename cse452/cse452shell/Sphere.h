#pragma once
#include "Shape.h"
#include "vecmath\Vector3.h"
#include "vecmath\Matrix3.h"
#include "../UIInterface.h"
#include "../cse452.h"
#include <FL/Fl_Window.H>
#include <vector>
#include <cmath>

class Sphere :
	public Shape
{
public:
	Sphere();
	~Sphere();
	void Sphere::newTess(int, int);
	HitRecord Sphere::intersect(Point3, Vector3);
};

