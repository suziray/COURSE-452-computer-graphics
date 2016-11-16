#include "SpecialShape2.h"

SpecialShape2::SpecialShape2()
{
}

SpecialShape2::~SpecialShape2()
{
}

void SpecialShape2::newTess(int t1, int t2)
{
	ts = vector<Triangle>();
	
	double a = min(2, t1 / 10.0); //can be 1-10
	double b = min(1.5, t2 / 10.0); //can be 1-9
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
		addTri(elly(a, b, vU), elly(a, b, vU1), elly(a, b, m*vU1));
		addTri(elly(a, b, vL1), elly(a, b, vL), elly(a, b, m*vL1));
		addTri(elly(a, b, vU1), elly(a, b, vL1), elly(a, b, m*vU1));
		addTri(elly(a, b, m*vU1), elly(a, b, vL1), elly(a, b, m*vL1));
		vU1 = m * vU1;
		vL1 = m * vL1;
	}
	for (int i = 1; i < 6; ++i){
		ellipsy(a, b);
	}
}

HitRecord SpecialShape2::intersect(Point3 p, Vector3 d){
	HitRecord hr = HitRecord();
	//the implicit function is x^2 + 4*y^2 + z^2 - 1 = 0

	double A = d[0] * d[0] + d[2] * d[2] + 4 * d[1] * d[1];
	double B = 2 * (d[0] * p[0] + d[2] * p[2] + 4 * d[1] * p[1]);
	double C = p[0] * p[0] + p[2] * p[2] + 4 * p[1] * p[1] - 1;
	double D = B * B - 4 * A * C;
	if (D >= 0){ // at least one intersection
		double t1 = (-B + sqrt(D)) / (2 * A);
		if (t1 >= 0){
			Point3 q = p + t1 * d;
			Vector3 n = Vector3(2 * q[0], 8 * q[1], 2 * q[2]);
			n.normalize();
			hr.addHit(t1, 0, 0, q, n);
		}
		if (D > 0){ // one more intersection
			double t2 = (-B - sqrt(D)) / (2 * A);
			if (t2 >= 0){
				Point3 q = p + t2 * d;
				Vector3 n = Vector3(2 * q[0], 8 * q[1], 2 * q[2]);
				n.normalize();
				hr.addHit(t2, 0, 0, q, n);
			}
		}
	}

	hr.sortHits();
	return hr;
}