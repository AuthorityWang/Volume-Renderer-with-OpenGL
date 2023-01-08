#include "dataloader.h"

RawLoader::RawLoader(std::string path, int x, int y, int z) {
    // get the file type
    std::string type = path.substr(path.length()-3, 3);
    std::string AbsPath;
    if (type != "raw" && type != "RAW") {
        std::cout<<"Invalid data type."<<std::endl;
        exit(-1);
    } else {
        AbsPath = getPath(path);
        std::cout<<"Detect raw file : read from " << AbsPath << std::endl;
    }
    // note that decide unint16_t or unint8_t to read file
    uint8_t temp;
    std::ifstream inFile(AbsPath, std::ifstream::in);
    if(!inFile) {
        std::cout << "load raw file error." << std::endl;
        exit(-1);
    }
    // store the function's parameter as texture
    GLubyte *rawdata = new GLubyte[x * y * z];
    inFile.read(reinterpret_cast<char*>(rawdata), 10000);
    GLuint RawTex;
    glGenTextures(1, &RawTex);
    glBindTexture(GL_TEXTURE_3D, RawTex);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_INTENSITY, x, y, z, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, rawdata);
    // in case of memory leak
    delete[] rawdata;
    rawTexture = RawTex;
}