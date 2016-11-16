#include "../cse452.h"
#include "MyScene.h"
#include "Cone.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Cube.h"
#include "Shape.h"
#include "SpecialShape2.h"
#include "SpecialShape1.h"

using namespace std;

void MyScene::resize(int w, int h) {
    // resize the film plane to the specified width/height
    camera.setWidthHeight(w, h);
}

/// Note: your camera and screen clear, etc, will be set up by
/// SceneviewUI.cpp *before* this gets called
void MyScene::draw() {
    // render the scene using OpenGL
    if (!isLoaded) // Don't draw if loadSceneFile hasn't been called yet
        return;


    // Turn off all lights
    for ( int i = 0; i < 7; i++ )
        glDisable( GL_LIGHT0 + i );

    //  .. and reset
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, &ambientLight[0] );
    for (unsigned int i = 0; i < lights.size(); i++) {
        lights[i].setOpenGLLight( GL_LIGHT0 + i );
    }

    // TODO: draw the rest of the scene here
	
	for (unsigned i = 0; i < objList.size(); ++i){
		Object obj = objList[i];
		glPushMatrix();
		glMultMatrixd(&obj.trans(0, 0));
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, &obj.ambient[0]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &obj.diffuse[0]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &obj.specular[0]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, &obj.emit[0]);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, obj.shine);

		obj.shapeptr->drawTriangles();
		glPopMatrix();
	}
		
}