#include <stdio.h>
#include <stdlib.h>
#include "Miro.h"
#include "Camera.h"
#include "Image.h"
#include "Scene.h"
#include "Console.h"
#include "OpenGL.h"

Camera * g_camera = 0;

static bool firstRayTrace = true;


Camera::Camera() :
	m_bgColor(0,0,0),
	m_renderer(RENDER_OPENGL),
	m_eye(0,0,0),
	m_viewDir(0,0,-1),
	m_up(0,1,0),
	m_lookAt(FLT_MAX, FLT_MAX, FLT_MAX),
	m_fov(45),
	m_distance(0.038627416998),
	m_focalDistance(0.1),
	m_lensSize(0.01),
	m_isLens(false)
{
	calcLookAt();
}


Camera::~Camera()
{

}


void
Camera::click(Scene* pScene, Image* pImage)
{
	calcLookAt();
	static bool firstRayTrace = false;
	static bool firstBlurRayTrace = false;

	if (m_renderer == RENDER_OPENGL)
	{
		glDrawBuffer(GL_BACK);
		pScene->openGL(this);
		firstRayTrace = true;
		firstBlurRayTrace = true;
	}
	else if (m_renderer == RENDER_RAYTRACE)
	{
		firstBlurRayTrace = true;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glDrawBuffer(GL_FRONT);
		if (firstRayTrace)
		{
			pImage->clear(bgColor());
			pScene->raytraceImage(this, g_image);
			firstRayTrace = false;
		}

		g_image->draw();
	}
	else if (m_renderer == RENDER_BLURRAYTRACE)
	{
		firstRayTrace = true;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glDrawBuffer(GL_FRONT);
		if (firstBlurRayTrace)
		{
			pImage->clear(bgColor());
			pScene->raytraceBlurImage(this, g_image);
			firstBlurRayTrace = false;
		}

		g_image->draw();
	}
}


void
Camera::calcLookAt()
{
	// this is true when a "lookat" is not used in the config file
	if (m_lookAt.x != FLT_MAX)
	{
		setLookAt(m_lookAt);
		m_lookAt.set(FLT_MAX, FLT_MAX, FLT_MAX);
	}
}


void
Camera::drawGL()
{
	// set up the screen with our camera parameters
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov(), g_image->width() / (float)g_image->height(), 0.01, 10000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	Vector3 vCenter = eye() + viewDir();
	gluLookAt(eye().x,   eye().y,   eye().z,
			  vCenter.x, vCenter.y, vCenter.z,
			  up().x,    up().y,    up().z);
}


Ray
Camera::eyeRay(int x, int y, int imageWidth, int imageHeight, float offset_x, float offset_y) //offset is for subpixeel sampling (Path Tracing)
{
	// first compute the camera coordinate system
	// ------------------------------------------
	const Vector3 wDir = Vector3(-m_viewDir).normalize(); //レンズの向き(奥=負)
	const Vector3 uDir = cross(m_up, wDir).normalize(); //x軸
	const Vector3 vDir = cross(wDir, uDir);

	// next compute the pixel location in the world coordinate system using the camera coordinate system
	// --------------------------------------------------------
	const float aspectRatio = (float)imageWidth / (float)imageHeight;
	const float imPlaneUPos = -((x + 0.5f + offset_x) / (float)imageWidth - 0.5f); //-0.5 ~ -0.5
	const float imPlaneVPos = -((y + 0.5f + offset_y) / (float)imageHeight - 0.5f);
	const Vector3 pixelPos = m_eye + (aspectRatio * FILM_SIZE * imPlaneUPos) * uDir + (FILM_SIZE * imPlaneVPos) * vDir + m_distance * wDir;
	if(m_isLens)
	{
		//m_eye = レンズの中心
		const Vector3 dir = (m_eye - pixelPos).normalize();
		const Vector3 focalPoint = pixelPos + dir * m_focalDistance * dot(-wDir, dir); //dot is minus, rayの集まる場所
		float point_x;
		float point_y;
		while(true) {
			point_x = (2.0f * rand() / RAND_MAX - 1.0f) * m_lensSize;
			point_y = (2.0f * rand() / RAND_MAX - 1.0f) * m_lensSize;
			if(point_x * point_x + point_y * point_y <= m_lensSize * m_lensSize) {
				break;
			}
		}
		const Vector3 lensPoint = m_eye + uDir * point_x + vDir * point_y;
		return Ray(lensPoint, (focalPoint - lensPoint).normalize());
	}
	else
	{
		return Ray(pixelPos, (m_eye - pixelPos).normalize());
	}
}
