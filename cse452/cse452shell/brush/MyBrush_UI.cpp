#include "cse452.h"
#include "ScreenPoint.h"
#include "BrushInterface.h"
#include <FL/Fl.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/fl_draw.H>
#include <FL/gl.h>
#include <cstring>
#include <cmath>

MyBrush::MyBrush()
{
	isMouseDown = false;

	imageWidth = screenWidth = 0;
	imageHeight = screenHeight = 0;

	// initialize your data here
}

MyBrush::~MyBrush() {
	// destroy your data here
}

void MyBrush::resize(int width, int height) {
	screenWidth = width;
	screenHeight = height;

	// First time initialization
	if (imageWidth == 0) {
		imageWidth = screenWidth;
		imageHeight = screenHeight;

		// Make image black
		pixelData.resize(width * height * 3, 0);
	}
}

void MyBrush::loadImage(Fl_Image* image) {
	imageWidth = image->w();
	imageHeight = image->h();
	// Reset viewport
	resize(screenWidth, screenHeight);
	pixelData.resize(imageWidth * imageHeight * 3, 0);

	// OpenGL's windows are reversed in y
	const int delta = imageWidth * 3;
	unsigned char* src = (unsigned char*)*image->data();
	for (int i = 0; i < imageHeight; i++) {
		// Ok, this is ugly
		unsigned char* dest = &pixelData[((imageHeight - 1 - i) * imageWidth * 3)];
		memcpy(dest, src, delta);
		src += delta;
	}
}

void MyBrush::draw() {
	// Set up camera for drawing
	setup2DDrawing(Color(0, 0, 0), screenWidth, screenHeight);

	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Draw a border around the actual image
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2i(0, 0);
	glVertex2i(imageWidth + 1, 0);
	glVertex2i(imageWidth + 1, imageHeight + 1);
	glVertex2i(0, imageHeight + 1);
	glEnd();


	glRasterPos2i(0, 0);
	// Copy data into window
	//for ( int iX = 0; iX < 100; iX++ )
	//putPixel( iX, iX, Color(1,0,0) );

	glDrawPixels(imageWidth, imageHeight, GL_RGB, GL_UNSIGNED_BYTE, &pixelData[0]);

	// These 5 lines draw a white line across your canvas
	// Remove this and replace it with intelligent OpenGL preview code

	const int r = brushUI->getRadius();
	const Color c = brushUI->getColor();
	glColor3f(c[0], c[1], c[2]);
	int x1 = mouseDrag[0], y1 = mouseDrag[1];
	// Add in your OpenGL pre-view code here
	if (brushUI->getToolType() == TOOL_BRUSH){

		glBegin(GL_POINTS);
		for (int x = 0; x < r / sqrt(2); ++x){
			int y = sqrt(r*r - x*x);
			glVertex2i(x1 + x, y1 + y);
			glVertex2i(x1 - x, y1 + y);
			glVertex2i(x1 + x, y1 - y);
			glVertex2i(x1 - x, y1 - y);
			glVertex2i(x1 + y, y1 + x);
			glVertex2i(x1 - y, y1 + x);
			glVertex2i(x1 + y, y1 - x);
			glVertex2i(x1 - y, y1 - x);
		}
		glEnd();
	}
	// display draw in progress (mouse is down)
	if (isMouseDown) {
		int x0 = mouseDown[0], y0 = mouseDown[1];

		if (brushUI->getToolType() == TOOL_LINE){
			vector<vector<int>> vx = vector<vector<int>>(); // the vertices of the rectangle
			vx.resize(2, vector<int>(4, 0));
			double dy = (double)(y1 - y0) / sqrt((x1 - x0)*(x1 - x0) + (y1 - y0)*(y1 - y0));
			double dx = (double)(x1 - x0) / sqrt((x1 - x0)*(x1 - x0) + (y1 - y0)*(y1 - y0));
			vx[0][0] = x0 - dy * r / 2.0;
			vx[0][1] = x0 + dy * r / 2.0;
			vx[0][2] = x1 - dy * r / 2.0;
			vx[0][3] = x1 + dy * r / 2.0;
			vx[1][0] = y0 + dx * r / 2.0;
			vx[1][1] = y0 - dx * r / 2.0;
			vx[1][2] = y1 + dx * r / 2.0;
			vx[1][3] = y1 - dx * r / 2.0;

			glBegin(GL_POLYGON);
			glVertex2i(vx[0][0], vx[1][0]);
			glVertex2i(vx[0][1], vx[1][1]);
			glVertex2i(vx[0][3], vx[1][3]);
			glVertex2i(vx[0][2], vx[1][2]);
			glEnd();
		}
		else if (brushUI->getToolType() == TOOL_CIRCLE){
			double radius = sqrt((x0 - x1)*(x0 - x1) + (y0 - y1)*(y0 - y1));
			glBegin(GL_LINES);
			for (int y = 0; y < radius - r / 2; ++y){
				double xl = sqrt((radius - r / 2)*(radius - r / 2) - y*y);
				double xr = sqrt((radius + r / 2)*(radius + r / 2) - y*y);
				glVertex2i(x0 + xr, y0 + y);
				glVertex2i(x0 + xl, y0 + y);
				glVertex2i(x0 - xr, y0 + y);
				glVertex2i(x0 - xl, y0 + y);
				glVertex2i(x0 + xr, y0 - y);
				glVertex2i(x0 + xl, y0 - y);
				glVertex2i(x0 - xr, y0 - y);
				glVertex2i(x0 - xl, y0 - y);
			}
			glEnd();
			glBegin(GL_LINES);
			for (int y = radius - r / 2; y < radius + r / 2; ++y){
				double xr = sqrt((radius + r / 2)*(radius + r / 2) - y*y);
				glVertex2i(x0 + xr, y0 + y);
				glVertex2i(x0, y0 + y);
				glVertex2i(x0 + xr, y0 - y);
				glVertex2i(x0, y0 - y);
				glVertex2i(x0 - xr, y0 + y);
				glVertex2i(x0, y0 + y);
				glVertex2i(x0 - xr, y0 - y);
				glVertex2i(x0, y0 - y);
			}
			glEnd();
		}
		else if (brushUI->getToolType() == TOOL_POLYGON){
			/*vector<vector<int>> vertices;
			while (!Fl::event_button3() && !Fl::event_state(FL_SHIFT)){

			}*/
		}
	}
	endDrawing();
}

// This does pixel flow
void MyBrush::draw_callback(void *in_data)
{
	MyBrush *opMe = static_cast<MyBrush *>(in_data);

	// Repeat the time out if we're not done yet
	if (opMe->isMouseDown == true) {
		opMe->drawBrush();

		Fl::repeat_timeout(0.05, MyBrush::draw_callback, (void *)opMe);

		RedrawWindow();
	}
}


int MyBrush::handle(int event) {
	// OpenGL & FLTK's y axes are oriented differently
	const ScreenPoint pt = ScreenPoint(Fl::event_x(), screenHeight - 1 - Fl::event_y());

	switch (event) {
	case FL_PUSH: {
					  mouseDrag = pt;
					  mouseDown = pt;

					  if (brushUI->getToolType() == TOOL_POLYGON) {
						  if (isMouseDown == true) {
							  polygon.push_back(mouseDrag);
						  }
						  else {
							  isMouseDown = true;
							  polygon.resize(0);
							  polygon.push_back(mouseDrag);
						  }
					  }
					  else {
						  isMouseDown = true;
						  if (brushUI->getToolType() == TOOL_BRUSH)
							  Fl::add_timeout(0, draw_callback, this);
					  }
					  return 1;
	}
	case FL_DRAG: mouseDrag = pt; RedrawWindow(); return 1;
	case FL_MOVE:
		mouseDrag = pt;
		if (brushUI->getToolType() == TOOL_BRUSH || (brushUI->getToolType() == TOOL_POLYGON && isMouseDown))
			RedrawWindow();
		return 1;
	case FL_RELEASE: {
						 mouseDrag = pt;
						 if (brushUI->getToolType() != TOOL_POLYGON) {
							 isMouseDown = false;
							 switch (brushUI->getToolType()) {
							 case TOOL_BRUSH:
								 break;
							 case TOOL_LINE:
								 drawLine();
								 break;
							 case TOOL_CIRCLE:
								 drawCircle();
								 break;
							 case TOOL_FILTER:
								 filterRegion();
								 break;
							 default: break;
							 }
						 }
						 else if (Fl::event_button3() || Fl::event_state(FL_SHIFT)) {
							 isMouseDown = false;
							 drawPolygon();
						 }
						 RedrawWindow();
						 return 1;
	}
	default: return 0;
	}
}
