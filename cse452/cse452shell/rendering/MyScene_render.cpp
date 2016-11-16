#include "../cse452.h"
#include "../sceneview/MyScene.h"
#include "RenderingInterface.h"
#include <FL/gl.h>
#include <cfloat>

using namespace std;

void MyScene::render(int type, int width, int height, unsigned char* pixels) {
    if (!isLoaded) return;
    // Add your rendering code here.
    // Keep track of your progress as a value between 0 and 1 so the progress bar can update as the rendering progresses
	progress = 0.0;
	stop = false;
	w = double(width);
	h = double(height);
	eye = camera.getEye();

    switch (type) {
        case RenderingUI::RENDER_SCANLINE:
			progress = 1.0;
			break;
		case RenderingUI::RENDER_RAY_TRACING: {
			double red, green, blue;
			for (double j = 0; j < height; j = j + 1.0){
				for (double i = 0; i < width; i = i + 1.0){
					traceRay(i, j, red, green, blue);
					int cur = (i + j * width) * 3;
					normalizeColor(red, green, blue);
					pixels[cur] = (unsigned char)(red * 255.0f);
					pixels[cur + 1] = (unsigned char)(green * 255.0f);
					pixels[cur + 2] = (unsigned char)(blue * 255.0f);
				}
				progress = (double)(j + 1) / (double)(height);
				if (stop) return;
				Fl::check();
			}
		} break;
		case RenderingUI::RENDER_PATH_TRACING:
			progress = 1.0;
			break;
        default: break;
    }
}

void MyScene::stopRender()
{
    // Because this is threaded code, this function
    // can be called in the middle of your rendering code.
    // You should then stop at the next scanline
	stop = true;
}

double MyScene::getRenderProgress() {
    // return the current progress as a value between 0 and 1
    return progress;
}

void MyScene::normalizeColor(double& r, double& g, double& b){
	double max = 1.0;
	if (r < 0) r = 0.0;
	else if (r > max) max = r;
	if (g < 0) g = 0.0;
	else if (g > max) max = g;
	if (b < 0) b = 0.0;
	else if (b > max) max = b;
	if (max > 1){
		r /= max;
		g /= max;
		b /= max;
	}
}

// add extra methods here
void MyScene::traceRay(double& i, double& j, double& red, double& green, double& blue){
	Vector3 ray = getCamera().getCameraToWorld() * Point3(((i + 0.5) * 2.0 / w - 1.0), 1.0 - (h - 1 - j + 0.5) * 2.0 / h, -1.0) - eye;
	ray.normalize();
	lightToScene(ray, eye, red, green, blue, 0);
}

void MyScene::lightToScene(const Vector3& ray, const Point3& p, double& r, double& g, double& b, int depth){
	Object * intersected_obj = nullptr;
	Vector3 hit_normal;
	Point3 hit_p;
	findIntersect(ray, p, intersected_obj, hit_normal, hit_p);

	if (depth < RECURSIVE_LIMIT && intersected_obj != nullptr){
		Color c_diffuse = Color(0.0, 0.0, 0.0), c_specular = Color(0.0, 0.0, 0.0), c_reflect = Color(0.0 ,0.0 ,0.0), c_refract = Color(0.0, 0.0, 0.0);
		for (vector<Light>::iterator l = lights.begin(); l != lights.end(); ++l){
			Object * hit_obj = nullptr;
			Vector3 dir_to_light = l->getPos() - hit_p, hit_obj_n, reflect_l;
			Point3 hit_obj_p, fallOff;
			Color c_light = l->getColor();
			double len = dir_to_light.length(), atten, shine, diffuse, specular;
			bool notInShadow = true;
			findIntersect(unit(dir_to_light), hit_p + SHIFT * unit(dir_to_light), hit_obj, hit_obj_n, hit_obj_p);

			if (hit_obj != nullptr){
				if (dir_to_light.length() > (hit_obj_p - hit_p).length()) notInShadow = false;
			}

			if (notInShadow){
				reflect_l = 2.0 * (dir_to_light * hit_normal) * hit_normal - dir_to_light;
				reflect_l.normalize();

				// attenuation
				fallOff = l->getFalloff();
				atten = 1.0 / (fallOff[0] + fallOff[1] * len + fallOff[2] * pow(len, 2.0));

				// diffuse
				diffuse = hit_normal * unit(dir_to_light);
				if (diffuse > 0) c_diffuse = c_diffuse + c_light * atten * intersected_obj->diffuse * diffuse;
				
				if (camera.getLook() * hit_obj_n <= 0){
					// specular
					shine = intersected_obj->shine;
					specular = ray * reflect_l;
					if (specular < 0) specular = 0.0;
					c_specular = c_specular + c_light * atten * intersected_obj->specular * pow(specular, shine);
				}
			}
		}

		// reflection
		Color obj_reflect = intersected_obj->reflect;
		if (obj_reflect.getMax() > 0){
			double reflect_r = 0.0, reflect_g = 0.0, reflect_b = 0.0;
			Vector3 reflected_ray = ray;
			reflected_ray = 2 * (hit_normal * reflected_ray) * hit_normal - reflected_ray;
			reflected_ray.normalize();

			lightToScene(unit(reflected_ray), hit_p + SHIFT * unit(reflected_ray), reflect_r, reflect_g, reflect_b, depth + 1);
			c_reflect = Color(static_cast<GLfloat>(reflect_r), static_cast<GLfloat>(reflect_g), static_cast<GLfloat>(reflect_b)) * obj_reflect;
		}

		// refraction & transparency
		Color obj_transp = intersected_obj->transparent;
		if (obj_transp.getMax() > 0){
			double trans_r = 0.0,  trans_g = 0.0, trans_b = 0.0;
			double c = -hit_normal * ray, ior = intersected_obj->ior;
			if(ray * hit_normal > 0) ior = 1.0 / ior;
			Vector3 refracted_ray = ior * ray + (ior * c - sqrt(1 - ior * ior * (1 - c * c))) * hit_normal;
			lightToScene(unit(refracted_ray), hit_p + SHIFT * unit(refracted_ray), trans_r, trans_g, trans_b, depth + 1);
			c_refract = Color(static_cast<GLfloat>(trans_r), static_cast<GLfloat>(trans_g), static_cast<GLfloat>(trans_b)) * obj_transp;
		}

		Color c = c_diffuse + c_specular + intersected_obj->ambient + c_reflect + c_refract;
		r = c[0]; g = c[1]; b = c[2];
	}
	else {
		r = background[0];
		g = background[1];
		b = background[2];
	}
}

void MyScene::findIntersect(const Vector3 ray, const Point3 p, Object*& intersected_obj, Vector3& normal, Point3& hit_p){
	double min_t = DBL_MAX, t, u, v;
	Point3 temp_p;
	Vector3 temp_d;
	HitRecord hits;
	Matrix4 m, m_inv;

	for (vector<Object>::iterator obj = objList.begin(); obj != objList.end(); ++obj){
		hits = obj->shapeptr->intersect(obj->trans_inv * p, obj->trans_inv * ray);
		hits.sortHits();
		if (hits.getFirstHit(t, u, v, temp_p, temp_d)){
			if (t < min_t && t > 0){
				min_t = t;
				intersected_obj = &(*obj);
				hit_p = temp_p;
				normal = temp_d;
				m = obj->trans;
				m_inv = obj->trans_inv;
			}
		}
	}
	normal = m * normal;
	normal.normalize();
	hit_p = m * hit_p;
}