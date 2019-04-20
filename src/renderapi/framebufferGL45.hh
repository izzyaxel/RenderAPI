#pragma once

#include "enums.hh"

#include <cstdint>
#include <initializer_list>
#include <vector>
#include <cstdio>

struct FramebufferGL45
{
	FramebufferGL45() = delete;
	
	inline FramebufferGL45(uint32_t width, uint32_t height, std::initializer_list<AttachmentType> const &options, std::string const &name)
	{
		this->width = width;
		this->height = height;
		for(auto const &option : options)
		{
			switch(option)
			{
				case AttachmentType::Color: this->hasColor = true; break;
				case AttachmentType::Alpha: this->hasAlpha = true; break;
				case AttachmentType::Depth: this->hasDepth = true; break;
				case AttachmentType::Stencil: this->hasStencil = true; break;
				default: break;
			}
		}
		this->name = name;
		createFBO(*this);
	}
	
	inline ~FramebufferGL45()
	{
		clearFBO(*this);
	}
	
	//copy
	inline FramebufferGL45(FramebufferGL45 &other)
	{
		this->handle = other.handle;
		other.handle = 0;
		
		this->colorHandle = other.colorHandle;
		other.colorHandle = 0;
		
		this->depthHandle = other.depthHandle;
		other.depthHandle = 0;
		
		this->stencilHandle = other.stencilHandle;
		other.stencilHandle = 0;
	}
	
	inline FramebufferGL45& operator=(FramebufferGL45 other)
	{
		this->handle = other.handle;
		other.handle = 0;
		
		this->colorHandle = other.colorHandle;
		other.colorHandle = 0;
		
		this->depthHandle = other.depthHandle;
		other.depthHandle = 0;
		
		this->stencilHandle = other.stencilHandle;
		other.stencilHandle = 0;
		return *this;
	}
	
	//move
	FramebufferGL45(FramebufferGL45 &&other) noexcept
	{
		this->handle = other.handle;
		other.handle = 0;
		
		this->colorHandle = other.colorHandle;
		other.colorHandle = 0;
		
		this->depthHandle = other.depthHandle;
		other.depthHandle = 0;
		
		this->stencilHandle = other.stencilHandle;
		other.stencilHandle = 0;
	}
	
	FramebufferGL45& operator=(FramebufferGL45 &&other) noexcept
	{
		this->handle = other.handle;
		other.handle = 0;
		
		this->colorHandle = other.colorHandle;
		other.colorHandle = 0;
		
		this->depthHandle = other.depthHandle;
		other.depthHandle = 0;
		
		this->stencilHandle = other.stencilHandle;
		other.stencilHandle = 0;
		return *this;
	}
	
	inline void use()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, this->handle);
	}
	
	inline void bind(AttachmentType type, uint32_t target)
	{
		switch(type)
		{
			case AttachmentType::Color: glBindTextureUnit(target, this->colorHandle); break;
			case AttachmentType::Depth: glBindTextureUnit(target, this->depthHandle); break;
			case AttachmentType::Stencil: glBindTextureUnit(target, this->stencilHandle); break;
			default: break;
		}
	}
	
	inline void regenerate(uint32_t width, uint32_t height)
	{
		this->width = width;
		this->height = height;
		clearFBO(*this);
		createFBO(*this);
	}
	
	uint32_t handle = 0, colorHandle = 0, depthHandle = 0, stencilHandle = 0, width = 0, height = 0;
	bool hasColor = false, hasDepth = false, hasAlpha = false, hasStencil = false;
	std::string name = "";

private:
	inline void createFBO(FramebufferGL45 &fbo)
	{
		glCreateFramebuffers(1, &fbo.handle);
		fbo.use();
		glViewport(0, 0, fbo.width, fbo.height);
		glScissor(0, 0, fbo.width, fbo.height);
		if(fbo.hasColor) glCreateTextures(GL_TEXTURE_2D, 1, &fbo.colorHandle);
		if(fbo.hasDepth) glCreateTextures(GL_TEXTURE_2D, 1, &fbo.depthHandle);
		glTextureStorage2D(fbo.colorHandle, 1, fbo.hasAlpha ? GL_RGBA32F : GL_RGB32F, fbo.width, fbo.height);
		glNamedFramebufferTexture(fbo.handle, GL_COLOR_ATTACHMENT0, fbo.colorHandle, 0);
		if(fbo.hasDepth)
		{
			glTextureStorage2D(fbo.depthHandle, 1, GL_DEPTH_COMPONENT32F, fbo.width, fbo.height);
			glNamedFramebufferTexture(fbo.handle, GL_DEPTH_ATTACHMENT, fbo.depthHandle, 0);
		}
		std::vector<GLenum> drawBuffers;
		drawBuffers.emplace_back(GL_COLOR_ATTACHMENT0);
		if(fbo.hasDepth) drawBuffers.emplace_back(GL_COLOR_ATTACHMENT1);
		glNamedFramebufferDrawBuffers(fbo.handle, static_cast<int32_t>(drawBuffers.size()), drawBuffers.data());
		GLenum error = glCheckNamedFramebufferStatus(fbo.handle, GL_FRAMEBUFFER);
		if(error != GL_FRAMEBUFFER_COMPLETE)
		{
			printf("FBO Creation Error: ");
			switch(error)
			{
				case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: printf("incomplete attachment\n"); break;
				case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT: printf("incomplete dimensions\n"); break;
				case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: printf("missing attachment\n"); break;
				case GL_FRAMEBUFFER_UNSUPPORTED: printf("FBOs are not supported\n"); break;
				default: break;
			}
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	inline void clearFBO(FramebufferGL45 &fbo)
	{
		glDeleteFramebuffers(1, &fbo.handle);
		glDeleteTextures(1, &fbo.colorHandle);
		glDeleteTextures(1, &fbo.depthHandle);
	}
};
