Name: Ray Su
Student Id: 432026
Email: suz@wustl.edu
Assignment 6: Ray Tracing

Implementation:
I created variables RECURSIVE_LIMIT = 5, progress, w, h, SHIFT = 0.001 (to handle dismissing current object), 
eye (to record eye position), stop (to decide if the stop progress button has been hit).

In MyScene_render.cpp, I created a loop to go through each pixel and call traceRay() on each pixel to compute
the corresponding colors in red, green and blue. I also create function normalizeColor() to rescale each color 
back to the range between 0 and 1. After each row of pixels, I called Fl::check() to update the
process and increase the progress value to handle the progress bar in UI. Here w and h are used to store the 
width and height globally. 

In traceRay(), I firstly convert the ray to world coordinate, and pass the ray, eye, color into lightToScene(), 
which is used for calculating illumination of each point. lightToScene() computes illumination recursively, which
means it would keep calling itself for reflection and refraction until the RECURSIVE_LIMIT has been reached.

For each call of lightToScene(), this function would firstly check if the ray hit some object by calling function
findIntersect(), and then if the ray has reasonably hit some object, it would start to loop through each light to 
decide if the object can be reached by the light and then compute attenuation, diffuse, specular accordingly. Then, 
it would compute reflection and refraction by calling itself. In the end all colors are added up and transferred back 
into the color variables passed in. If RECURSIVE_LIMIT has been reached or the ray hits nothing, background color 
would be returned.

For findIntersect(), this function would loop through all the objects in the flattened object list created in the 
previous labs, for which this function would compare their distance from the original object and return the nearest
object, the hitting point, and the normal vector of that point.

Extra Credit:
Refraction & Transparency: I applied refraction and transparency by adding this part in lightToScene(). I calculated 
the refracted_ray as ior * ray + (ior * c - sqrt(1 - ior * ior * (1 - c * c))) * hit_normal, where c = -hit_normal * ray, 
and ior is determined by the object's ior and the relation between ray and hit_normal.
Then, I called lightToScene() recursively to get the transparent colors, which would be added to the final colors.

Speed Optimization: I directly computed the inverse of transformation matrices when flattening 
the tree, which saves the time for explicitly computing inverse twice in findIntersect(). I also initialized the 
pointers to each object's shape during flattening, which saves the time for findIntersect() as well.


Known Bug:
NONE