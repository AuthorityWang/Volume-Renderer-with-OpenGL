#include "dataloader.h"

RawLoader::RawLoader(std::string path, int x, int y, int z, int datatype) {
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

    FILE *fp;
    int size = x * y * z;
    fp = fopen(path.c_str(), "rb");
    if (datatype == 1) {
        GLubyte *rawdata = new GLubyte[size];
        fread(rawdata, sizeof(char), size, fp);
        fclose(fp);
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
    } else {
        GLshort *rawdata = new GLshort[size];
        fread(rawdata, sizeof(int), size, fp);
        fclose(fp);
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
}