#include "assignment1.h"
#include <math.h>
#include "Miro.h"
#include "Scene.h"
#include "Camera.h"
#include "Image.h"

#include "PointLight.h"
#include "TriangleMesh.h"
#include "Triangle.h"
#include "Sphere.h"
#include "Lambert.h"
#include "Mirror.h"
#include "Glass.h"
#include "PFMLoader.h"

// local helper function declarations
namespace
{
	void addMeshTrianglesToScene(TriangleMesh * mesh, Material * material);
	inline Matrix4x4 translate(float x, float y, float z);
	inline Matrix4x4 scale(float x, float y, float z);
	inline Matrix4x4 rotate(float angle, float x, float y, float z);
} // namespace


void
makeTeapotScene()
{
	g_camera = new Camera;
	g_scene = new Scene;
	g_image = new Image;

	g_image->resize(512, 512);

	// set up the camera
	g_camera->setBGColor(Vector3(0.0f, 0.0f, 0.2f));
	g_camera->setEye(Vector3(0, 3, 6));
	g_camera->setLookAt(Vector3(0, 0, 0));
	g_camera->setUp(Vector3(0, 1, 0));
	g_camera->setFOV(45);
	//g_camera->activeLensCamera(true);
	//g_camera->setLensSize(0.5f);
	g_camera->setFocalDistance(2.5f);

	// create and place a point light source
	PointLight * light = new PointLight;
	light->setPosition(Vector3(10, 10, 10));
	light->setColor(Vector3(1, 1, 1));
	light->setWattage(4.0 * PI * 700);
	g_scene->addLight(light);

	//Material* material = new Lambert(Vector3(1.0f));
	Material* material_tea = new Lambert(Vector3(1.0f));
	//Material* material_tea = new Mirror(Vector3(1.0f, 1.0f, 1.0f));
	//Material* material_tea = new Glass(Vector3(1.0f, 1.0f, 1.0f), 1.55f); //glass
	TriangleMesh * teapot = new TriangleMesh;
	teapot->load("teapot.obj");
	addMeshTrianglesToScene(teapot, material_tea);

	// create the floor triangle
	TriangleMesh * floor = new TriangleMesh;
	//Material* material_floor = new Mirror(Vector3(0.1f, 1.0f, 0.1f));
	//Material* material_floor = new Mirror(Vector3(1.0f));
	Material* material_floor = new Lambert(Vector3(0.0f, 1.0f, 0.0f));
	floor->createSingleTriangle();
	floor->setV1(Vector3(-10, 0, -10));
	floor->setV2(Vector3(  0, 0,  10));
	floor->setV3(Vector3( 10, 0, -10));
	floor->setN1(Vector3(0, 1, 0));
	floor->setN2(Vector3(0, 1, 0));
	floor->setN3(Vector3(0, 1, 0));

	Triangle* t = new Triangle;
	t->setIndex(0);
	t->setMesh(floor);
	t->setMaterial(material_floor);
	g_scene->addObject(t);

	// create the floor triangle
	TriangleMesh * floor2 = new TriangleMesh;
	Material* material_floor2 = new Lambert(Vector3(1.0f));
	// Material* material_floor2 = new Mirror(Vector3(1.0f));
	floor2->createSingleTriangle();
	floor2->setV1(Vector3(-10, 5, -10));
	floor2->setV2(Vector3(  0, 5,  10));
	floor2->setV3(Vector3( 10, 5, -10));
	floor2->setN1(Vector3(0, 1, 0));
	floor2->setN2(Vector3(0, 1, 0));
	floor2->setN3(Vector3(0, 1, 0));

	Triangle* t2 = new Triangle;
	t2->setIndex(0);
	t2->setMesh(floor2);
	t2->setMaterial(material_floor2);
	g_scene->addObject(t2);

/*
	//Material* material_sphere = new Mirror(Vector3(1.0f, 1.0f, 1.0f));
	Material* material_sphere = new Glass(Vector3(0.9f, 0.9f, 0.2f), 1.55f); //glass
	//Material* material_sphere = new Glass(Vector3(1.0f), 1.0f); //glass = 1.55 0.90.90.2

	const float a = 3.0f;
	// float t_s = 0.01f;
	// float theta = 4*PI*t_s;
	float r = a;
	float x = -0.3f;
	float y = 2.0f;
	float z = 2.0f;
	Sphere * sphere = new Sphere;
	sphere->setCenter(Vector3(x,y,z));
	sphere->setRadius(r/10);
	sphere->setMaterial(material_sphere);
	g_scene->addObject(sphere);
*/

	// let objects do pre-calculations if needed
	g_scene->preCalc();
}


void
makeBunny1Scene()
{
	g_camera = new Camera;
	g_scene = new Scene;
	g_image = new Image;

	g_image->resize(512, 512);

	// set up the camera
	g_camera->setBGColor(Vector3(0.0f, 0.0f, 0.2f));
	g_camera->setEye(Vector3(0, 5, 15));
	g_camera->setLookAt(Vector3(0, 0, 0));
	g_camera->setUp(Vector3(0, 1, 0));
	g_camera->setFOV(45);

	// create and place a point light source
	PointLight * light = new PointLight;
	light->setPosition(Vector3(10, 20, 10));
	light->setColor(Vector3(1, 1, 1));
	light->setWattage(4.0 * PI * 1000);
	g_scene->addLight(light);

	//Material* material_bunny = new Lambert(Vector3(1.0f));
	Material* material_bunny = new Mirror(Vector3(1.0f, 0.01f, 1.0f));
	TriangleMesh * bunny = new TriangleMesh;
	bunny->load("bunny.obj");
	addMeshTrianglesToScene(bunny, material_bunny);

	// create the floor triangle
	TriangleMesh * floor = new TriangleMesh;
	Material* material_floor = new Lambert(Vector3(1.0f, 1.0f, 0.0f));
	//Material* material_floor = new Mirror(Vector3(0.01f, 0.9f, 0.01f));
	floor->createSingleTriangle();
	floor->setV1(Vector3(-100, 0, -100));
	floor->setV2(Vector3(   0, 0,  100));
	floor->setV3(Vector3( 100, 0, -100));
	floor->setN1(Vector3(0, 1, 0));
	floor->setN2(Vector3(0, 1, 0));
	floor->setN3(Vector3(0, 1, 0));

	Triangle* t = new Triangle;
	t->setIndex(0);
	t->setMesh(floor);
	t->setMaterial(material_floor);
	g_scene->addObject(t);

	// let objects do pre-calculations if needed
	g_scene->preCalc();
}



void
makeBunny20Scene()
{
	g_camera = new Camera;
	g_scene = new Scene;
	g_image = new Image;

	//g_image->resize(128, 128);
	g_image->resize(512, 512);

	// set up the camera
	g_camera->setBGColor(Vector3(0.0f, 0.0f, 0.2f));
	g_camera->setEye(Vector3(0, 5, 15));
	g_camera->setLookAt(Vector3(0, 0, 0));
	g_camera->setUp(Vector3(0, 1, 0));
	g_camera->setFOV(45);

	// create and place a point light source
	PointLight * light = new PointLight;
	light->setPosition(Vector3(10, 20, 10));
	light->setColor(Vector3(1, 1, 1));
	light->setWattage(4.0 * PI * 1000);
	g_scene->addLight(light);

	TriangleMesh * mesh;
	Material* material = new Lambert(Vector3(1.0f));
	Material* material_mirror_bunny = new Mirror(Vector3(1.0f, 0.01f, 1.0f));
	Matrix4x4 xform;
	Matrix4x4 xform2;
	xform2 *= rotate(110, 0, 1, 0);
	xform2 *= scale(.6, 1, 1.1);


	// bunny 1
	xform.setIdentity();
	xform *= scale(0.3, 2.0, 0.7);
	xform *= translate(-1, .4, .3);
	xform *= rotate(25, .3, .1, .6);
	mesh = new TriangleMesh;
	mesh->load("bunny.obj", xform);
	addMeshTrianglesToScene(mesh, material);

	// bunny 2
	xform.setIdentity();
	xform *= scale(.6, 1.2, .9);
	xform *= translate(7.6, .8, .6);
	mesh = new TriangleMesh;
	mesh->load("bunny.obj", xform);
	addMeshTrianglesToScene(mesh, material_mirror_bunny);

	// bunny 3
	xform.setIdentity();
	xform *= translate(.7, 0, -2);
	xform *= rotate(120, 0, .6, 1);
	mesh = new TriangleMesh;
	mesh->load("bunny.obj", xform);
	addMeshTrianglesToScene(mesh, material);

	// bunny 4
	xform.setIdentity();
	xform *= translate(3.6, 3, -1);
	mesh = new TriangleMesh;
	mesh->load("bunny.obj", xform);
	addMeshTrianglesToScene(mesh, material_mirror_bunny);

	// bunny 5
	xform.setIdentity();
	xform *= translate(-2.4, 2, 3);
	xform *= scale(1, .8, 2);
	mesh = new TriangleMesh;
	mesh->load("bunny.obj", xform);
	addMeshTrianglesToScene(mesh, material);

	// bunny 6
	xform.setIdentity();
	xform *= translate(5.5, -.5, 1);
	xform *= scale(1, 2, 1);
	mesh = new TriangleMesh;
	mesh->load("bunny.obj", xform);
	addMeshTrianglesToScene(mesh, material);

	// bunny 7
	xform.setIdentity();
	xform *= rotate(15, 0, 0, 1);
	xform *= translate(-4, -.5, -6);
	xform *= scale(1, 2, 1);
	mesh = new TriangleMesh;
	mesh->load("bunny.obj", xform);
	addMeshTrianglesToScene(mesh, material);

	// bunny 8
	xform.setIdentity();
	xform *= rotate(60, 0, 1, 0);
	xform *= translate(5, .1, 3);
	mesh = new TriangleMesh;
	mesh->load("bunny.obj", xform);
	addMeshTrianglesToScene(mesh, material_mirror_bunny);

	// bunny 9
	xform.setIdentity();
	xform *= translate(-3, .4, 6);
	xform *= rotate(-30, 0, 1, 0);
	mesh = new TriangleMesh;
	mesh->load("bunny.obj", xform);
	addMeshTrianglesToScene(mesh, material);

	// bunny 10
	xform.setIdentity();
	xform *= translate(3, 0.5, -2);
	xform *= rotate(180, 0, 1, 0);
	xform *= scale(1.5, 1.5, 1.5);
	mesh = new TriangleMesh;
	mesh->load("bunny.obj", xform);
	addMeshTrianglesToScene(mesh, material);

	// bunny 11
	xform = xform2;
	xform *= scale(0.3, 2.0, 0.7);
	xform *= translate(-1, .4, .3);
	xform *= rotate(25, .3, .1, .6);
	mesh = new TriangleMesh;
	mesh->load("bunny.obj", xform);
	addMeshTrianglesToScene(mesh, material);

	// bunny 12
	xform = xform2;
	xform *= scale(.6, 1.2, .9);
	xform *= translate(7.6, .8, .6);
	mesh = new TriangleMesh;
	mesh->load("bunny.obj", xform);
	addMeshTrianglesToScene(mesh, material);

	// bunny 13
	xform = xform2;
	xform *= translate(.7, 0, -2);
	xform *= rotate(120, 0, .6, 1);
	mesh = new TriangleMesh;
	mesh->load("bunny.obj", xform);
	addMeshTrianglesToScene(mesh, material);

	// bunny 14
	xform = xform2;
	xform *= translate(3.6, 3, -1);
	mesh = new TriangleMesh;
	mesh->load("bunny.obj", xform);
	addMeshTrianglesToScene(mesh, material);

	// bunny 15
	xform = xform2;
	xform *= translate(-2.4, 2, 3);
	xform *= scale(1, .8, 2);
	mesh = new TriangleMesh;
	mesh->load("bunny.obj", xform);
	addMeshTrianglesToScene(mesh, material);

	// bunny 16
	xform = xform2;
	xform *= translate(5.5, -.5, 1);
	xform *= scale(1, 2, 1);
	mesh = new TriangleMesh;
	mesh->load("bunny.obj", xform);
	addMeshTrianglesToScene(mesh, material);

	// bunny 17
	xform = xform2;
	xform *= rotate(15, 0, 0, 1);
	xform *= translate(-4, -.5, -6);
	xform *= scale(1, 2, 1);
	mesh = new TriangleMesh;
	mesh->load("bunny.obj", xform);
	addMeshTrianglesToScene(mesh, material);

	// bunny 18
	xform = xform2;
	xform *= rotate(60, 0, 1, 0);
	xform *= translate(5, .1, 3);
	mesh = new TriangleMesh;
	mesh->load("bunny.obj", xform);
	addMeshTrianglesToScene(mesh, material);

	// bunny 19
	xform = xform2;
	xform *= translate(-3, .4, 6);
	xform *= rotate(-30, 0, 1, 0);
	mesh = new TriangleMesh;
	mesh->load("bunny.obj", xform);
	addMeshTrianglesToScene(mesh, material);

	// bunny 20
	xform = xform2;
	xform *= translate(3, 0.5, -2);
	xform *= rotate(180, 0, 1, 0);
	xform *= scale(1.5, 1.5, 1.5);
	mesh = new TriangleMesh;
	mesh->load("bunny.obj", xform);
	addMeshTrianglesToScene(mesh, material);


	// create the floor triangle
	mesh = new TriangleMesh;
	mesh->createSingleTriangle();
	mesh->setV1(Vector3(-100, 0, -100));
	mesh->setV2(Vector3(   0, 0,  100));
	mesh->setV3(Vector3( 100, 0, -100));
	mesh->setN1(Vector3(0, 1, 0));
	mesh->setN2(Vector3(0, 1, 0));
	mesh->setN3(Vector3(0, 1, 0));

	Triangle* t = new Triangle;
	t->setIndex(0);
	t->setMesh(mesh);
	t->setMaterial(material);
	g_scene->addObject(t);

	// let objects do pre-calculations if needed
	g_scene->preCalc();
}


void
makeSponzaScene()
{
	g_camera = new Camera;
	g_scene = new Scene;
	g_image = new Image;

	g_image->resize(512, 512);

	// set up the camera
	g_camera->setBGColor(Vector3(0.0f, 0.0f, 0.2f));
	g_camera->setEye(Vector3(8, 1.5, 1));
	g_camera->setLookAt(Vector3(0, 2.5, -1));
	g_camera->setUp(Vector3(0, 1, 0));
	g_camera->setFOV(55);

	// create and place a point light source
	PointLight * light = new PointLight;
	light->setPosition(Vector3(0, 10.0, 0));
	light->setColor(Vector3(1, 1, 1));
	light->setWattage(4.0 * PI * 200);
	g_scene->addLight(light);

	Material* material = new Lambert(Vector3(1.0f));
	TriangleMesh * mesh = new TriangleMesh;
	mesh->load("sponza.obj");
	addMeshTrianglesToScene(mesh, material);

	// let objects do pre-calculations if needed
	g_scene->preCalc();
}

void
makeCornellBoxScene()
{
	g_camera = new Camera;
	g_scene = new Scene;
	g_image = new Image;

	g_image->resize(512, 512);

	// set up the camera
	g_camera->setBGColor(Vector3(0.0f, 0.0f, 0.2f));
	g_camera->setEye(Vector3(2.7, 1.5, 6));
	g_camera->setLookAt(Vector3(2.7, 2.5, -1));
	g_camera->setUp(Vector3(0, 1, 0));
	g_camera->setFOV(55);

	// create and place a point light source
	PointLight * light = new PointLight;
	// light->setPosition(Vector3(3, 4, 0));
	light->setPosition(Vector3(2.75, 5.4, -1.75));//CENTER
	light->setColor(Vector3(1, 1, 1));
	light->setWattage(4.0 * PI * 30); //*100
	g_scene->addLight(light);

	Material* material = new Lambert(Vector3(0.95f), true);
	Material* material_r = new Lambert(Vector3(1.0f, 0.0f, 0.0f), true);
	Material* material_g = new Lambert(Vector3(0.0f, 1.0f, 0.0f), true);
	Material* material_b = new Lambert(Vector3(0.0f, 0.0f, 1.0f), true);
	// Material* material = new Lambert(Vector3(0.95f));
	// Material* material_r = new Lambert(Vector3(1.0f, 0.05f, 0.05f));
	// Material* material_g = new Lambert(Vector3(0.05f, 1.0f, 0.05f));

	TriangleMesh * mesh_w = new TriangleMesh;
	// mesh_w->load("cornell_box_w_no_hole.obj"); //motion blur
	mesh_w->load("cornell_box_w_boxes.obj"); //path tracing
	//mesh_w->load("cornell_box_w.obj");
	addMeshTrianglesToScene(mesh_w, material);
	TriangleMesh * mesh_r = new TriangleMesh;
	mesh_r->load("cornell_box_r.obj");
	addMeshTrianglesToScene(mesh_r, material_r);
	TriangleMesh * mesh_g = new TriangleMesh;
	mesh_g->load("cornell_box_g.obj");
	addMeshTrianglesToScene(mesh_g, material_g);

	// motion blur
	// float r = 0.5;
	// float x = 3.5f;
	// float y = 2.0f;
	// float z = -1.5f;
	// Sphere * sphere = new Sphere;
	// sphere->setCenter(Vector3(x,y,z));
	// sphere->setRadius(r);
	// sphere->setMaterial(material_b);
	// sphere->setMove(true);
	// g_scene->addObject(sphere);
	//



	g_scene->setSubpixelsampling();

	// let objects do pre-calculations if needed
	g_scene->preCalc();
}

void addGraceHDR()
{
	g_camera = new Camera;
	g_scene = new Scene;
	g_image = new Image;

	g_image->resize(512, 512);

	// set up the camera
	g_camera->setBGColor(Vector3(0.0f, 0.0f, 0.2f));
	g_camera->setEye(Vector3(0, 3, 6));
	g_camera->setLookAt(Vector3(0, 0, 0));
	g_camera->setUp(Vector3(0, 1, 0));
	g_camera->setFOV(45);

	// create and place a point light source
	PointLight * light = new PointLight;
	light->setPosition(Vector3(10, 10, 10));
	light->setColor(Vector3(1, 1, 1));
	light->setWattage(4.0 * PI * 700);
	g_scene->addLight(light);

	int width = 1000;
	int height = 1000;
	Vector3* hdr = readPFMImage("grace_probe.pfm", &width, &height);
	g_scene->addImage(hdr, width, height);

	//Material* material = new Lambert(Vector3(1.0f));
	 // Material* material_tea = new Lambert(Vector3(1.0f));
	//Material* material_tea = new Mirror(Vector3(1.0f, 1.0f, 1.0f));
	//Material* material_tea = new Glass(Vector3(1.0f, 1.0f, 1.0f), 1.55f); //glass
	// TriangleMesh * teapot = new TriangleMesh;
	// teapot->load("teapot.obj");
	// addMeshTrianglesToScene(teapot, material_tea);

	// create the floor triangle
	TriangleMesh * floor = new TriangleMesh;
	//Material* material_floor = new Mirror(Vector3(0.1f, 1.0f, 0.1f));
	//Material* material_floor = new Mirror(Vector3(1.0f));
	Material* material_floor = new Lambert(Vector3(1.0f, 1.0f, 1.0f));
	floor->createSingleTriangle();
	floor->setV1(Vector3(-10, 0, -10));
	floor->setV2(Vector3(  0, 0,  10));
	floor->setV3(Vector3( 10, 0, -10));
	floor->setN1(Vector3(0, 1, 0));
	floor->setN2(Vector3(0, 1, 0));
	floor->setN3(Vector3(0, 1, 0));

	Triangle* t = new Triangle;
	t->setIndex(0);
	t->setMesh(floor);
	t->setMaterial(material_floor);
	g_scene->addObject(t);


	// Material* material_sphere = new Lambert(Vector3(1.0f, 1.0f, 1.0f));
	Material* material_sphere = new Mirror(Vector3(1.0f, 1.0f, 1.0f));
	// Material* material_sphere = new Glass(Vector3(1.0f), 1.55f); //glass = 1.55 0.90.90.2
	// float t_s = 0.01f;
	// float theta = 4*PI*t_s;
	float r = 1.0f;
	float x = -0.3f;
	float y = 2.0f;
	float z = -1.0f;
	Sphere * sphere = new Sphere;
	sphere->setCenter(Vector3(x,y,z));
	sphere->setRadius(r);
	sphere->setMaterial(material_sphere);
	g_scene->addObject(sphere);


	// let objects do pre-calculations if needed
	g_scene->preCalc();
}

// local helper function definitions
namespace
{

	void
	addMeshTrianglesToScene(TriangleMesh * mesh, Material * material)
	{
		// create all the triangles in the bunny mesh and add to the scene
		for (int i = 0; i < mesh->numTris(); ++i)
		{
			Triangle* t = new Triangle;
			t->setIndex(i);
			t->setMesh(mesh);
			t->setMaterial(material);
			g_scene->addObject(t);
		}
	}


	inline Matrix4x4
	translate(float x, float y, float z)
	{
		Matrix4x4 m;
		m.setColumn4(Vector4(x, y, z, 1));
		return m;
	}


	inline Matrix4x4
	scale(float x, float y, float z)
	{
		Matrix4x4 m;
		m.m11 = x;
		m.m22 = y;
		m.m33 = z;
		return m;
	}

	// angle is in degrees
	inline Matrix4x4
	rotate(float angle, float x, float y, float z)
	{
		float rad = angle*(PI/180.);

		float x2 = x*x;
		float y2 = y*y;
		float z2 = z*z;
		float c = cos(rad);
		float cinv = 1-c;
		float s = sin(rad);
		float xy = x*y;
		float xz = x*z;
		float yz = y*z;
		float xs = x*s;
		float ys = y*s;
		float zs = z*s;
		float xzcinv = xz*cinv;
		float xycinv = xy*cinv;
		float yzcinv = yz*cinv;

		Matrix4x4 m;
		m.set(x2 + c*(1-x2), xy*cinv+zs, xzcinv - ys, 0,
			  xycinv - zs, y2 + c*(1-y2), yzcinv + xs, 0,
			  xzcinv + ys, yzcinv - xs, z2 + c*(1-z2), 0,
			  0, 0, 0, 1);
		return m;
	}

} // namespace
