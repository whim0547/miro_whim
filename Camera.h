#ifndef MIRO_CAMERA_H_INCLUDED
#define MIRO_CAMERA_H_INCLUDED

#include "Vector3.h"
#include "Miro.h"
#include "Ray.h"

//32 * 32
#define FILM_SIZE 0.032
//512 * 512
//#define FILM_SIZE 0.00195

const float HalfDegToRad = DegToRad/2.0f;

class Camera
{
public:
	Camera();
	virtual ~Camera();

	enum
	{
		RENDER_OPENGL   = 0,
		RENDER_RAYTRACE = 1,
		RENDER_BLURRAYTRACE = 2
	};

	void click(Scene* pScene, Image* pImage);

	inline bool isOpenGL() const {return m_renderer == RENDER_OPENGL;}
	inline void setRenderer(int i) {m_renderer = i;}

	inline void setEye(float x, float y, float z);
	inline void setEye(const Vector3& eye);
	inline void setUp(float x, float y, float z);
	inline void setUp(const Vector3& up);
	inline void setViewDir(float x, float y, float z);
	inline void setViewDir(const Vector3& vd);
	inline void setLookAt(float x, float y, float z);
	inline void setLookAt(const Vector3& look);
	inline void setBGColor(float x, float y, float z);
	inline void setBGColor(const Vector3& color);
	inline void setFOV(float fov)
	{ // FOV is in degree not radian
		m_fov = fov;
		m_distance = FILM_SIZE / (2.0 * tan(fov * HalfDegToRad));
	}
	inline void setDistance(float distance)
	{ // FOV is in degree not radian
		m_distance = distance;
		m_fov = 2.0 * atan2(FILM_SIZE, 2.0 * distance) * RadToDeg;
	}
	inline void setFocalDistance(float d);
	inline void setLensSize(float r);
	inline void activeLensCamera(bool b);


	inline float fov() const                {return m_fov;}
	inline float distance() const           {return m_distance;}
	inline const Vector3 & viewDir() const  {return m_viewDir;}
	inline const Vector3 & lookAt() const   {return m_lookAt;}
	inline const Vector3 & up() const       {return m_up;}
	inline const Vector3 & eye() const      {return m_eye;}
	inline const Vector3 & bgColor() const  {return m_bgColor;}

	inline bool isLens() const {return m_isLens;}

	Ray eyeRay(int x, int y, int imageWidth, int imageHeight, float offset_x = 0.0f, float offset_y = 0.0f);

	void drawGL();

private:
	void calcLookAt();

	Vector3 m_bgColor;
	int m_renderer;

	// main screen params
	Vector3 m_eye;
	Vector3 m_up;
	Vector3 m_viewDir;
	Vector3 m_lookAt;
	float m_fov;
	float m_distance;

	//lens
	float m_focalDistance; //Distance to Focal Plane
	float m_lensSize;
	bool m_isLens;
};

extern Camera * g_camera;

//--------------------------------------------------------

inline void Camera::setEye(float x, float y, float z)
{
	m_eye.set(x, y, z);
}

inline void Camera::setEye(const Vector3& eye)
{
	setEye(eye.x, eye.y, eye.z);
}

inline void Camera::setUp(float x, float y, float z)
{
	m_up.set(x, y, z);
	m_up.normalize();
}

inline void Camera::setUp(const Vector3& up)
{
    setUp(up.x, up.y, up.z);
}

inline void Camera::setViewDir(float x, float y, float z)
{
	m_viewDir.set(x, y, z);
	m_viewDir.normalize();
}

inline void Camera::setViewDir(const Vector3& vd)
{
	setViewDir(vd.x, vd.y, vd.z);
}

inline void Camera::setLookAt(float x, float y, float z)
{
	Vector3 dir = Vector3(x, y, z) - m_eye;
	setViewDir(dir);
}

inline void Camera::setLookAt(const Vector3& vd)
{
	setLookAt(vd.x, vd.y, vd.z);
}

inline void Camera::setBGColor(float x, float y, float z)
{
	m_bgColor.set(x, y, z);
}

inline void Camera::setBGColor(const Vector3& vd)
{
	setBGColor(vd.x, vd.y, vd.z);
}

inline void Camera::setFocalDistance(float d)
{
	m_focalDistance = d;
}

inline void Camera::setLensSize(float r)
{
	m_lensSize = r;
}

inline void Camera::activeLensCamera(bool b)
{
	m_isLens = b;
}


#endif // MIRO_CAMERA_H_INCLUDED
