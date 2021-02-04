#ifndef MIRO_MATERIAL_H_INCLUDED
#define MIRO_MATERIAL_H_INCLUDED

#include "Miro.h"
#include "Vector3.h"

class Material
{
public:
	Material();
	virtual ~Material();

	virtual void preCalc() {}

	virtual Vector3 shade(const Ray& ray, const HitInfo& hit, const Scene& scene, int depth = 0, int E = 1) const;
// protected:
// 	const int N = 100;
};

#endif // MIRO_MATERIAL_H_INCLUDED
