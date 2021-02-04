#pragma once

#include "Material.h"

// Specular Reflection
class Mirror : public Material
{
public:
	Mirror(const Vector3 & ks = Vector3(1));
	virtual ~Mirror();

	const Vector3 & ks() const {return m_ks;}

	void setKs(const Vector3 & ks) {m_ks = ks;}

	virtual void preCalc() {}

	virtual Vector3 shade(const Ray& ray, const HitInfo& hit, const Scene& scene, int depth = 0, int E = 1) const;

protected:
	Vector3 m_ks;
};
