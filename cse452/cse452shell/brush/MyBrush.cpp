#include "cse452.h"
#include "MyBrush.h"
#include "BrushInterface.h"
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

void MyBrush::changedBrush() {
	// this is called anytime the brush type or brush radius changes
	// it should recompute the brush mask appropriately
	const int radius = brushUI->getRadius();
	const int type = brushUI->getBrushType();
	mask = vector<vector<double>>();
	mask.resize(radius + 1, vector<double>(radius + 1, 0));
	if (type == 0){ // CONSTANT
		for (int i = 0; i < radius + 1; ++i){
			for (int j = 0; j < radius + 1; ++j){
				if (radius > sqrt(i*i + j*j)){
					mask[i][j] = 1;
				}
			}
		}
	}
	else if (type == 1){ // LINEAR
		for (int i = 0; i < radius + 1; ++i){
			for (int j = 0; j < radius + 1; ++j){
				if (radius > sqrt(i*i + j*j)){
					mask[i][j] = (radius - sqrt(i*i + j*j)) / radius;
				}
			}
		}
	}
	else if (type == 2){ // QUADRATIC
		for (int i = 0; i < radius + 1; ++i){
			for (int j = 0; j < radius + 1; ++j){
				if (radius > sqrt(i*i + j*j)){
					mask[i][j] = 1 - (sqrt(i*i + j*j) / radius)*(sqrt(i*i + j*j) / radius);
				}
			}
		}
	}
	else if (type == 3){ // GAUSSIAN
		for (int i = 0; i < radius + 1; ++i){
			for (int j = 0; j < radius + 1; ++j){
				if (radius > sqrt(i*i + j*j)){
					double x = sqrt(i*i + j*j) / radius;
					mask[i][j] = exp(-x*x / 2.0) / sqrt(2 * M_PI);
				}
			}
		}
	}
	else if (type == 4){ // SPECIAL
		for (int i = 0; i < radius + 1; ++i){
			for (int j = 0; j < radius + 1; ++j){
				double x = sqrt(i*i + j*j);
				if (radius / 2.0 > x){
					mask[i][j] = 1 - pow(abs(x - radius / 4) / (radius / 4), 1.5);
				}
				else if (radius > x && x > radius / 2.0){
					mask[i][j] = 1 - pow(abs(x - radius * 3 / 4) / (radius  / 4), 1.5);
				}
			}
		}
	}

}

void MyBrush::drawBrush() {
	// apply the current brush mask to image location (x,y)
	// the mouse location is in mouseDrag

	const int radius = brushUI->getRadius();
	const float pixelFlow = brushUI->getFlow();
	const Color colBrush = brushUI->getColor();
	int x = mouseDrag[0];
	int y = mouseDrag[1];
	int xUBound = min(x + radius, 800);
	int yUBound = min(y + radius, 600);
	int xLBound = max(x - radius, 0);
	int yLBound = max(y - radius, 0);
	for (int i = xLBound; i < xUBound; ++i){
		for (int j = yLBound; j < yUBound; ++j){
			int ii = abs(i - x);
			int jj = abs(j - y);
			Color c = getPixel(i, j);
			vector<double> r = deposit(c, colBrush, pixelFlow * mask[ii][jj]);
			putPixel(i, j, Color(r[0], r[1], r[2]));
		}
	}
}

vector<double> MyBrush::deposit(Color c, Color b, double m) {
	//deposit onto color c with brush color b and mask+flow factor m
	vector<double> r;
	for (int i = 0; i < 3; ++i){
		if (c[i] * (1 - m) + b[i] * m > 1){
			r.push_back(1);
		}
		else r.push_back(c[i] * (1 - m) + b[i] * m);
	}
	return r;
}

void MyBrush::drawLine() {
	// draw a thick line from mouseDown to mouseDrag
	// the width of the line is given by the current brush radius
	const int radius = brushUI->getRadius();
	const Color colBrush = brushUI->getColor();
	int x0 = mouseDown[0];
	int y0 = mouseDown[1];
	int x1 = mouseDrag[0];
	int y1 = mouseDrag[1];

	double dy = (double)(y1 - y0) / sqrt((x1 - x0)*(x1 - x0) + (y1 - y0)*(y1 - y0));
	double dx = (double)(x1 - x0) / sqrt((x1 - x0)*(x1 - x0) + (y1 - y0)*(y1 - y0));
	vx = vector<vector<int>>(); // the vertices
	vx.resize(2, vector<int>(4, 0));
	vx[0][0] = x0 - dy * radius / 2.0;
	vx[0][1] = x0 + dy * radius / 2.0;
	vx[0][2] = x1 - dy * radius / 2.0;
	vx[0][3] = x1 + dy * radius / 2.0;
	vx[1][0] = y0 + dx * radius / 2.0;
	vx[1][1] = y0 - dx * radius / 2.0;
	vx[1][2] = y1 + dx * radius / 2.0;
	vx[1][3] = y1 - dx * radius / 2.0;

	vector<int> ys = sort(vx[1]);
	int yMin = ys[0], yMax = ys[3];
	edge = vector<vector<int>>(); // the edges
	edge.resize(4, vector<int>(4, 0));
	edge[0] = { vx[0][0], vx[1][0], vx[0][1], vx[1][1] };
	edge[1] = { vx[0][0], vx[1][0], vx[0][2], vx[1][2] };
	edge[2] = { vx[0][3], vx[1][3], vx[0][2], vx[1][2] };
	edge[3] = { vx[0][3], vx[1][3], vx[0][1], vx[1][1] };
	yMin = max(0, yMin);
	yMax = min(600, yMax + 2);
	for (int i = yMin; i < yMax; ++i){
		vector<int> intersec;
		for (int j = 0; j < 4; ++j){
			if (i <= max(edge[j][1], edge[j][3]) && i > min(edge[j][1], edge[j][3])) {
				intersec.push_back(scanline(edge[j], i));
			}
		}
		if (intersec.size() == 1) putPixel(intersec[0], i, colBrush);
		if (intersec.size() > 1) {
			vector<int> sortedIntersec = sort(intersec);
			int leftLimit = max(0, sortedIntersec[0]);
			int rightLimit = min(800, sortedIntersec[1] + 1);
			for (int k = leftLimit; k < rightLimit; k++){
				putPixel(k, i, colBrush);
			}
		}
	}
}

int MyBrush::scanline(vector<int> edge, int y){
	return edge[0] + (double)(edge[2] - edge[0]) / (edge[3] - edge[1]) * (y - edge[1]);
}

vector<int> MyBrush::sort(vector<int> n){
	vector<int> sorted;
	while (n.size() > 1){
		int m = 10000000, l;
		for (int i = 0; i < n.size(); i++){
			if (m > n[i]) {
				m = n[i];
				l = i;
			}
		}
		n.erase(n.begin() + l);
		sorted.push_back(m);
	}
	sorted.push_back(n[0]);
	return sorted;
}

void MyBrush::drawCircle() {
	// draw a thick circle at mouseDown with radius r
	// the width of the circle is given by the current brush radius
	const Color colBrush = brushUI->getColor();
	const int width = brushUI->getRadius();
	int x0 = mouseDown[0];
	int y0 = mouseDown[1];
	int x1 = mouseDrag[0];
	int y1 = mouseDrag[1];
	double r = sqrt((x0 - x1)*(x0 - x1) + (y0 - y1)*(y0 - y1));
	for (int i = max(0, y0 - r - width / 2.0); i < min(600, y0 + r + width / 2.0); ++i){
		for (int j = max(0, x0 - r - width / 2.0); j < min(800, x0 + r + width / 2.0); ++j){
			int d = sqrt((i - y0)*(i - y0) + (j - x0)*(j - x0));
			if (d > r - width / 2 && d < r + width / 2){
				putPixel(j, i, colBrush);
			}
		}
	}
}

void MyBrush::drawPolygon() {
	// draw a polygon with numVertices whos coordinates are stored in the
	// polygon array: {x0, y0, x1, y1, ...., xn-1, yn-1}
	const Color colBrush = brushUI->getColor();
	vector<int> ys;
	int n = polygon.size();
	for (int i = 0; i < n; ++i){
		ys.push_back(polygon[i][1]);
	}
	vector<int> sortedYs = sort(ys);
	int yMin = max(0, sortedYs[0]);
	int yMax = min(600, sortedYs[n - 1]);

	// initiate the edges
	edges.resize(n, vector<int>(4, 0));
	for (int i = 0; i < n - 1; ++i){
		edges[i] = { polygon[i][0], polygon[i][1], polygon[i + 1][0], polygon[i + 1][1] };
	}
	edges[n - 1] = { polygon[n - 1][0], polygon[n - 1][1], polygon[0][0], polygon[0][1] };

	for (int i = yMin + 1; i < yMax; ++i){
		vector<int> intersec;
		for (int j = 0; j < n; ++j){
			if (i > min(edges[j][1], edges[j][3]) && i <= max(edges[j][1], edges[j][3]))
				intersec.push_back(scanline(edges[j], i));
		}
		if (intersec.size() == 1) putPixel(intersec[0], i, colBrush);
		vector<int> sortedIntersec = sort(intersec);
		if (intersec.size() % 2 == 0 && intersec.size() > 1) { // even intersections
			for (int k = 0; k < intersec.size() / 2.0; k += 2){
				for (int l = sortedIntersec[k]; l < sortedIntersec[k + 1] + 1; ++l){
					putPixel(l, i, colBrush);
				}
			}
		}
		else if (intersec.size() % 2 == 1){
			for (int k = 0; k < sortedIntersec.size() / 2.0; k += 2){
				for (int l = sortedIntersec[k]; l < sortedIntersec[k + 1] + 1; ++l){
					putPixel(l, i, colBrush);
				}
				putPixel(sortedIntersec[intersec.size()], i, colBrush);
			}
		}
	}
}

void MyBrush::filterRegion() {
	// apply the filter indicated by filterType to the square
	// defined by the two corner points mouseDown and mouseDrag
	// these corners are not guarenteed to be in any order
	// The filter width is given by the brush radius
	const int filterType = brushUI->getFilterType();
	int n = 31;
	filter = vector<vector<double>>();
	filter.resize(n, vector<double>(n, 0));
	if (filterType == 0){ // blur
		int sum = 0;
		for (int i = 0; i < n; ++i){
			for (int j = 0; j < n; ++j){
				filter[i][j] = 1 - (double)((i - (n + 1) / 2)* (i - (n + 1) / 2) + (j - (n + 1) / 2)* (j - (n + 1) / 2)) / (n / 2);
				sum += filter[i][j];
			}
		}
		for (int i = 0; i < n; ++i){
			for (int j = 0; j < n; ++j){
				filter[i][j] = filter[i][j] / sum;
			}
		}
	}
	else if (filterType == 1){ // 

	}
	cout << "ok" << endl;

	int x0 = mouseDown[0];
	int y0 = mouseDown[1];
	int x1 = mouseDrag[0];
	int y1 = mouseDrag[1];
	for (int i = min(x0, x1); i < max(x0, x1) + 1; ++i){
		for (int j = min(y0, y1); j < max(y0, y1) + 1; ++j){
			filterMatrix(i, j, filter);
		}
	}
}

void MyBrush::filterMatrix(int x, int y, vector<vector<double>> filter){
	int n = filter.size();
	int c0 = getPixel(x, y)[0];
	int c1 = getPixel(x, y)[1];
	int c2 = getPixel(x, y)[2];
	cout << n << " " << c0 << " " << c1 << " " << c2 << endl;
	c0 = c0 * filter[(n + 1) / 2][(n + 1) / 2];
	c0 = c0 * filter[(n + 1) / 2][(n + 1) / 2];
	c0 = c0 * filter[(n + 1) / 2][(n + 1) / 2];
	for (int i = 0; i < n; ++i){
		for (int j = 0; j < n; ++j){
			Color c = getPixel(x + i - n / 2, y + j - n / 2);
			c0 += c[0] * filter[i][j];
			c1 += c[1] * filter[i][j];
			c2 += c[2] * filter[i][j];
		}
	}
	putPixel(x, y, Color(c0, c1, c2));
}