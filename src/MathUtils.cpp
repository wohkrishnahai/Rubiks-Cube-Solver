#include "../include/MathUtils.h"

// Factorial using standard iteration. 
// For n <= 12, an iterative approach is so fast, it often beats memory latency.
uint32_t factorial(uint32_t n) {
  if (n <= 1) return 1;

  uint32_t val = 1;
  for (uint32_t i = 2; i <= n; ++i) {
    val *= i;
  }

  return val;
}

// Calculates nCk = n! / (k! * (n-k)!)
uint32_t pick(uint32_t n, uint32_t k) {
  if (k > n) return 0;
  if (k == 0 || k == n) return 1;

  // Optimize calculation by ensuring we divide by the smaller number
  if (k > n - k) k = n - k;

  uint32_t ans = 1;
  for (uint32_t i = 0; i < k; ++i) {
    ans *= (n - i);
    ans /= (i + 1);
  }

  return ans;
}
