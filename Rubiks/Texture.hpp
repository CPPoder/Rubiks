#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <glad\glad.h>
#include "stb_image.h"

#include <string>
#include <iostream>


namespace my
{

	class Texture
	{
	public:
		struct Properties
		{
		public:
			int width;
			int height;
			int numChannels;

		public:
			Properties() = default;
			Properties(int _width, int _height, int _numChannels) : width(_width), height(_height), numChannels(_numChannels) {}
			Properties(Properties const &) = default;
			Properties& operator=(Properties const &) = default;
			~Properties() = default;
		};

	private:
		GLuint mID;
		bool mIsValid = false;
		Properties mProperties;

	public:
		Texture();
		Texture(Texture const &) = delete;
		Texture& operator=(Texture const &) = delete;
		~Texture();

	public:
		void load(std::string const & texturePath, bool verticalFlip = false, GLint format = GL_RGB, GLint wrap = GL_REPEAT);
		void free();

		GLuint getID() const;
		bool getIsValid() const;
		Properties getProperties() const;


	};


}




#endif //TEXTURE_HPP
