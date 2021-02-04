#pragma once
#include "Vector3.h"
#include <algorithm>

class BoundingVolume {
public:
  BoundingVolume():m_min(std::numeric_limits<float>::infinity()), m_max(-std::numeric_limits<float>::infinity()) {}
  //BoundingVolume(Vector3 min, Vector3 max): m_min(min), m_max(max) {}
  BoundingVolume(Vector3 min, Vector3 max)
  {
    Vector3 true_min = Vector3(min.x < max.x ? min.x : max.x,
                               min.y < max.y ? min.y : max.y,
                               min.z < max.z ? min.z : max.z);
    Vector3 true_max = Vector3(min.x > max.x ? min.x : max.x,
                               min.y > max.y ? min.y : max.y,
                               min.z > max.z ? min.z : max.z);
    m_min = true_min;
    m_max = true_max;
  }

  float calcVolume() {
    if(m_min == std::numeric_limits<float>::infinity()) return 0.0f;
    return (m_max.x - m_min.x) * (m_max.y - m_min.y) * (m_max.z - m_min.z);
  }

  // ２つのBVを内包するBVを生成
  BoundingVolume operator+(const BoundingVolume& bv) const
	{
    Vector3 new_min = Vector3(std::min(m_min.x, bv.m_min.x),
                              std::min(m_min.y, bv.m_min.y),
                              std::min(m_min.z, bv.m_min.z));
    Vector3 new_max = Vector3(std::max(m_max.x, bv.m_max.x),
                              std::max(m_max.y, bv.m_max.y),
                              std::max(m_max.z, bv.m_max.z));
		return BoundingVolume(new_min, new_max);
	}

	const BoundingVolume & operator+=(const BoundingVolume& bv)
	{
    m_min = Vector3(std::min(m_min.x, bv.m_min.x),
                    std::min(m_min.y, bv.m_min.y),
                    std::min(m_min.z, bv.m_min.z));
    m_max = Vector3(std::max(m_max.x, bv.m_max.x),
                    std::max(m_max.y, bv.m_max.y),
                    std::max(m_max.z, bv.m_max.z));
    return *this;
	}

  //座標の小さい方のx,y,z
  Vector3 m_min;
  //座標の大きい方のx,y,z
  Vector3 m_max;
};
