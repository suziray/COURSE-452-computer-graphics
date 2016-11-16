#include "Cube.h"

Cube::Cube()
{
}

Cube::~Cube()
{
}

void Cube::newTess(int t1, int t2)
{
	ts = vector<Triangle>();
	double w = 1.0 / t1;
	double h = 1.0 / t2;

	for (int i = 0; i < t1; i++){
		for (int j = 0; j < t2; j++){
			Vector3 nor = Vector3(1, 0, 0); // front side
			addTri(Vector3(.5, .5 - j*h, .5 - i*w), Vector3(.5, .5 - (j + 1)*h, .5 - i*w), Vector3(.5, .5 - j*h, .5 - (i + 1)*w), nor);
			addTri(Vector3(.5, .5 - j*h, .5 - (i + 1)*w), Vector3(.5, .5 - (j + 1)*h, .5 - i*w), Vector3(.5, .5 - (j + 1)*h, .5 - (i + 1)*w), nor);

			nor = Vector3(0, 0, -1); // right side
			addTri(Vector3(.5 - i*w, .5 - j*h, -.5), Vector3(.5 - i*w, .5 - (j + 1)*h, -.5), Vector3(.5 - (i + 1)*w, .5 - j*h, -.5), nor);
			addTri(Vector3(.5 - (i + 1)*w, .5 - j*h, -.5), Vector3(.5 - i*w, .5 - (j + 1)*h, -.5), Vector3(.5 - (i + 1)*w, .5 - (j + 1)*h, -.5), nor);

			nor = Vector3(-1, 0, 0); // back side
			addTri(Vector3(-.5, .5 - j*h, -.5 + i*w), Vector3(-.5, .5 - (j + 1)*h, -.5 + i*w), Vector3(-.5, .5 - j*h, -.5 + (i + 1)*w), nor);
			addTri(Vector3(-.5, .5 - j*h, -.5 + (i + 1)*w), Vector3(-.5, .5 - (j + 1)*h, -.5 + i*w), Vector3(-.5, .5 - (j + 1)*h, -.5 + (i + 1)*w), nor);

			nor = Vector3(0, 0, 1); // left side
			addTri(Vector3(-.5 + i*w, .5 - j*h, .5), Vector3(-.5 + i*w, .5 - (j + 1)*h, .5), Vector3(-.5 + (i + 1)*w, .5 - j*h, .5), nor);
			addTri(Vector3(-.5 + (i + 1)*w, .5 - j*h, .5), Vector3(-.5 + i*w, .5 - (j + 1)*h, .5), Vector3(-.5 + (i + 1)*w, .5 - (j + 1)*h, .5), nor);

			nor = Vector3(0, 1, 0); // up side
			addTri(Vector3(-.5 + j*h, .5, .5 - i*w), Vector3(-.5 + (j + 1)*h, .5, .5 - i*w), Vector3(-.5 + j*h, .5, .5 - (i + 1)*w), nor);
			addTri(Vector3(-.5 + j*h, .5, .5 - (i + 1)*w), Vector3(-.5 + (j + 1)*h, .5, .5 - i*w), Vector3(-.5 + (j + 1)*h, .5, .5 - (i + 1)*w), nor);

			nor = Vector3(0, -1, 0); // down side
			addTri(Vector3(.5 - j*h, -.5, .5 - i*w), Vector3(.5 - (j + 1)*h, -.5, .5 - i*w), Vector3(.5 - j*h, -.5, .5 - (i + 1)*w), nor);
			addTri(Vector3(.5 - j*h, -.5, .5 - (i + 1)*w), Vector3(.5 - (j + 1)*h, -.5, .5 - i*w), Vector3(.5 - (j + 1)*h, -.5, .5 - (i + 1)*w), nor);
		}
	}
}

HitRecord Cube::intersect(Point3 p, Vector3 d){
	HitRecord hr = HitRecord();
	double t = (.5 - p[0]) / d[0];
	Point3 q = p + t * d;
	if (t >= 0 && abs(q[1]) < .5 && abs(q[2]) < .5){
		hr.addHit(t, 0, 0, q, Vector3(1, 0, 0));
	}

	t = (-.5 - p[0]) / d[0];
	q = p + t * d;
	if (t >= 0 && abs(q[1]) < .5 && abs(q[2]) < .5){
		hr.addHit(t, 0, 0, q, Vector3(-1, 0, 0));
	}

	t = (.5 - p[1]) / d[1];
	q = p + t * d;
	if (t >= 0 && abs(q[0]) < .5 && abs(q[2]) < .5){
		hr.addHit(t, 0, 0, q, Vector3(0, 1, 0));
	}

	t = (-.5 - p[1]) / d[1];
	q = p + t * d;
	if (t >= 0 && abs(q[0]) < .5 && abs(q[2]) < .5){
		hr.addHit(t, 0, 0, q, Vector3(0, -1, 0));
	}

	t = (.5 - p[2]) / d[2];
	q = p + t * d;
	if (t >= 0 && abs(q[0]) < .5 && abs(q[1]) < .5){
		hr.addHit(t, 0, 0, q, Vector3(0, 0, 1));
	}

	t = (-.5 - p[2]) / d[2];
	q = p + t * d;
	if (t >= 0 && abs(q[0]) < .5 && abs(q[1]) < .5){
		hr.addHit(t, 0, 0, q, Vector3(0, 0, -1));
	}

	hr.sortHits();
	return hr;
}