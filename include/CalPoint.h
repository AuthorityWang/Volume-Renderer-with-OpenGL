#ifndef CALPOINT_H_
#define CALPOINT_H_

#include "defines.h"
#include "utils.h"

class PointGenerate {
public:
    unsigned int PointTexture;
    unsigned int Buffer;
    PointGenerate(int width, int height);
};
#endif