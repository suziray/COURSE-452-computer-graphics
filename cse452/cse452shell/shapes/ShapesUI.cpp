#include "../cse452.h"
#include "ShapesUI.h"
#include "ShapesInterface.h"
#include "../vecmath/Vector3.h"
#include "../vecmath/Matrix3.h"
#include "../Color.h"
#include <FL/Fl.H>
#include <FL/gl.h>
#include <GL/glu.h>
#include <vector>

using namespace std;

ShapesUI::ShapesUI() {
    width = height = 0;

    // ToDo: initialize your variables here

}

ShapesUI::~ShapesUI() {
    // ToDo: delete your variables here
}

void ShapesUI::resize(int w, int h) {
    width = w;
    height = h;
}

void ShapesUI::draw() {
    // Sets up the viewport and background color
    setup3DDrawing( Color( 0,0,0 ), width, height, true );

    // Changes the way triangles are drawn
    switch ( shapesUI->getDisplayType() ) {
        case DISPLAY_WIREFRAME: {
            glDisable(GL_LIGHTING);
            glPolygonMode(GL_FRONT, GL_LINE);
            glColor3f(1.0f, 1.0f, 1.0f);
        } break;
        case DISPLAY_FLAT_SHADING: {
            glEnable(GL_LIGHTING);
            glPolygonMode(GL_FRONT, GL_FILL);
            glColor3f(1.0f, 1.0f, 1.0f);
            glShadeModel(GL_FLAT);
        } break;
        case DISPLAY_SMOOTH_SHADING: {
            glEnable(GL_LIGHTING);
            glPolygonMode(GL_FRONT, GL_FILL);
            glColor3f(1.0f, 1.0f, 1.0f);
            glShadeModel(GL_SMOOTH);
        } break;
        default: break;
    }

    // Setup the camera
    gluLookAt( 3.5 * cos( shapesUI->getYRot() ) * cos( shapesUI->getXRot() ), 
               3.5 * sin( shapesUI->getYRot() ), 
               3.5 * cos( shapesUI->getYRot() ) * sin( shapesUI->getXRot() ), 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // ToDo: draw your shape here
    // DO NOT put the actual draw OpenGL code here - put it in the shape class and call the draw method
	s->drawTriangles();
    endDrawing();
}

int ShapesUI::handle(int event) {
    return 0;
}

void ShapesUI::changedShape()
{
    // ToDo: Switch shapes
    RedrawWindow();
	int type = shapesUI->getShapeType();
	t1 = shapesUI->getTessel1();
	t2 = shapesUI->getTessel2();
	if (type == SHAPE_CUBE){
		s = new Cube();
		s->newTess(t1, t1);
		
	}
	else if (type == SHAPE_CYLINDER){
		s = new Cylinder();
		s->newTess(t1, t2);
	}
	else if (type == SHAPE_CONE){
		s = new Cone();
		s->newTess(t1, t2);
	}
	else if (type == SHAPE_SPHERE){
		s = new Sphere();
		s->newTess(t1, t2);
	}
	else if (type == SHAPE_SPECIAL1){
		s = new SpecialShape1();
		s->newTess(t1, t2);
	}
	else if (type == SHAPE_SPECIAL2){
		s = new SpecialShape2();
		s->newTess(t1, t2);
	}
}

void ShapesUI::changedTessel() {
    // ToDo: tessellate your shape here
    RedrawWindow();
	t1 = shapesUI->getTessel1();
	t2 = shapesUI->getTessel2();
	s->newTess(t1, t2);
}

