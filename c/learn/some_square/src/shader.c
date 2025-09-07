#include "shader.h"

static inline char ascii_from_brightness(float b, const char* ramp) {
    if (b < 0) b = 0; 
    if (b > 1) b = 1;

    size_t n = 0;
    while (ramp[n]) ++n;
    size_t i = (size_t)(b * (n - 1) + 0.5f);
    
    return ramp[i];
}