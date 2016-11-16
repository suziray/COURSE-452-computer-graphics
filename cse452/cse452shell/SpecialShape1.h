#pragma once
#include "Shape.h"
#include "vecmath\Vector3.h"
#include "vecmath\Matrix3.h"
#include "../UIInterface.h"
#include "../cse452.h"
#include <FL/Fl_Window.H>
#include <vector>
#include <cmath>

class SpecialShape1 :
	public Shape
{
public:
	SpecialShape1();
	~SpecialShape1();
	void SpecialShape1::newTess(int, int);
	HitRecord SpecialShape1::intersect(Point3, Vector3);
};

