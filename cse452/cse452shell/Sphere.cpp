#include "Sphere.h"

using namespace std;

Sphere::Sphere()
{
}

Sphere::~Sphere()
{
}

void Sphere::newTess(int t1, int t2)
{
	ts = vector<Triangle>();
	double r = 2 * M_PI / 5;
	double r1 = r / 2;
	Matrix3 m = rotater(r); // for rotation
	Matrix3 m1 = rotater(r/2); // for rotation
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
	for (int i = 1; i < min(6, t1); ++i){
		bally();
	}
}

HitRecord Sphere::intersect(Point3 p, Vector3 d){
	HitRecord hr = HitRecord();
	double A = d.length() * d.length();
	double B = 2 * (p[0] * d[0] + p[1] * d[1] + p[2] * d[2]);
	double C = p.distanceFromOriginSquared() - .25;
	double D = B * B - 4 * A * C;
	if (D >= 0){ // at least one intersection
		double t1 = (-B + sqrt(D)) / (2 * A);
		if (t1 >= 0){
			Point3 q = p + t1 * d;
			Vector3 n = Vector3(q[0], q[1], q[2]);
			n.normalize();
			hr.addHit(t1, 0, 0, q, n);
		}
		if (D > 0){ // one more intersection
			double t2 = (-B - sqrt(D)) / (2 * A);
			if (t2 >= 0){
				Point3 q = p + t2 * d;
				Vector3 n = Vector3(q[0], q[1], q[2]);
				n.normalize();
				hr.addHit(t2, 0, 0, q, n);
			}
		}
	}
	hr.sortHits();
	return hr;
}