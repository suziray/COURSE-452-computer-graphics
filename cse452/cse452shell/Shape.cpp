#include "Shape.h"
#include "../Color.h"
#include <FL/Fl.H>
#include <FL/gl.h>
#include <GL/glu.h>
#include <vector>
#include "vecmath\Vector3.h"

using namespace std;

Shape::Shape()
{
}


Shape::~Shape()
{
}

void Shape::addTri(Vector3 vec1, Vector3 vec2, Vector3 vec3, Vector3 nor)
{
	Triangle t;
	t.v1 = vec1;
	t.v2 = vec2;
	t.v3 = vec3;
	t.n1 = nor;
	t.n2 = nor;
	t.n3 = nor;
	ts.push_back(t);
}

void Shape::addTri(Vector3 vec1, Vector3 vec2, Vector3 vec3)
{
	Triangle t;
	t.v1 = vec1;
	t.v2 = vec2;
	t.v3 = vec3;
	t.n1 = vec1;
	t.n2 = vec2;
	t.n3 = vec3;
	ts.push_back(t);
}

void Shape::drawTriangles()
{
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < ts.size(); ++i){
		glNormal3d(ts[i].n1[0], ts[i].n1[1], ts[i].n1[2]);
		glVertex3d(ts[i].v1[0], ts[i].v1[1], ts[i].v1[2]);
		glNormal3d(ts[i].n2[0], ts[i].n2[1], ts[i].n2[2]);
		glVertex3d(ts[i].v2[0], ts[i].v2[1], ts[i].v2[2]);
		glNormal3d(ts[i].n3[0], ts[i].n3[1], ts[i].n3[2]);
		glVertex3d(ts[i].v3[0], ts[i].v3[1], ts[i].v3[2]);
	}
	glEnd();
}

void Shape::bally()
{
	int n = ts.size();
	for (int i = 0; i < n; ++i){
		Vector3 v12 = (ts[i].v1 + ts[i].v2) / 2.0;
		Vector3 v23 = (ts[i].v2 + ts[i].v3) / 2.0;
		Vector3 v31 = (ts[i].v3 + ts[i].v1) / 2.0;
		v12 = .5*v12 / v12.length();
		v23 = .5*v23 / v23.length();
		v31 = .5*v31 / v31.length();
		addTri(ts[i].v1, v12, v31);
		addTri(ts[i].v2, v23, v12);
		addTri(ts[i].v3, v31, v23);
		addTri(v12, v23, v31);
	}
	ts.erase(ts.begin(), ts.begin() + n);
}

void Shape::ellipsy(double a, double b)
{
	int n = ts.size();
	for (int i = 0; i < n; ++i){
		Vector3 v12 = (ts[i].v1 + ts[i].v2) / 2.0;
		Vector3 v23 = (ts[i].v2 + ts[i].v3) / 2.0;
		Vector3 v31 = (ts[i].v3 + ts[i].v1) / 2.0;
		v12 = elly(a, b, v12);
		v23 = elly(a, b, v23);
		v31 = elly(a, b, v31);
		addTri(ts[i].v1, v12, v31);
		addTri(ts[i].v2, v23, v12);
		addTri(ts[i].v3, v31, v23);
		addTri(v12, v23, v31);
	}
	ts.erase(ts.begin(), ts.begin() + n);
}

void Shape::sharpy()
{
	int n = ts.size();
	for (int i = 0; i < n; ++i){
		Vector3 v0 = (ts[i].v1 + ts[i].v2 + ts[i].v3) / 3.0;
		v0 = .75*v0 / v0.length();
		addTri(ts[i].v1, ts[i].v2, v0, (ts[i].v1 - v0) ^ (ts[i].v2 - v0));
		addTri(ts[i].v2, ts[i].v3, v0, (ts[i].v2 - v0) ^ (ts[i].v3 - v0));
		addTri(ts[i].v3, ts[i].v1, v0, (ts[i].v3 - v0) ^ (ts[i].v1 - v0));
	}
	ts.erase(ts.begin(), ts.begin() + n);
}

Matrix3 Shape::rotater(double r)
{
	return  Matrix3(Vector3(cos(r), 0, sin(r)), Vector3(0, 1, 0), Vector3(-sin(r), 0, cos(r)));
}

Vector3 Shape::elly(double a, double b, Vector3 v)
{
	double t = asin(v[1] / v.length());
	double r = a*b / sqrt(a*a*sin(t)*sin(t) + b*b*cos(t)*cos(t));
	return r * v / v.length();
}