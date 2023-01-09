#ifndef DATALOADER_H_
#define DATALOADER_H_

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "defines.h"
#include "utils.h"

class RawLoader {
public:
    GLuint rawTexture;
    RawLoader(std::string path, int x = 256, int y = 256, int z = 256, int datatype = 1);
};
#endif