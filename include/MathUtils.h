#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <cstdint>

// We use uint32_t because 12! fits in 32 bits, and we only ever need up to 
// 12! for edges, and 8! for corners.

// Calculates n!
uint32_t factorial(uint32_t n);

// Calculates nCk
uint32_t pick(uint32_t n, uint32_t k);

#endif // MATH_UTILS_H
