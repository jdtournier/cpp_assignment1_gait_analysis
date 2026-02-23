#include <vector>

#include "utils.h"

double compute_mean (const std::vector<double>& v)
{
  double sum = 0.0;
  for (auto x : v)
    sum += x;
  return sum / v.size();
}
