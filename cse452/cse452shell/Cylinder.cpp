#include "Cylinder.h"

using namespace std;

Cylinder::Cylinder()
{
}

Cylinder::~Cylinder()
{
}

void Cylinder::newTess(int t1, int t2)
{
	ts = vector<Triangle>();
	double h = 1.0 / t2;
	double r = 2 * M_PI / t1;
	Matrix3 m = rotater(r); // for rotation
	Vector3 vU = Vector3(.5, .5, 0);
	Vector3 vL = Vector3(.5, -.5, 0);
	Vector3 nM = Vector3(cos(r / 2.0), 0, -sin(r / 2.0));
	for (int i = 0; i < t1; i++){
		Vector3 vU1 = m*vU;
		Vector3 vL1 = m*vL;	
		Vector3 nor = Vector3(0, 1, 0); // up side
		addTri(Vector3(vU[0], vU[1], vU[2]), Vector3(vU1[0], vU1[1], vU1[2]), Vector3(0, 0.5, 0), nor);
		nor = Vector3(0, -1, 0); // down side
		addTri(Vector3(vL1[0], vL1[1], vL1[2]), Vector3(vL[0], vL[1], vL[2]), Vector3(0, -0.5, 0), nor);
		nor = nM;
		for (int j = 0; j < t2; j++){
			addTri(Vector3(vL[0], vL[1] + j*h, vL[2]), Vector3(vL1[0], vL1[1] + j*h, vL1[2]), Vector3(vL1[0], vL1[1] + (j + 1)*h, vL1[2]), nor);
			addTri(Vector3(vL[0], vL[1] + j*h, vL[2]), Vector3(vL1[0], vL1[1] + (j + 1)*h, vL1[2]), Vector3(vL[0], vL[1] + (j+1)*h, vL[2]), nor);
		}
		vU = vU1;
		vL = vL1;
		nM = m*nM;
	}
}

HitRecord Cylinder::intersect(Point3 p, Vector3 d){
	HitRecord hr = HitRecord();
	//top cap check
	double t = (.5 - p[1]) / d[1];
	Point3 q = p + t * d;
	if (t >= 0 && q[0] * q[0] + q[2] * q[2] <= .25){
		Vector3 n = Vector3(0, 1, 0);
		n.normalize();
		hr.addHit(t, 0, 0, q, n);
	}

	//bottom cap check
	t = (-.5 - p[1]) / d[1];
	q = p + t * d;
	if (t >= 0 &&  q[0] * q[0] + q[2] * q[2] <= .25){
		Vector3 n = Vector3(0, -1, 0);
		n.normalize();
		hr.addHit(t, 0, 0, q, n);
	}

	//body check
	double A = d[0] * d[0] + d[2] * d[2];
	double B = 2 * (p[0] * d[0] + p[2] * d[2]);
	double C = p[0] * p[0] + p[2] * p[2] - .25;
	double D = B * B - 4 * A * C;
	if (D >= 0){ // at least one intersection with body
		double t1 = (-B + sqrt(D)) / (2 * A);
		Point3 q = p + t1 * d;
		if (t1 >= 0 && q[1] < .5 && q[1] > -.5){
			Vector3 n1 = Vector3(q[0], 0, q[2]);
			n1.normalize();
			hr.addHit(t1, 0, 0, q, n1);
		}
		if (D > 0){ // one more intersection
			double t2 = (-B - sqrt(D)) / (2 * A);
			Point3 q = p + t2 * d;
			if (t2 >= 0 && q[1] < .5 && q[1] > -.5){
				Vector3 n2 = Vector3(q[0], 0, q[2]);
				n2.normalize();
				hr.addHit(t2, 0, 0, q, n2);
			}
		}
	}

	hr.sortHits();
	return hr;
}