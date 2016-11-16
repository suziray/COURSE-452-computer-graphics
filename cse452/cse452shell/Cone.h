#pragma once
#include "Shape.h"
#include "vecmath\Vector3.h"
#include "vecmath\Matrix3.h"
#include "../UIInterface.h"
#include "../cse452.h"
#include <FL/Fl_Window.H>
#include <vector>
#include <cmath>

class Cone :
	public Shape
{
public:
	Cone();
	~Cone();
	void Cone::newTess(int, int);
	HitRecord Cone::intersect(Point3, Vector3);
};

