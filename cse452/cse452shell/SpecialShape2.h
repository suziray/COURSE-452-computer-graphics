#pragma once
#include "Shape.h"
#include "vecmath\Vector3.h"
#include "vecmath\Matrix3.h"
#include "../UIInterface.h"
#include "../cse452.h"
#include <FL/Fl_Window.H>
#include <vector>
#include <cmath>

class SpecialShape2 :
	public Shape
{
public:
	SpecialShape2();
	~SpecialShape2();
	void SpecialShape2::newTess(int, int);
	HitRecord SpecialShape2::intersect(Point3, Vector3);
};

