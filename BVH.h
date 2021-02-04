#pragma once

#include "Miro.h"
#include "Object.h"

namespace Costs {
  extern const float cost_o;
  extern const float cost_t;
}

class Tree
{
public:
  Tree();
  Tree(Tree* r_node, Tree* l_node);
  Tree(Objects* objects);
  ~Tree();
  void addNode();
  void countNodesAndLeaves(int& n_counter, int& l_counter, int& o_counter);
  void PrintLeaves(int counter);

  float calcVolume();

  BoundingVolume m_BV;
  Tree* m_Rnode;
  Tree* m_Lnode;
  Objects m_leaf;
  //座標の小さい方のx,y,z
  // Vector3 m_min;
  // //座標の大きい方のx,y,z
  // Vector3 m_max;
};

// BoundingVolumeHierarchy
class BVH
{
public:
  void MakeBVH(Objects* objects);
  void countNodesAndLeaves(int& n_counter, int& l_counter, int& o_counter);
  void PrintLeaves();

  Tree* m_root;
};
