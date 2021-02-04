#ifndef MIRO_OBJECT_H_INCLUDED
#define MIRO_OBJECT_H_INCLUDED

#include <vector>
#include "Miro.h"
#include "Material.h"
#include "BoundingVolume.h"

class Object
{
public:
	Object(): m_is_move(false) {}
	virtual ~Object() {}

	void setMaterial(const Material* m) {m_material = m;}

	virtual void renderGL() {}
	virtual void preCalc() {}

	virtual bool intersect(HitInfo& result, const Ray& ray, float tMin = 0.0f, float tMax = MIRO_TMAX) = 0;

	virtual BoundingVolume calcBoundingVolume() = 0;
	virtual Vector3 getGeometriNormal(Vector3 point) = 0;

	virtual void Move(Vector3 dist) {}
	virtual bool isMove() {return m_is_move;}
	virtual void setMove(bool f) {m_is_move = f;}
protected:
  const Material* m_material;
	bool m_is_move;
};

typedef std::vector<Object*> Objects;

#endif // MIRO_OBJECT_H_INCLUDED
