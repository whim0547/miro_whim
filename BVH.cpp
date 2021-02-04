#include "BVH.h"
#include <limits>
#include <vector>
#include <algorithm>
#include <cmath>

#include <iostream>

namespace Log {
  int object_data;
  int done_data = 0;
  int tree_depth = 0;
  int whole_tree_depth = 0;
}

namespace Costs {
  const float cost_o = 1.0f;
  const float cost_t = 1.0f;
}

const int CUT = 16;



BoundingVolume makeBoundingVolume(const Objects* objects);
Tree* subdivision(Objects* objects);

Tree::Tree(){}

Tree::Tree(Tree* r_node, Tree* l_node) : m_Rnode(r_node), m_Lnode(l_node), m_leaf(){
  // Vector3 min = Vector3(std::min(r_node->m_min.x, l_node->m_min.x),
  //                       std::min(r_node->m_min.y, l_node->m_min.y),
  //                       std::min(r_node->m_min.z, l_node->m_min.z));
  // Vector3 max = Vector3(std::max(r_node->m_max.x, l_node->m_max.x),
  //                       std::max(r_node->m_max.y, l_node->m_max.y),
  //                       std::max(r_node->m_max.z, l_node->m_max.z));
  // this->m_min = min;
  // this->m_max = max;
  Vector3 min = Vector3(std::min(r_node->m_BV.m_min.x, l_node->m_BV.m_min.x),
                        std::min(r_node->m_BV.m_min.y, l_node->m_BV.m_min.y),
                        std::min(r_node->m_BV.m_min.z, l_node->m_BV.m_min.z));
  Vector3 max = Vector3(std::max(r_node->m_BV.m_max.x, l_node->m_BV.m_max.x),
                        std::max(r_node->m_BV.m_max.y, l_node->m_BV.m_max.y),
                        std::max(r_node->m_BV.m_max.z, l_node->m_BV.m_max.z));
  m_BV = BoundingVolume(min, max);
}
Tree::Tree(Objects* objects) :m_Rnode(nullptr), m_Lnode(nullptr) ,m_leaf(*objects){
  m_BV = makeBoundingVolume(objects);
  // Vector3 min = Vector3(std::numeric_limits<float>::infinity());
  // Vector3 max = Vector3(-std::numeric_limits<float>::infinity());
  // for(size_t i = 0; i < objects->size(); ++i) {
  //   BoundingVolume bv = (*objects)[i]->calcBoundingVolume();
  //   Vector3 i_min = bv.m_min;
  //   Vector3 i_max = bv.m_max;
  //   min.x = std::min(min.x, i_min.x);
  //   min.y = std::min(min.y, i_min.y);
  //   min.z = std::min(min.z, i_min.z);
  //   max.x = std::max(max.x, i_max.x);
  //   max.y = std::max(max.y, i_max.y);
  //   max.z = std::max(max.z, i_max.z);
  // }
  // m_min = min;
  // m_max = max;
  //m_BV = BoundingVolume(m_min, m_max);
}

Tree::~Tree() {}

void Tree::countNodesAndLeaves(int &n_counter, int &l_counter, int &o_counter) {
  if(m_Rnode == nullptr) { //count leaves
    o_counter+=m_leaf.size();
    l_counter++;
  } else {
    n_counter++;
    m_Rnode->countNodesAndLeaves(n_counter, l_counter, o_counter);
    m_Lnode->countNodesAndLeaves(n_counter, l_counter, o_counter);
  }
}


float Tree::calcVolume() {
  //return (m_max.x - m_min.x) * (m_max.y - m_min.y) * (m_max.z - m_min.z);
  return m_BV.calcVolume();
}



void BVH::MakeBVH(Objects* objects) {
  Log::object_data = objects->size();
  m_root = subdivision(objects);
  std::cout << std::endl;
}

void BVH::countNodesAndLeaves(int &n_counter, int &l_counter, int &o_counter) {
  m_root->countNodesAndLeaves(n_counter, l_counter, o_counter);
}

void Tree::PrintLeaves(int counter) {
  if(m_Rnode == nullptr) { //count leaves
    std::cout << counter << "\t" << m_leaf.size() << ":" << m_BV.m_min << ":" << m_BV.m_max << std::endl;
  } else {
    std::cout << "Node" << counter << ":" << m_BV.m_min << ":" << m_BV.m_max << std::endl;
    m_Rnode->PrintLeaves(counter+1);;
    m_Lnode->PrintLeaves(counter+1);
  }
}

void BVH::PrintLeaves() {
  m_root->PrintLeaves(0);
}
/*
//axis: 0->x, 1->y, 2->z
void merge(Objects* objects1, Objects* objects2, Objects* objects, const int axis) {
  size_t i = 0, j = 0;
  while(i < objects1->size() || j < objects2->size()) {
    if(j >= objects2->size()) {
      *((*objects)[i+j]) = *((*objects1)[i]);
      i++;
      continue;
    }
    if(i >= objects1->size()) {
      *((*objects)[i+j]) = *((*objects2)[j]);
      j++;
      continue;
    }
    Vector3 min1 = (*objects1)[i]->calcBoundingVolume().m_min;
    Vector3 min2 = (*objects2)[j]->calcBoundingVolume().m_min;
    bool flag =
      axis == 0 ? min1.x < min2.x :
      axis == 1 ? min1.y < min2.y :
                  min1.z < min2.z;
    if (flag) {
      *((*objects)[i+j]) = *((*objects1)[i]);
      i++;
    } else {
      *((*objects)[i+j]) = *((*objects2)[j]);
      j++;
    }
  }
}

//using merge sort
void SortObjects(Objects* objects, const int axis){
 if(objects->size() > 1){
   size_t split = objects->size() / 2;
   Objects objects1;
   Objects objects2;
   auto start = (*objects).begin();
   auto end = (*objects).end();
   objects1.resize(split);
   objects2.resize(objects->size() - split);
   std::copy(start, start + split, objects1.begin());
   std::copy(start + split, end, objects2.begin());
   SortObjects(&objects1, axis);
   SortObjects(&objects2, axis);
   merge(&objects1, &objects2, objects, axis);
 }
}
*/

BoundingVolume makeBoundingVolume(const Objects* objects) {
  Vector3 min = Vector3(std::numeric_limits<float>::infinity());
  Vector3 max = Vector3(-std::numeric_limits<float>::infinity());
  for(size_t i = 0; i < objects->size(); ++i) {
    BoundingVolume bv = (*objects)[i]->calcBoundingVolume();
    Vector3 i_min = bv.m_min;
    Vector3 i_max = bv.m_max;
    min.x = std::min(min.x, i_min.x);
    min.y = std::min(min.y, i_min.y);
    min.z = std::min(min.z, i_min.z);
    max.x = std::max(max.x, i_max.x);
    max.y = std::max(max.y, i_max.y);
    max.z = std::max(max.z, i_max.z);
  }
  return BoundingVolume(min, max);
}


float calcBoundingVolume(const Objects* objects) {
  if(objects->size() == 0) return 0.0f;
  // Vector3 min = Vector3(std::numeric_limits<float>::infinity());
  // Vector3 max = Vector3(-std::numeric_limits<float>::infinity());
  // for(size_t i = 0; i < objects->size(); ++i) {
  //   BoundingVolume bv = (*objects)[i]->calcBoundingVolume();
  //   Vector3 i_min = bv.m_min;
  //   Vector3 i_max = bv.m_max;
  //   min.x = std::min(min.x, i_min.x);
  //   min.y = std::min(min.y, i_min.y);
  //   min.z = std::min(min.z, i_min.z);
  //   max.x = std::max(max.x, i_max.x);
  //   max.y = std::max(max.y, i_max.y);
  //   max.z = std::max(max.z, i_max.z);
  // }
  //return (max.x - min.x) * (max.y - min.y) * (max.z - min.z);
  return makeBoundingVolume(objects).calcVolume();
}


/*
void Sweep(Objects* objects, std::vector<float>& BV_lower, std::vector<float>& BV_upper) {
  size_t size = objects->size();
  BV_lower.resize(size);
  BV_upper.resize(size);
  BoundingVolume lower = (*objects)[0]->calcBoundingVolume();
  BoundingVolume upper = (*objects)[size-1]->calcBoundingVolume();
  BV_lower[0] = lower.calcVolume();
  BV_upper[0] = upper.calcVolume();
  for(size_t i = 1; i < size; ++i) {
    BoundingVolume lower_tmp = (*objects)[i]->calcBoundingVolume();
    lower += lower_tmp;
    upper += (*objects)[size-1-i]->calcBoundingVolume();
    BV_lower[i] = lower.calcVolume();
    BV_upper[i] = upper.calcVolume();
  }
}

void checkSplitAxis(Objects* objects, Objects* split_min_1, Objects* split_min_2, float& C_min, bool& update, int axis) {
  SortObjects(objects, axis);
  auto start = (*objects).begin();
  auto end = (*objects).end();

  //Sweeping
  std::vector<float> BV_lower; //sortの下から, 体積が入る
  std::vector<float> BV_upper; //上から
  Sweep(objects, BV_lower, BV_upper);
  size_t size = objects->size();
  float ca = BV_lower[size - 1];
  size_t i_tmp = size;
  for(size_t i = 0; i < size - 1; ++i) {
    float c1 = BV_lower[i];
    float c2 = BV_upper[size - 1 - i];
    float C_cand = 2 * Costs::cost_t + Costs::cost_o * (i + 1) * c1 / ca + (size - i - 1) * c2 / ca;
    if(C_cand < C_min) {
      update = true;
      C_min = C_cand;
      i_tmp = i;
    }
  }
  if(i_tmp < size) { //更新された
    split_min_1->resize(i_tmp + 1);
    split_min_2->resize(size - 1 - i_tmp);
    std::copy(start, start + i_tmp + 1, split_min_1->begin());
    std::copy(start + i_tmp + 1, end, split_min_2->begin());
  }
}
*/

void Sweep(Objects* objects/*参照*/, float* BV_lower, float* BV_upper, Objects* Obs/*vectorポインタの配列*/, Vector3 min, Vector3 max) {
  size_t size = objects->size();
  Vector3 range = (max - min) / CUT;
  BoundingVolume BVs[CUT*3] = {};

  //objectsの各軸をCUT個に区分された基数ソート
  for(size_t k = 0; k < size; ++k) {
    BoundingVolume bv_tmp = (*objects)[k]->calcBoundingVolume();
    Vector3 id_float = (bv_tmp.m_min - min) / range;
    int index_x = std::max(std::min((int)std::floor(id_float.x), CUT - 1), 0);
    int index_y = std::max(std::min((int)std::floor(id_float.y), CUT - 1), 0) + CUT;
    int index_z = std::max(std::min((int)std::floor(id_float.z), CUT - 1), 0) + CUT * 2;

    (Obs[index_x]).push_back((*objects)[k]);
    (Obs[index_y]).push_back((*objects)[k]);
    (Obs[index_z]).push_back((*objects)[k]);
    BVs[index_x] += bv_tmp;
    BVs[index_y] += bv_tmp;
    BVs[index_z] += bv_tmp;
  }
  // x,y,xそれぞれindexまでのBVをとる(CUT-1番目は全部全体のBV)
  BoundingVolume BVs_lower[CUT*3] = {};
  BoundingVolume BVs_upper[CUT*3] = {};
  for(int i = 0; i < CUT; ++i) {
    int index_x = i;
    int index_y = i + CUT;
    int index_z = i + CUT * 2;
    for(int k = 0; k <= i; ++k) {
      BVs_lower[index_x] += BVs[k];
      BVs_lower[index_y] += BVs[k + CUT];
      BVs_lower[index_z] += BVs[k + CUT * 2];
    }
    for(int k = i + 1; k < CUT; ++k) {
      BVs_upper[index_x] += BVs[k];
      BVs_upper[index_y] += BVs[k + CUT];
      BVs_upper[index_z] += BVs[k + CUT * 2];
    }
  }
  for(int i = 0; i < CUT * 3; ++i) {
    BV_lower[i] = BVs_lower[i].calcVolume();
    BV_upper[i] = BVs_upper[i].calcVolume();
  }
}

void checkSplit(Objects* objects, Objects* split_min_1, Objects* split_min_2, float& C_min, bool& update) {
  //SortObjects(objects, axis);
  // Vector3 min = Vector3(std::numeric_limits<float>::infinity());
  // Vector3 max = Vector3(-std::numeric_limits<float>::infinity());
  // for(size_t i = 0; i < objects->size(); ++i) {
  //   BoundingVolume bv = (*objects)[i]->calcBoundingVolume();
  //   Vector3 i_min = bv.m_min;
  //   Vector3 i_max = bv.m_max;
  //   min.x = std::min(min.x, i_min.x);
  //   min.y = std::min(min.y, i_min.y);
  //   min.z = std::min(min.z, i_min.z);
  //   max.x = std::max(max.x, i_max.x);
  //   max.y = std::max(max.y, i_max.y);
  //   max.z = std::max(max.z, i_max.z);
  // }
  BoundingVolume BV = makeBoundingVolume(objects);


  //Sweeping
  float BV_lower[CUT*3]; //下から, 体積が入る x,y,zに16ずつのバッファで基数ソート  0 1 | 2 ... CUT の左部 0番目が0と1の境界でCUT番目は右端なので無意味
  float BV_upper[CUT*3]; //上から, 体積が入る x,y,zに16ずつのバッファで基数ソート
  Objects Obs[CUT*3]; //上に対応
  Sweep(objects, BV_lower, BV_upper, Obs, BV.m_min, BV.m_max);//ここでObsに基数ソート結果を入れる

  size_t size = objects->size();
  float ca = calcBoundingVolume(objects);
  size_t i_tmp;
  size_t k_tmp;
  for(size_t i = 0; i < 3; ++i) {
    size_t num = 0;
    for(size_t k = 0; k < CUT - 1; ++k) {
      size_t index = i * CUT + k;
      if(Obs[index].size() == 0) continue;
      num += Obs[index].size();
      float c1 = BV_lower[index];
      float c2 = BV_upper[index];
      //std::cout << c1<<" "<<c2<<" "<<ca << std::endl;
      float C_cand = 2 * Costs::cost_t + Costs::cost_o * num * c1 / ca + (size - num) * c2 / ca;
      if(C_cand < C_min) {
        update = true;
        C_min = C_cand;
        i_tmp = i;
        k_tmp = k;
      }
    }
  }
  if(update) { //更新された
    for(size_t k = 0; k <= k_tmp; ++k) {
      split_min_1->insert(split_min_1->end(), Obs[k + i_tmp * CUT].begin(), Obs[k + i_tmp * CUT].end());
    }
    for(size_t k = k_tmp + 1; k < CUT; ++k) {
      split_min_2->insert(split_min_2->end(), Obs[k + i_tmp * CUT].begin(), Obs[k + i_tmp * CUT].end());
    }
  }
}

Tree* subdivision(Objects* objects) {
  ++Log::tree_depth;
  Log::whole_tree_depth = std::max(Log::tree_depth, Log::whole_tree_depth);
  std::cout << "Depth: " << Log::tree_depth << "/" << Log::whole_tree_depth
            << ", Processed: " << Log::done_data << "/" << Log::object_data
            << ", Processing: " << (int)objects->size()
            << "                                \r" << std::flush;
  float C_min = Costs::cost_o * objects->size();
  bool update = false;
  Objects split_min_1;
  Objects split_min_2;


  // //x
  // checkSplitAxis(objects, &split_min_1, &split_min_2, C_min, update, 0);
  // //y
  // checkSplitAxis(objects, &split_min_1, &split_min_2, C_min, update, 1);
  // //z
  // checkSplitAxis(objects, &split_min_1, &split_min_2, C_min, update, 2);
  checkSplit(objects, &split_min_1, &split_min_2, C_min, update);

  if(update) {
    Tree* t = new Tree(subdivision(&split_min_1), subdivision(&split_min_2));
    --Log::tree_depth;
    return t;
  } else {
    Log::done_data += objects->size();
    --Log::tree_depth;
    return new Tree(objects);
  }
}
