#include "Texture.hpp"

namespace my
{


	Texture::Texture()
		: mIsValid(false)
	{
	}

	Texture::~Texture()
	{
		this->free();
	}


	void Texture::load(std::string const & texturePath, bool verticalFlip, GLint format, GLint wrap)
	{
		this->free();

		stbi_set_flip_vertically_on_load(verticalFlip);
		GLuint id;
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, numChannels;
		unsigned char *data = stbi_load(texturePath.c_str(), &width, &height, &numChannels, 0);
		mProperties = Properties(width, height, numChannels);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			mIsValid = true;
			mID = id;
		}
		else
		{
			std::cout << "Failed to load texture!" << std::endl;
			mIsValid = false;
		}

		stbi_image_free(data);

	}

	void Texture::free()
	{
		if (mIsValid)
		{
			glDeleteTextures(1, &mID);
		}
		mIsValid = false;
	}

	GLuint Texture::getID() const
	{
		return mID;
	}
	bool Texture::getIsValid() const
	{
		return mIsValid;
	}
	Texture::Properties Texture::getProperties() const
	{
		return mProperties;
	}


}
