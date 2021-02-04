#include "Miro.h"
#include "Scene.h"
#include "Camera.h"
#include "Image.h"
#include "Console.h"
#include <time.h>

Scene * g_scene = 0;
Vector3 filmResponse(const Vector3 rawImage, float scale = 1.0f);

void
Scene::openGL(Camera *cam)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cam->drawGL();

	// draw objects
	for (size_t i = 0; i < m_objects.size(); ++i)
	{
		m_objects[i]->renderGL();
	}

	glutSwapBuffers();
}

void
Scene::preCalc(bool isDisplay)
{
	Objects::iterator it;
	for (it = m_objects.begin(); it != m_objects.end(); it++)
	{
		Object* pObject = *it;
		pObject->preCalc();
	}
	Lights::iterator lit;
	for (lit = m_lights.begin(); lit != m_lights.end(); lit++)
	{
		PointLight* pLight = *lit;
		pLight->preCalc();
	}

	m_accel.build(&m_objects, isDisplay);
}

void
Scene::raytraceImage(Camera *cam, Image *img)
{
	Ray ray;
	HitInfo hitInfo;
	Vector3 shadeResult;
	int numberOfIntersections = 0;


	// loop over all pixels in the image
	clock_t start = clock();
	for (int j = 0; j < img->height(); ++j)
	{
		for (int i = 0; i < img->width(); ++i)
		{
			if(m_is_subpixelsampling) {
				int N = 10;
				Vector3 shadeResult = Vector3();
				for (int sx = 0; sx < 2; ++sx) {
					Vector3 shadeResult_tmp = Vector3();
					for(int sy = 0; sy < 2; ++sy) {
						for(int k = 0; k < N; ++k) {
							float r1 = 2.0f * rand() / RAND_MAX;
							float offset_x = r1 < 1 ? sqrt(r1) - 1.0f : 1.0f - sqrt(2.0f - r1); //[-1, 1]
							float r2 = 2.0f * rand() / RAND_MAX;
							float offset_y = r2 < 1 ? sqrt(r2) - 1.0f : 1.0f - sqrt(2.0f - r2);
							ray = cam->eyeRay(i, j, img->width(), img->height(), (offset_x + sx + 0.5f) * 0.5f , (offset_y + sy + 0.5f) * 0.5f);
							if (trace(hitInfo, ray))
							{
								shadeResult_tmp += hitInfo.material->shade(ray, hitInfo, *this) / N;
								++numberOfIntersections;
							}
							else if(m_image != nullptr)
							{
								shadeResult_tmp += getImage(ray.d) / N;
							}
						}
					}
					shadeResult += clamp(shadeResult_tmp, 0.0f, 1.0f) * 0.25f; // 0.25 = 1 / num_of_sumpixels(=2*2)
				}
				// shadeResult *= 0.25f; // 0.25 = 1 / num_of_sumpixels(=2*2)
				// shadeResult *= 0.25f; // 0.25 = 1 / num_of_sumpixels(=2*2)
				img->setPixel(i, j, shadeResult);
			} else {
				ray = cam->eyeRay(i, j, img->width(), img->height());
				if (trace(hitInfo, ray))
				{
					shadeResult = hitInfo.material->shade(ray, hitInfo, *this);
					img->setPixel(i, j, shadeResult);
					++numberOfIntersections;
				}
				else if(m_image != nullptr)
				{
					shadeResult = getImage(ray.d);
					img->setPixel(i, j, shadeResult);
				}
				if(cam->isLens()) {
					int N = 99;
					for(int k = 0; k < N; ++k) {
						ray = cam->eyeRay(i, j, img->width(), img->height());
						if (trace(hitInfo, ray))
						{
							shadeResult += hitInfo.material->shade(ray, hitInfo, *this);
							++numberOfIntersections;
						}
						else if(m_image != nullptr)
						{
							shadeResult += getImage(ray.d);
						}
					}
					shadeResult /= N;
					img->setPixel(i, j, shadeResult);
				}
			}
		}
		img->drawScanline(j);
		glFinish();
		printf("Rendering Progress: %.3f%%, Intersections: %d\r", j/float(img->height())*100.0f, numberOfIntersections, numberOfTraversals);
		fflush(stdout);
	}
	clock_t end = clock();
	printf("Rendering Progress: 100.000%\n");
	const float sec = static_cast<float>(end - start) / CLOCKS_PER_SEC;
	const int min = static_cast<int>(sec / 60);
	printf("Rendering Time: %d:%.2f, ", min, sec - min * 60.0f);
	printf("Ray-Triangle Intersections: %d, Traversals: %d\n", numberOfRayObjectIntersections, numberOfTraversals);
	debug("done Raytracing!\n");
}

void
Scene::raytraceBlurImage(Camera *cam, Image *img)
{
	Ray ray;
	HitInfo hitInfo;
	Vector3 shadeResult;
	Vector3 shadeResults[img->width()][img->height()];
	int numberOfIntersections = 0;
	int TIME = 20;
	Objects::iterator it;

	// loop over all pixels in the image
	clock_t start = clock();
	for (int t = 0; t < TIME; ++t) {
		for (int j = 0; j < img->height(); ++j)
		{
			for (int i = 0; i < img->width(); ++i)
			{
				if(t == 0) shadeResults[i][j] = Vector3();
				if(m_is_subpixelsampling) {
					int N = 10;
					Vector3 shadeResult = Vector3();
					for (int sx = 0; sx < 2; ++sx) {
						Vector3 shadeResult_tmp = Vector3();
						for(int sy = 0; sy < 2; ++sy) {
							for(int k = 0; k < N; ++k) {
								float r1 = 2.0f * rand() / RAND_MAX;
								float offset_x = r1 < 1 ? sqrt(r1) - 1.0f : 1.0f - sqrt(2.0f - r1); //[-1, 1]
								float r2 = 2.0f * rand() / RAND_MAX;
								float offset_y = r2 < 1 ? sqrt(r2) - 1.0f : 1.0f - sqrt(2.0f - r2);
								ray = cam->eyeRay(i, j, img->width(), img->height(), (offset_x + sx + 0.5f) * 0.5f , (offset_y + sy + 0.5f) * 0.5f);
								if (trace(hitInfo, ray))
								{
									shadeResult_tmp += hitInfo.material->shade(ray, hitInfo, *this) / N;
									++numberOfIntersections;
								}
								else if(m_image != nullptr)
								{
									shadeResult_tmp += getImage(ray.d) / N;
								}
							}
						}
						shadeResult += clamp(shadeResult_tmp, 0.0f, 1.0f) * 0.25f; // 0.25 = 1 / num_of_sumpixels(=2*2)
					}
					// shadeResult *= 0.25f; // 0.25 = 1 / num_of_sumpixels(=2*2)
					// shadeResult *= 0.25f; // 0.25 = 1 / num_of_sumpixels(=2*2)
					shadeResults[i][j] += shadeResult;
					img->setPixel(i, j, shadeResults[i][j]  / (t+1));
				} else {
					ray = cam->eyeRay(i, j, img->width(), img->height());
					if (trace(hitInfo, ray))
					{
						shadeResult = hitInfo.material->shade(ray, hitInfo, *this);
						shadeResults[i][j] += shadeResult;
						img->setPixel(i, j, shadeResults[i][j]  / (t+1));
						++numberOfIntersections;
					}
					else if(m_image != nullptr)
					{
						shadeResult = getImage(ray.d);
						shadeResults[i][j] += shadeResult;
						img->setPixel(i, j, shadeResults[i][j]  / (t+1));
					}
					if(cam->isLens()) {
						int N = 99;
						for(int k = 0; k < N; ++k) {
							ray = cam->eyeRay(i, j, img->width(), img->height());
							if (trace(hitInfo, ray))
							{
								shadeResult += hitInfo.material->shade(ray, hitInfo, *this);
								++numberOfIntersections;
							}
							else if(m_image != nullptr)
							{
								shadeResult += getImage(ray.d);
							}
						}
						shadeResult /= N;
						shadeResults[i][j] += shadeResult;
						img->setPixel(i, j, shadeResults[i][j]  / (t+1));
					}
				}

			}
			img->drawScanline(j);
			glFinish();
			printf("Time: %d, Rendering Progress: %.3f%%, Intersections: %d\r", t, j/float(img->height())*100.0f, numberOfIntersections, numberOfTraversals);
			fflush(stdout);
		}

		//update position
		Vector3 v = Vector3(sin((t + 1) * 1.0f / TIME * PI / 4) - sin(t * 1.0f / TIME * PI / 4), cos((t + 1) * 1.0f / TIME * PI / 4) - cos(t * 1.0f / TIME * PI / 4), 0.0f);
		for (it = m_objects.begin(); it != m_objects.end(); it++)
		{
			Object* pObject = *it;
			if(pObject->isMove()) {
				pObject->Move(v);
			}
		}
		preCalc(false);
	}
	clock_t end = clock();
	printf("Rendering Progress: 100.000%\n");
	const float sec = static_cast<float>(end - start) / CLOCKS_PER_SEC;
	const int min = static_cast<int>(sec / 60);
	printf("Rendering Time: %d:%.2f, ", min, sec - min * 60.0f);
	printf("Ray-Triangle Intersections: %d, Traversals: %d\n", numberOfRayObjectIntersections, numberOfTraversals);
	debug("done Raytracing!\n");
}


bool
Scene::trace(HitInfo& minHit, const Ray& ray, bool isUpdatePrevHit, float tMin, float tMax) const
{
	return m_accel.intersect(minHit, ray, tMin, tMax, isUpdatePrevHit);
}

bool
Scene::trace2(HitInfo& minHit, const Ray& ray, const Object* avoidObject, float tMin, float tMax) const
{
	return m_accel.intersect(minHit, ray, tMin, tMax, false, avoidObject);
}

void Scene::addImage(Vector3* img, int width, int height)
{
	m_image = img;
	m_img_width = width;
	m_img_height = height;
}

Vector3 Scene::getImage(int x, int y)const
{
	if (m_image != nullptr && x >= 0 && x < m_img_width && y < m_img_height && y >= 0)
	{
		return filmResponse(m_image[y*m_img_width+x]);
	}
	else
	{
		return Vector3();//bgColor
	}
}

Vector3 Scene::getImage(Vector3 dir) const
{
	if(m_image == nullptr) return Vector3();
	dir.normalize();
	float r = (dir.x * dir.x + dir.y * dir.y) == 0.0f ? 0.0f : acos(dir.z) / sqrt(dir.x * dir.x + dir.y * dir.y) / PI;
	float u = (dir.x * r + 1.0f) * 0.5f; //[-1, 1] -> [0, 1]
	float v = (dir.y * r + 1.0f) * 0.5f;
 	int x = std::min(std::max(static_cast<int>(u * m_img_width), 0), m_img_width - 1);
	int y = std::min(std::max(static_cast<int>(v * m_img_height), 0), m_img_height - 1);
	Vector3 rawImage = m_image[y*m_img_width+x];
	return filmResponse(rawImage);
	// return rawImage;
}

bool Scene::hasImage() const
{
	return m_image != nullptr;
}

Vector3 filmResponse(const Vector3 raw, float scale)
{
	float r = 1.0f - exp(-raw.x * scale);
	float g = 1.0f - exp(-raw.y * scale);
	float b = 1.0f - exp(-raw.z * scale);
	return Vector3(r, g, b);
}
