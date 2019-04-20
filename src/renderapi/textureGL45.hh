#pragma once

#include "enums.hh"

#include <cstdint>

struct TextureGL45
{
	TextureGL45() = delete;
	
	inline TextureGL45(uint8_t **data, uint32_t width, uint32_t height, ColorFormat colorFormat, bool sRGB = false)
	{
		this->width = width;
		this->height = height;
		glCreateTextures(GL_TEXTURE_2D, 1, &this->handle);
		glTextureStorage2D(this->handle, 1, sRGB ? (colorFormat == ColorFormat::RGB ? GL_SRGB8 : GL_SRGB8_ALPHA8) : (colorFormat == ColorFormat::RGB ? GL_RGB8 : GL_RGBA8), width, height);
		for(uint32_t i = 0; i < this->height; i++) glTextureSubImage2D(this->handle, 0, 0, i, this->width, 1, colorFormat == ColorFormat::RGB ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data[i]);
		this->setInterpolation(InterpolationMode::Linear, InterpolationMode::Linear);
		this->setAnisotropyLevel(1);
	}
	
	inline explicit TextureGL45(uint8_t red = 255, uint8_t green = 255, uint8_t blue = 255, uint8_t alpha = 255, bool sRGB = false)
	{
		this->width = 1;
		this->height = 1;
		uint8_t **data = new uint8_t *;
		data[0] = new uint8_t[4];
		data[0][0] = red;
		data[0][1] = green;
		data[0][2] = blue;
		data[0][3] = alpha;
		glCreateTextures(GL_TEXTURE_2D, 1, &this->handle);
		glTextureStorage2D(this->handle, 1, sRGB ? GL_SRGB8_ALPHA8 : GL_RGBA8, this->width, this->height);
		glTextureSubImage2D(this->handle, 0, 0, 0, this->width, 1, GL_RGBA, GL_UNSIGNED_BYTE, data[0]);
		this->setInterpolation(InterpolationMode::Linear, InterpolationMode::Linear);
		this->setAnisotropyLevel(1);
	}
	
	inline ~TextureGL45()
	{
		glDeleteTextures(1, &this->handle);
	}
	
	//copy
	inline TextureGL45(TextureGL45 &other)
	{
		this->handle = other.handle;
		other.handle = 0;
	}
	
	inline TextureGL45& operator=(TextureGL45 other)
	{
		this->handle = other.handle;
		other.handle = 0;
		return *this;
	}
	
	//move
	inline TextureGL45(TextureGL45 &&other) noexcept
	{
		this->handle = other.handle;
		other.handle = 0;
	}
	
	inline TextureGL45& operator=(TextureGL45 &&other) noexcept
	{
		this->handle = other.handle;
		other.handle = 0;
		return *this;
	}
	
	inline void use(uint32_t target)
	{
		glBindTextureUnit(target, this->handle);
	}
	
	inline void setInterpolation(InterpolationMode min, InterpolationMode mag)
	{
		GLenum glMin = GL_NEAREST, glMag = GL_NEAREST;
		switch(min)
		{
			case InterpolationMode::Nearest: glMin = GL_NEAREST; break;
			case InterpolationMode::Linear: glMin = GL_LINEAR; break;
			default: break;
		}
		
		switch(mag)
		{
			case InterpolationMode::Nearest: glMag = GL_NEAREST; break;
			case InterpolationMode::Linear: glMag = GL_LINEAR; break;
			default: break;
		}
		glTextureParameteri(this->handle, GL_TEXTURE_MIN_FILTER, glMin);
		glTextureParameteri(this->handle, GL_TEXTURE_MAG_FILTER, glMag);
	}
	
	inline void setAnisotropyLevel(uint32_t level)
	{
		glTextureParameterf(this->handle, GL_TEXTURE_MAX_ANISOTROPY, level);
	}
	
	uint32_t handle = 0, width = 0, height = 0;
};
