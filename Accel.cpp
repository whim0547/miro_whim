#include "Accel.h"
#include "Ray.h"
#include <algorithm>
#include <iostream>
#include <time.h>

bool overlap(const HitInfo& minHit, const Ray& ray, Tree* t);
bool traverse(HitInfo& minHit, const Ray& ray, float tMin, float tMax, Tree* t, bool isUpdate, const Object* avoidObject);

Object* prev_hit = nullptr;
int numberOfTraversals = 0;
int numberOfRayObjectIntersections = 0;

void
Accel::build(Objects * objs, bool isDisplay)
{
	// construct the bounding volume hierarchy
	m_objects = objs;
	if(isDisplay) std::cout << "BVH start" << std::endl;
	clock_t start = clock();
	m_BVH.MakeBVH(objs);
	clock_t end = clock();
	const float sec = static_cast<float>(end - start) / CLOCKS_PER_SEC;
	const int min = static_cast<int>(sec / 60);
	if(isDisplay) {
		int n=0,l=0,o=0;
		m_BVH.countNodesAndLeaves(n, l, o);
		std::cout << "Construction Time: " << min << ":" << sec - min * 60.0f << std::endl;
		std::cout << "Nodes: " << n << ", Leaves: " << l << ", Objects: " << o << std::endl;
		// m_BVH.PrintLeaves();
	}
}


bool
Accel::intersect(HitInfo& minHit, const Ray& ray, float tMin, float tMax, bool isUpdate, const Object* avoidObject) const
{
	// Here you would need to traverse the acceleration data structure to perform ray-intersection
	// acceleration. For now we just intersect every object.
	bool hit = false;
	minHit.t = MIRO_TMAX;
	HitInfo tempMinHit;
	if(isUpdate && prev_hit != nullptr && prev_hit != avoidObject && prev_hit->intersect(tempMinHit, ray, tMin, tMax)) {
		if (tempMinHit.t < minHit.t)
		{
			++numberOfRayObjectIntersections;
			minHit = tempMinHit;
			minHit.hitObject = prev_hit;
			hit = true;
		}
	}
	hit = (overlap(minHit, ray, m_BVH.m_root) && traverse(minHit, ray, tMin, tMax, m_BVH.m_root, isUpdate, avoidObject)) || hit;
	if(!hit && isUpdate) prev_hit = nullptr;


	// !!!!! Not Accelerating !!!!!!
	// bool hit = false;
	// HitInfo tempMinHit;
	// minHit.t = MIRO_TMAX;
	// for (size_t i = 0; i < m_objects->size(); ++i)
	// {
	// 	if ((*m_objects)[i]->intersect(tempMinHit, ray, tMin, tMax))
	// 	{
	// 		hit = true;
	// 		++numberOfRayObjectIntersections;
	// 		if (tempMinHit.t < minHit.t)
	// 			minHit = tempMinHit;
	// 	}
	// }

	return hit;
}

bool traverse(HitInfo& minHit, const Ray& ray, float tMin, float tMax, Tree* t, bool isUpdate = true, const Object* avoidObject = nullptr) {
	++numberOfTraversals;
	bool hit = false;
	if(t->m_Rnode == nullptr) { // = Leaf
		HitInfo tempMinHit;
		for (size_t i = 0; i < t->m_leaf.size(); ++i)
		{
			if (t->m_leaf[i] == avoidObject) continue;
			if (t->m_leaf[i]->intersect(tempMinHit, ray, tMin, tMax))
			{
				++numberOfRayObjectIntersections;
				if (tempMinHit.t < minHit.t)
				{
					hit = true;
					minHit = tempMinHit;
					minHit.hitObject = t->m_leaf[i];
					if(isUpdate) prev_hit = t->m_leaf[i];
				}
			}
		}
	} else {
		if(overlap(minHit, ray, t->m_Rnode))
			hit = traverse(minHit, ray, tMin, tMax, t->m_Rnode, isUpdate, avoidObject) || hit;
		if(overlap(minHit, ray, t->m_Lnode))
			hit = traverse(minHit, ray, tMin, tMax, t->m_Lnode, isUpdate, avoidObject) || hit;
	}
	return hit;
}

bool overlap(const HitInfo& minHit, const Ray& ray, Tree* t)
{
	// Vector3 bv_min = t->m_min;
	// Vector3 bv_max = t->m_max;
	Vector3 bv_min = t->m_BV.m_min;
	Vector3 bv_max = t->m_BV.m_max;
	Vector3 t0 = (bv_min - ray.o) / ray.d;
	Vector3 t1 = (bv_max - ray.o) / ray.d;
	float t_min = std::max(std::max(std::min(t0.x, t1.x), std::min(t0.y, t1.y)), std::min(t0.z, t1.z));
	float t_max = std::min(std::min(std::max(t0.x, t1.x), std::max(t0.y, t1.y)), std::max(t0.z, t1.z));
	return t_min <= t_max && (t_min > 0 || t_max > 0) && minHit.t > t_min;
}
