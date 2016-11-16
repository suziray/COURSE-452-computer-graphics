#include "Cone.h"

using namespace std;

Cone::Cone()
{
}

Cone::~Cone()
{
}

void Cone::newTess(int t1, int t2)
{
	ts = vector<Triangle>();
	double r = 2 * M_PI / t1;
	Matrix3 m = rotater(r); // for rotation
	Vector3 vU = Vector3(0, .5, 0);
	Vector3 vL = Vector3(.5, -.5, 0);
	Vector3 nM = Vector3(cos(r / 2.0), .5*sqrt(cos(r / 2.0)*cos(r / 2.0) + sin(r / 2.0)*sin(r / 2.0)), -sin(r / 2.0));
	for (int i = 0; i < t1; i++){
		Vector3 vL1 = m*vL;
		Vector3 nor = Vector3(0, -1, 0); // down side
		addTri(Vector3(vL1[0], vL1[1], vL1[2]), Vector3(vL[0], vL[1], vL[2]), Vector3(0, -0.5, 0), nor);
		nor = nM;
		Vector3 d0 = (vU - vL)/t2;
		Vector3 d1 = (vU - vL1)/t2;

		for (int j = 0; j < t2 - 1; j++){
			addTri(vL+j*d0, vL1+j*d1, vL1+(j+1)*d1, nor);
			addTri(vL + j*d0, vL1 + (j + 1)*d1, vL + (j+1)*d0, nor);
		}
		addTri(vU, vU - d0, vU - d1, nor);
		vL = vL1;
		nM = m*nM;
	}
}

HitRecord Cone::intersect(Point3 p, Vector3 d){
	HitRecord hr = HitRecord();
	//cap check
	double t = (-.5 - p[1]) / d[1];
	Point3 q = p + t * d;
	if (t >= 0 && q[0] * q[0] + q[2] * q[2] <= .25){
		hr.addHit(t, 0, 0, q, Vector3(0, -1, 0));
	}

	//body check
	double A = d[0] * d[0] + d[2] * d[2] - .25 * d[1] * d[1];
	double B = 2 * (p[0] * d[0] + p[2] * d[2]) + .5 * d[1] * (.5 - p[1]);
	double C = p[0] * p[0] + p[2] * p[2] - .25 * (.5 - p[1]) * (.5 - p[1]);
	double D = B * B - 4 * A * C;
	if (D >= 0){ // at least one intersection with body
		double t1 = (-B + sqrt(D)) / (2 * A);
		Point3 q = p + t1 * d;
		if (t1 >= 0 && q[1] < .5 && q[1] > -.5){
			Vector3 n1 = Vector3(q[0], .5 * sqrt(q[0]*q[0] + q[2]*q[2]), q[2]);
			n1.normalize();
			hr.addHit(t1, 0, 0, q, n1);
		}
		if (D > 0){ // one more intersection
			double t2 = (-B - sqrt(D)) / (2 * A);
			Point3 q = p + t2 * d;
			if (t2 >= 0 && q[1] < .5 && q[1] > -.5){
				Vector3 n2 = Vector3(q[0], .5 * sqrt(q[0] * q[0] + q[2] * q[2]), q[2]);
				n2.normalize();
				hr.addHit(t2, 0, 0, q, n2);
			}
		}
	}

	return hr;
}