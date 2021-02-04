#ifndef MIRO_LAMBERT_H_INCLUDED
#define MIRO_LAMBERT_H_INCLUDED

#include "Material.h"
#include <random>

class Lambert : public Material
{
public:
	Lambert(const Vector3 & kd = Vector3(1), const bool is_pathtracing = false);
	virtual ~Lambert();

	const Vector3 & kd() const {return m_kd;}

	void setKd(const Vector3 & kd) {m_kd = kd;}

	virtual void preCalc() {}

	virtual Vector3 shade(const Ray& ray, const HitInfo& hit, const Scene& scene, int depth = 0, int E = 1) const;

protected:
	Vector3 m_kd;
	bool m_is_pathtracing;
	std::default_random_engine engine;
};

#endif // MIRO_LAMBERT_H_INCLUDED
