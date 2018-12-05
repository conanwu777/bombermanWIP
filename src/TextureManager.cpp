//
// Created by hjeannin on 28/07/17.
//

#include "../inc/TextureManager.hpp"

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
}

void
TextureManager::generateDefaultTexture(GLuint textureID)
{
    GLuint      iter_width, iter_height = 0;
    GLubyte     custom_texture[16][16][4];

    for (iter_width = 0; iter_width < 16; iter_width++)
    {
        for (iter_height = 0; iter_height < 16; iter_height++)
        {
            custom_texture[iter_width][iter_height][0] = (GLubyte) 255;
            custom_texture[iter_width][iter_height][1] = (GLubyte) 255;
            custom_texture[iter_width][iter_height][2] = (GLubyte) 255;
            custom_texture[iter_width][iter_height][3] = (GLubyte) 125;
        }
    }

    for (iter_height = 0; iter_height < 16; iter_height++)
    {
        custom_texture[0][iter_height][0] = (GLubyte) 255;
        custom_texture[0][iter_height][1] = (GLubyte) 0;
        custom_texture[0][iter_height][2] = (GLubyte) 0;
        custom_texture[0][iter_height][3] = (GLubyte) 125;
        custom_texture[1][iter_height][0] = (GLubyte) 0;
        custom_texture[1][iter_height][1] = (GLubyte) 255;
        custom_texture[1][iter_height][2] = (GLubyte) 0;
        custom_texture[1][iter_height][3] = (GLubyte) 125;
        custom_texture[2][iter_height][0] = (GLubyte) 0;
        custom_texture[2][iter_height][1] = (GLubyte) 0;
        custom_texture[2][iter_height][2] = (GLubyte) 255;
        custom_texture[2][iter_height][3] = (GLubyte) 125;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 16, 16, 0, GL_RGBA, GL_UNSIGNED_BYTE, custom_texture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

void TextureManager::loadImageFile(GLuint textureID, const char *file, int wanted_bpp)
{
    int             width, height, bpp;
    unsigned char*  image = stbi_load(file, &width, &height, &bpp, wanted_bpp);

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)width, (GLsizei)height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    stbi_image_free(image);
}

void TextureManager::parametrizeFontTexture(GLuint textureID)
{
}

void TextureManager::loadCubemap(GLuint textureID, std::vector<const GLchar*> faces, int wanted_bpp)
{
    int                 width, height, bpp;
    unsigned char*		image;

    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    for(GLuint i = 0; i < faces.size(); i++)
	{
        image = stbi_load(faces[i], &width, &height, &bpp, wanted_bpp);
        if (image == nullptr)
		{
            std::cerr << " Error loading Cube texture" << std::endl;
        }
        glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
			GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
			);
        stbi_image_free(image);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void TextureManager::loadAtlas(GLuint textureID, char const *filename, int count, int wanted_bpp)
{
    int                 width, height, bpp;
    unsigned char*      image;

    image = stbi_load(filename, &width, &height, &bpp, wanted_bpp);

    if (image == nullptr)
    {
        std::cerr << " Error loading atlas" << std::endl;
    }
    glBindTexture(GL_TEXTURE_2D_ARRAY, textureID);
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8,
				256, 256, count,
				0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr); // allocate the texture array
	glPixelStorei(GL_UNPACK_ROW_LENGTH, width); // set the total atlas size in order to cut through it with glTexSubImage3D
	for (int x = 0; x < count; ++x)
	{
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0,
						0, 0, x, // z (using x tex coord) is the `index` of the array
						256, 256, 1, // generate 1 subtexture of 256x256
						GL_RGBA, GL_UNSIGNED_BYTE, image + (x * 256) * 4); // get the pointer on the texture data using very advanced arithmetics. (index * width * RGBA)
	}
	stbi_image_free(image);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}