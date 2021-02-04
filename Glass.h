#pragma once

#include "Material.h"

// Specular Reflection
class Glass : public Material
{
public:
	Glass(const Vector3 & kt = Vector3(1), const float eta = 1.0f);
	virtual ~Glass();

	const Vector3 & kt() const {return m_kt;}

	void setkt(const Vector3 & kt) {m_kt = kt;}

  const float eta() const {return m_eta;}

	void seteta(const float eta) {m_eta = eta;}

	virtual void preCalc() {}

	virtual Vector3 shade(const Ray& ray, const HitInfo& hit, const Scene& scene, int depth = 0, int E = 1) const;

protected:
	Vector3 m_kt;
  float m_eta;
};
