#ifndef FUNCTIONLOADER_H_
#define FUNCTIONLOADER_H_

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "defines.h"
#include "utils.h"

class TfLoader {
public:
    GLuint tfTexture;
    TfLoader(std::string path);
};
#endif