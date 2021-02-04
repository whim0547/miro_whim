#include "Lambert.h"
#include "Ray.h"
#include "Scene.h"
#include <algorithm>

#include "my_rand.h"

std::uniform_real_distribution<float> dist(0.0f, 1.0f);

Lambert::Lambert(const Vector3 & kd, const bool is_pathtracing) :
	m_kd(kd), m_is_pathtracing(is_pathtracing)
{
	std::random_device seed_gen;
	engine.seed(seed_gen());
}

Lambert::~Lambert()
{
}

Vector3
Lambert::shade(const Ray& ray, const HitInfo& hit, const Scene& scene, int depth, int E) const
{

	Vector3 L = Vector3(0.0f, 0.0f, 0.0f);
	//std::cout << depth << ":" << hit.hitObject << std::endl;
	const Vector3 viewDir = -ray.d; // d is a unit vector

	if(depth > 10) return L;
	Vector3 oriented_N = dot(hit.N, ray.d) < 0 ? hit.N : -hit.N;
	float p = fmax(fmax(m_kd.x, m_kd.y), m_kd.z);
	Vector3 color = m_kd;
	// Russian Roulette
	if(depth > 5 || !p) {//!p = "p == 0.0f"
		if(uniform_0_1() >= p) return L;
		else color /= p;
	}

	const Lights *lightlist = scene.lights();

	// loop over all of the lights
	Lights::const_iterator lightIter;
	for (lightIter = lightlist->begin(); lightIter != lightlist->end(); lightIter++)
	{
		PointLight* pLight = *lightIter;
			Vector3 l = pLight->position() - hit.P;

			// the inverse-squared falloff
			float falloff = l.length2();

			// normalize the light direction
			l /= sqrt(falloff);

			// get the irradiance
			// trace ray from hit to light if intersect other object then put shodow
			HitInfo hitInfo;
			Ray ray2 = Ray(hit.P, l);
			bool isHit = scene.trace2(hitInfo, ray2, hit.hitObject, 0.0f, sqrt(falloff)) /*avoid numerical error(self)*/; //includes Next-Event Estimation(path tracing)
			//std::cout << depth << ":" << hit.hitObject << std::endl;
			//bool isHit = scene.trace(hitInfo, ray) && hit.hitObject != hitInfo.hitObject /*avoid numerical error(self)*/;
			Vector3 irradiance = isHit ?
													 Vector3() : //drop shadow
													 (pLight->color() * pLight->wattage()) * fmax(0.0f, dot(hit.N, l)) / (4.0 * PI * falloff);
			// if(!isHit && hit.dist_to_light > l.length()) hit.dist_to_light = l.length();
			// std::cout << isHit << std::endl;

			// if(isHit) std::cout << hit.hitObject << " " << hitInfo.hitObject << std::endl;
			L += irradiance * (color / PI);
	}

	// std::cout << scene.hasImage() << " " << N << std::endl;
	if(scene.hasImage()) {
		Vector3 Ei = Vector3();
		// std::random_device seed_gen;
		// std::default_random_engine engine(seed_gen()); //->global

		for(int i = 0; i < 500; ++i) {
			Vector3 dir = generateRandomHemiDir(oriented_N);
			Ray ray3 = Ray(hit.P, dir);
			HitInfo hitInfo;
			bool isHit = scene.trace2(hitInfo, ray3, hit.hitObject) /*avoid numerical error(self)*/;
			if(!isHit) Ei += scene.getImage(dir) / 500;
		}
		L += Ei;
	}

	if(m_is_pathtracing) {
		HitInfo hitInfo;
		// Vector3 rand_refl_dir = generateRandomHemiDir(oriented_N, engine);
		Vector3 rand_refl_dir = generateRandomHemiDir(oriented_N);
		Ray ray3 = Ray(hit.P, rand_refl_dir);
		if(scene.trace2(hitInfo, ray3, hit.hitObject)) {
			//Vector3 brdf = m_kd / PI;
			L += hitInfo.material->shade(ray3, hitInfo, scene, depth+1, 0) * color/* brdf * PI*/; //2*PI means Hemisphecal random probabilty p(w_i)
		}
	}

	return L;
}
