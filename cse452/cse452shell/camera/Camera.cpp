#include "../cse452.h"
#include "Camera.h"
#include <cmath>
#include <FL/Fl.H>

using namespace std;

Camera::Camera() 
{
    // initialize your data here
	setFrom(Point3(3.0,2.0,6.0));
	setAt(Point3(0, 0, 0));
	setUp(Vector3(0, 1.0, 0));
	setWidthHeight(0, 0);
	setZoom(0);
	setNearFar(0, 0);
	setAspectRatioScale(0);
}

Camera::~Camera() {
    // destroy your data here
}

// The following three should be unit length and orthogonal to each other
// u vector
Vector3 Camera::getRight() const
{
	return u;
}

// v vector
Vector3 Camera::getUp() const
{
    return v;
}

// - n vector
Vector3 Camera::getLook() const
{
    return -n;
}

//EXTRA
double Camera::getSkew() const 
{
    // Change this to implement the extra credit
    return 0.0;
}

//EXTRA
double Camera::getAspectRatioScale() const
{
    return a;
}

//EXTRA
Point3 Camera::getProjectionCenter() const
{
    // Change this to implement the extra credit
    return Point3( 0.0, 0.0, 0.0 );
}

Matrix4 Camera::getProjection() const {
    // return the current projection and scale matrix
    return D * Sxyz * Sxy;
}

Matrix4 Camera::getWorldToCamera() const {
    // return the current world to camera matrix
    // Rotation and translation
    return R * T;
}

Matrix4 Camera::getRotationFromXYZ() const
{
    // return just the rotation matrix
    return R;
}

Matrix4 Camera::getRotationToXYZ() const
{
    // return just the rotation matrix
    return R.transpose();
}

Matrix4 Camera::getCameraToWorld() const {
    // return the current camera to world matrix
    // This is the inverse of the rotation, translation, and scale
    return Matrix4::translation(P) * R.transpose() * Matrix4::scaling(a*tan(qh/2), tan(qh/2), 1) * Matrix4::scaling(df);
}

int Camera::getWidth()  const{
    // return the current image width
    return a * height;
}

int Camera::getHeight()  const{
    // return the current image height
    return height;
}

Point3 Camera::getEye()  const{
    // return the current eye location
    return P;
}

double Camera::getZoom() const
{
    return qh;
}

void Camera::setFrom(const Point3& from) {
    // set the current viewpoint (eye point)
	P = from;
	update_T();
}

void Camera::setAt(const Point3& at) {
    // set the point the camera is looking at
    // calling this requires that the from (or eye) point already be valid
	L = at - P;
	update_n();
	update_v();
	update_u();
	update_R();
}

void Camera::setLook(const Vector3& l) {
    // set the direction the camera is looking at
	L = l;
	update_n();
	update_v();
	update_u();
	update_R();
}

void Camera::setUp(const Vector3& up) {
    // set the upwards direction of the camera
	U = up;
	update_v();
	update_u();
	update_R();
}

void Camera::setWidthHeight(int w, int h) {
    // set the current width and height of the film plane
	height = h * 1.0;
	a = ((double) w) * 1.0 / h;
	update_Sxy();
}

void Camera::setZoom(double z) {
    // set field of view (specified in degrees)
	qh = z / 180.0 * M_PI;
	update_Sxy();
}

void Camera::setNearFar(double n, double f) {
    // set the near and far clipping planes
	dn = n;
	df = f;
	update_Sxyz();
	update_D();
}

//EXTRA
void Camera::setSkew( double d )
{
}

void Camera::setAspectRatioScale( double d )
{
}

//EXTRA
void Camera::setProjectionCenter( const Point3 &p )
{
}

void Camera::moveForward(double dist) {
    // move the camera forward (in the viewing direction)
    // by the amount dist
	setFrom(P + dist * L / L.length());
}

void Camera::moveSideways(double dist) {
    // move the camera sideways (orthogonal to the viewing direction)
    // by the amount dist
	setFrom(P + dist * u / u.length());
}

void Camera::moveVertical(double dist) {
    // move the camera vertically (along the up vector)
    // by the amount dist
	setFrom(P + dist * U / U.length());
}

void Camera::rotateYaw(double angle) {
    // rotate the camera left/right (around the up vector)
	setLook(cos(angle)*unit(L) + sin(angle)* (-u));
	
}

void Camera::rotatePitch(double angle) {
    // rotate the camera up/down (pitch angle)
	setLook(cos(angle)*unit(L) + sin(angle)* v);
}

void Camera::rotateAroundAtPoint(int axis, double angle, double focusDist) {
    // Rotate the camera around the right (0), up (1), or look (2) vector
    // around the point at eye + look * focusDist

}


void Camera::moveKeyboard( )
{
    // you may change key controls for the interactive
    // camera controls here, make sure you document your changes
    // in your README file

    if (Fl::event_key('w'))
        moveForward(+0.05);
    if (Fl::event_key('s'))
        moveForward(-0.05);
    if (Fl::event_key('a'))
        moveSideways(-0.05);
    if (Fl::event_key('d'))
        moveSideways(+0.05);
    if (Fl::event_key(FL_Up))
        moveVertical(+0.05);
    if (Fl::event_key(FL_Down))
        moveVertical(-0.05);
    if (Fl::event_key(FL_Left))
        rotateYaw(+0.05);
    if (Fl::event_key(FL_Right))
        rotateYaw(-0.05);
    if (Fl::event_key(FL_Page_Up))
        rotatePitch(+0.05);
    if (Fl::event_key(FL_Page_Down))
        rotatePitch(-0.05);
}

void Camera::update_n(){
	n = - unit(L);
}

void Camera::update_v(){
	Vector3 v1 = U - dot(U, n) * n;
	v = unit(v1);
}

void Camera::update_u(){
	u = cross(v, n);
}

void Camera::update_T(){
	T = Matrix4::translation(Point3(-P[0], -P[1], -P[2]));
}

void Camera::update_R(){
	R = Matrix4(u, v, n);
}

void Camera::update_Sxy(){
	Sxy = Matrix4::scaling(1.0 / ( a * tan(qh / 2.0)), 1.0 / tan(qh / 2.0), 1.0);
}

void Camera::update_Sxyz(){
	Sxyz = Matrix4::scaling(1.0 / df);
}

void Camera::update_D(){
	double k = dn / df;
	D = Matrix4(Vector4(1, 0, 0, 0),
				Vector4(0, 1, 0, 0),
				Vector4(0, 0, 1 / (k - 1), k / (k - 1)),
				Vector4(0, 0, -1, 0));
}
