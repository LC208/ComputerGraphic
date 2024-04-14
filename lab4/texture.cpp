#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb-master/stb_image.h"
#include <GL/gl.h>


void Texture::bindTexture()
{
    int sW;
    int sH;
    int tcnt;
    unsigned char *data=stbi_load(const_cast<char*>(filename.data()),&sW,&sH,&tcnt,0);
    this->width = sW;
    this->height = sH;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sW, sH,
    0, tcnt == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
}
