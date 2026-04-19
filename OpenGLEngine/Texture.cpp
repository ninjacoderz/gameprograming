//
// Created by Binh Nguyen Thanh on 19/4/26.
//

#include <SOIL/SOIL.h>
#include <GL/glew.h>

#include "Texture.h"

Texture::Texture()
:mTextureID(0)
,mWidth(0)
,mHeight(0){
}

Texture::~Texture()
{

}

bool Texture::Load(const char *fileName) {
    int channels = 0;
    unsigned char* image = SOIL_load_image(
        fileName,
        &mWidth,
        &mHeight,
        &channels,
        SOIL_LOAD_AUTO);
    int format = GL_RGB;
    if (channels == 4) {
        format = GL_RGBA;
    }

    glGenTextures(1, &mTextureID);
    glBindTexture(GL_TEXTURE_2D, mTextureID);
}
