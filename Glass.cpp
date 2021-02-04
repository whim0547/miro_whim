#include "Glass.h"
#include "Ray.h"
#include "Scene.h"
#include <algorithm>

Glass::Glass(const Vector3 & kt, const float eta) :
	m_kt(kt), m_eta(eta)
{
}

Glass::~Glass()
{
}

Vector3
Glass::shade(const Ray& ray, const HitInfo& hit, const Scene& scene, int depth, int E) const
{
	Vector3 L = Vector3(0.0f, 0.0f, 0.0f);

	const Vector3 viewDir = -ray.d; // d is a unit vector, 視点への方向(だから反転)

	const Lights *lightlist = scene.lights();

  Vector3 interpolated_normal = hit.N;
  Vector3 geometric_normal = hit.hitObject->getGeometriNormal(hit.P); //面の法線ベクトル != interploated normal
  geometric_normal = dot(geometric_normal, ray.d) > 0 ? -geometric_normal : geometric_normal;//dotが正なら反転
  geometric_normal.normalize();
  float ei_div_eo; // = etaIn / etaOut
  //float next_eta = 1.0f; //traceする際にhitInfo.tracelingEtaに入れる値

  float d = dot(ray.d, interpolated_normal);
  if(d < 0) {// outside to inside
    ei_div_eo = 1.0f / m_eta; //warning!!!空気からという前提(not water)
  } else { //オブジェクトの中身を通った
    ei_div_eo = m_eta; //warning!!!!! もしかしたら外のオブジェクトが空気じゃないかも
    // interpolated_normal = -interpolated_normal; //入射側に合わせる
  }

  Vector3 reflectDir = -2 * dot(ray.d, interpolated_normal) * interpolated_normal + ray.d; //反射したray

  float d_refl = dot(reflectDir, geometric_normal); //dが正 = 入射と面に対して同じ側(正しく反射) | 負 = 面に対して進入 →調整
  if(d_refl < 0) reflectDir = reflectDir - 2 * d * geometric_normal;

  // normalize the reflected direction
  reflectDir.normalize();
  float D = 1.0f - ei_div_eo * ei_div_eo * (1.0f - d * d);
  HitInfo hitInfo_refl;
  Ray reflectRay = Ray(hit.P, reflectDir);
  bool isHit_refl = depth < 10000 && scene.trace2(hitInfo_refl, reflectRay, hit.hitObject) /*avoid numerical error(self)*/;

  // get the irradiance
  // trace ray from hit to light if intersect other object then put shodow
  Vector3 irradiance_refl = 	isHit_refl ?
                              hitInfo_refl.material->shade(reflectRay, hitInfo_refl, scene, depth+1, E) : //鏡の先を取得
                              scene.getImage(reflectDir);
  Vector3 irradiance_refr = Vector3();
  bool isHit_refr = depth < 10000;
  float R = 1.0f;
  if(D >= 0.0f) { //全反射しなかった

    Vector3 refractDir = ei_div_eo * (ray.d - d * hit.N) - sqrt(D) * hit.N; //屈折した光

    float d_refr = dot(refractDir, geometric_normal); //dが正 = 入射と面に対して同じ側(反射) | 負 = 面に対して進入
    if(d_refr > 0) refractDir = refractDir - 2 * d * geometric_normal;

    // normalize the refracted direction
    refractDir.normalize();

    // get the irradiance
    // trace ray from hit to light if intersect other object then put shodow
    HitInfo hitInfo_refr;
    Ray refractRay = Ray(hit.P, refractDir);
    isHit_refr = isHit_refr && scene.trace2(hitInfo_refr, refractRay, hit.hitObject) /*avoid numerical error(self)*/;

    float cos_i = d > 0 ? d : -d;
    float cos_o = dot(refractDir, hit.N);
    cos_o = cos_o > 0 ? cos_o : -cos_o;
    float ps = (ei_div_eo * cos_i - cos_o) / (ei_div_eo * cos_i + cos_o);
    float pt = (ei_div_eo * cos_o - cos_i) / (ei_div_eo * cos_o + cos_i);
    R = (ps * ps + pt * pt) * 0.5f;//Frensel Reflection

    irradiance_refr = isHit_refr ?
                      hitInfo_refr.material->shade(ray, hitInfo_refr, scene, depth+1, E) : //鏡の先を取得
                      scene.getImage(refractDir);
  }
  L += R * irradiance_refl + (1.0f - R) * irradiance_refr;

  if(!isHit_refl || !isHit_refr) {
    // loop over all of the lights
  	Lights::const_iterator lightIter;
  	for (lightIter = lightlist->begin(); lightIter != lightlist->end(); lightIter++)
  	{
  		PointLight* pLight = *lightIter;

  		// the inverse-squared falloff
      float falloff = (pLight->position() - hit.P).length2();

      Vector3 irradiance = (pLight->color() * pLight->wattage()) * std::max(0.0f, dot(hit.N, (pLight->position() - hit.P) / falloff)) / (4.0f * PI * falloff);
      L += (1 - isHit_refl) * R * irradiance + (1 - isHit_refr) * (1 - R) * irradiance;
  	}
  }
  L *= m_kt;
	return L;
}
