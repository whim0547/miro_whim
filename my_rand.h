#pragma once

#include <stdlib.h>
#include <random>

inline
float uniform_0_1() {
  return (rand() + 0.5f) / (RAND_MAX * 1.0f + 1.0f);
}
