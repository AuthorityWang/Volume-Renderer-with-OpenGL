#include "CalPoint.h"

PointGenerate::PointGenerate(int width, int height) {
    // use shader to get the ray position
    unsigned int PointTex;
    glGenTextures(1, &PointTex);
    glBindTexture(GL_TEXTURE_2D, PointTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    unsigned int DBuffer;
    glGenRenderbuffers(1, &DBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, DBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    // need depth to get the ray
    unsigned int FBuffer;
    glGenFramebuffers(1, &FBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, FBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, PointTex, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, DBuffer);
    // glBindFramebuffer(GL_FRAMEBUFFER, 0);
    Buffer = DBuffer;
    PointTexture = PointTex;
};