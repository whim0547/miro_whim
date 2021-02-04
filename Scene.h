#ifndef MIRO_SCENE_H_INCLUDED
#define MIRO_SCENE_H_INCLUDED

#include "Miro.h"
#include "Object.h"
#include "PointLight.h"
#include "Accel.h"

#include "Image.h"

class Camera;
class Image;

class Scene
{
public:
	 Scene(): m_image(nullptr), m_img_width(0), m_img_height(0), m_is_subpixelsampling(false){}
	 ~Scene();

	void addObject(Object* pObj)   {m_objects.push_back(pObj);}
	const Objects* objects() const {return &m_objects;}

	void addLight(PointLight* pObj) {m_lights.push_back(pObj);}
	const Lights* lights() const    {return &m_lights;}

	void setSubpixelsampling() {m_is_subpixelsampling = true;}
	void unsetSubpixelsampling() {m_is_subpixelsampling = false;}

	void preCalc(bool isDisplay = true);
	void openGL(Camera *cam);

	void raytraceImage(Camera *cam, Image *img);
	void raytraceBlurImage(Camera *cam, Image *img);
	bool trace(HitInfo& minHit, const Ray& ray, bool isUpdatePrevHit = true, float tMin = 0.0f, float tMax = MIRO_TMAX) const;
	bool trace2(HitInfo& minHit, const Ray& ray, const Object* avoidObject ,float tMin = 0.0f, float tMax = MIRO_TMAX) const;

	void addImage(Vector3* img, int width, int height);
	Vector3 getImage(int x, int y) const;
	Vector3 getImage(Vector3 dir) const;
	bool hasImage() const;

protected:
	Objects m_objects;
	Accel m_accel;
	Lights m_lights;
	Vector3* m_image;
	int m_img_width;
	int m_img_height;
	bool m_is_subpixelsampling;
};

extern Scene * g_scene;

#endif // MIRO_SCENE_H_INCLUDED
