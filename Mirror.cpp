#include "Mirror.h"
#include "Ray.h"
#include "Scene.h"
#include <algorithm>

Mirror::Mirror(const Vector3 & ks) :
	m_ks(ks)
{
}

Mirror::~Mirror()
{
}

Vector3
Mirror::shade(const Ray& ray, const HitInfo& hit, const Scene& scene, int depth, int E) const
{
	Vector3 L = Vector3(0.0f, 0.0f, 0.0f);

	//const Vector3 viewDir = -ray.d; // d is a unit vector, 視点への方向(だから反転)

  Vector3 interpolated_normal = hit.N;
  Vector3 geometric_normal = hit.hitObject->getGeometriNormal(hit.P); //面の法線ベクトル != interploated normal
  geometric_normal = dot(geometric_normal, ray.d) > 0 ? -geometric_normal : geometric_normal;//dotが正なら反転
  geometric_normal.normalize();

  Vector3 l = -2 * dot(ray.d, interpolated_normal) * interpolated_normal + ray.d; //traceするray
  float d = dot(l, geometric_normal); //dが正 = 入射と面に対して同じ側(正しく反射) | 負 = 面に対して進入 →調整
  if(d < 0) l = l - 2 * d * geometric_normal;

  // normalize the reflected direction
  l.normalize();

  // get the irradiance
  // trace ray from hit to light if intersect other object then put shodow
  HitInfo hitInfo;
  Ray ray2 = Ray(hit.P, l);
  bool isHit = depth < 100 && scene.trace2(hitInfo, ray2, hit.hitObject) /*avoid numerical error(self)*/;

  L += isHit ?
       m_ks * hitInfo.material->shade(ray2, hitInfo, scene, depth+1, E) : //鏡の先を取得
       scene.getImage(l);

  if(!isHit) {
    const Lights *lightlist = scene.lights();

  	// loop over all of the lights
  	Lights::const_iterator lightIter;

  	for (lightIter = lightlist->begin(); lightIter != lightlist->end(); lightIter++)
  	{
  		PointLight* pLight = *lightIter;

  		// the inverse-squared falloff
      float falloff = (pLight->position() - hit.P).length2();

      Vector3 irradiance = (pLight->color() * pLight->wattage()) * std::max(0.0f, dot(hit.N, (pLight->position() - hit.P) / falloff)) / (4.0f * PI * falloff);
      L += irradiance * m_ks;
  	}
  }

	return L;
}
