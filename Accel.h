#ifndef MIRO_ACCEL_H_INCLUDED
#define MIRO_ACCEL_H_INCLUDED

#include "Miro.h"
#include "Object.h"
#include "BVH.h"

extern int numberOfTraversals;
extern int numberOfRayObjectIntersections;


class Accel
{
public:
	void build(Objects * objs, bool isDisplay = true);
	bool intersect(HitInfo& result, const Ray& ray, float tMin = 0.0f, float tMax = MIRO_TMAX, bool isUpdate = true, const Object* avoidObejct = nullptr) const;
protected:
	Objects * m_objects;
	BVH m_BVH;
};

#endif // MIRO_ACCEL_H_INCLUDED
