#include "functionloader.h"

TfLoader::TfLoader(std::string path) {
    // get the file type
    std::string type = path.substr(path.length()-3, 3);
    std::string AbsPath;
    if (type != "dat" && type != "DAT") {
        std::cout<<"Invalid data type."<<std::endl;
        exit(-1);
    } else {
        AbsPath = getPath(path);
        std::cout<<"Detect dat file : read from " << AbsPath << std::endl;
    }
    // note that decide unint16_t or unint8_t to read file
    uint8_t temp;
    std::ifstream inFile(AbsPath, std::ifstream::in);
    if(!inFile) {
        std::cout << "load dat file error." << std::endl;
        exit(-1);
    }
    
    GLubyte *Tf = (GLubyte *) calloc(10000, sizeof(GLubyte));
    inFile.read(reinterpret_cast<char*>(Tf), 10000);
    GLuint TfTex;
    glGenTextures(1, &TfTex);
    glBindTexture(GL_TEXTURE_1D, TfTex);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA8, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, Tf);
    free(Tf);
    tfTexture = TfTex;
}