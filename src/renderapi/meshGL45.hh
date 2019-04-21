#pragma once

#include <initializer_list>
#include <vector>
#include <cstdint>

struct Mesh //TODO support index buffer
{
	Mesh() = delete;
	
	inline ~Mesh()
	{
		glDeleteBuffers(1, &this->vboV);
		glDeleteBuffers(1, &this->vboU);
		glDeleteBuffers(1, &this->vboN);
		glDeleteBuffers(1, &this->vboI);
		glDeleteVertexArrays(1, &this->vao);
	}
	
	inline Mesh(std::vector<float> const &verts)
	{
		if(!verts.empty())
		{
			this->hasVerts = true;
			this->numVerts = verts.size();
		}
		glCreateVertexArrays(1, &this->vao);
		glCreateBuffers(1, &this->vboV);
		
		glNamedBufferData(this->vboV, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);
		glVertexArrayAttribBinding(this->vao, 0, 0);
		glVertexArrayVertexBuffer(this->vao, 0, this->vboV, 0, this->vertexStride);
		glEnableVertexArrayAttrib(this->vao, 0);
		glVertexArrayAttribFormat(this->vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
	}
	
	inline Mesh(std::vector<float> const &verts, std::vector<float> const &uvs)
	{
		if(!verts.empty())
		{
			this->hasVerts = true;
			this->numVerts = verts.size();
		}
		if(!uvs.empty()) this->hasUVs = true;
		glCreateVertexArrays(1, &this->vao);
		glCreateBuffers(1, &this->vboV);
		glCreateBuffers(1, &this->vboU);
		
		glNamedBufferData(this->vboV, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);
		glVertexArrayAttribBinding(this->vao, 0, 0);
		glVertexArrayVertexBuffer(this->vao, 0, this->vboV, 0, this->vertexStride);
		glEnableVertexArrayAttrib(this->vao, 0);
		glVertexArrayAttribFormat(this->vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
		
		glNamedBufferData(this->vboU, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW);
		glVertexArrayAttribBinding(this->vao, 1, 1);
		glVertexArrayVertexBuffer(this->vao, 1, this->vboU, 0, this->uvStride);
		glEnableVertexArrayAttrib(this->vao, 1);
		glVertexArrayAttribFormat(this->vao, 1, 2, GL_FLOAT, GL_FALSE, 0);
	}
	
	inline Mesh(std::vector<float> const &verts, std::vector<float> const &uvs, std::vector<float> const &normals)
	{
		if(!verts.empty())
		{
			this->hasVerts = true;
			this->numVerts = verts.size();
		}
		if(!uvs.empty()) this->hasUVs = true;
		if(!normals.empty()) this->hasNormals = true;
		glCreateVertexArrays(1, &this->vao);
		glCreateBuffers(1, &this->vboV);
		glCreateBuffers(1, &this->vboU);
		glCreateBuffers(1, &this->vboN);
		
		glNamedBufferData(this->vboV, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);
		glVertexArrayAttribBinding(this->vao, 0, 0);
		glVertexArrayVertexBuffer(this->vao, 0, this->vboV, 0, this->vertexStride);
		glEnableVertexArrayAttrib(this->vao, 0);
		glVertexArrayAttribFormat(this->vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
		
		glNamedBufferData(this->vboU, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW);
		glVertexArrayAttribBinding(this->vao, 1, 1);
		glVertexArrayVertexBuffer(this->vao, 1, this->vboU, 0, this->uvStride);
		glEnableVertexArrayAttrib(this->vao, 1);
		glVertexArrayAttribFormat(this->vao, 1, 2, GL_FLOAT, GL_FALSE, 0);
		
		glNamedBufferData(this->vboN, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);
		glVertexArrayAttribBinding(this->vao, 2, 2);
		glVertexArrayVertexBuffer(this->vao, 2, this->vboN, 0, this->normalStride);
		glEnableVertexArrayAttrib(this->vao, 2);
		glVertexArrayAttribFormat(this->vao, 2, 3, GL_FLOAT, GL_FALSE, 0);
	}
	
	//TODO float* size_t constructors
	//TODO initializer list constructors
	
	//copy
	inline Mesh(Mesh &other)
	{
		this->vao = other.vao;
		other.vao = 0;
		
		this->vboV = other.vboV;
		other.vboV = 0;
		
		this->vboU = other.vboU;
		other.vboU = 0;
		
		this->vboN = other.vboN;
		other.vboN = 0;
		
		this->vboI = other.vboI;
		other.vboI = 0;
		
		this->numVerts = other.numVerts;
		other.numVerts = 0;
		
		this->hasVerts = other.hasVerts;
		other.hasVerts = false;
		
		this->hasUVs = other.hasUVs;
		other.hasUVs = false;
		
		this->hasNormals = other.hasNormals;
		other.hasNormals = false;
	}
	
	inline Mesh& operator=(Mesh other)
	{
		this->vao = other.vao;
		other.vao = 0;
		
		this->vboV = other.vboV;
		other.vboV = 0;
		
		this->vboU = other.vboU;
		other.vboU = 0;
		
		this->vboN = other.vboN;
		other.vboN = 0;
		
		this->vboI = other.vboI;
		other.vboI = 0;
		
		this->numVerts = other.numVerts;
		other.numVerts = 0;
		
		this->hasVerts = other.hasVerts;
		other.hasVerts = false;
		
		this->hasUVs = other.hasUVs;
		other.hasUVs = false;
		
		this->hasNormals = other.hasNormals;
		other.hasNormals = false;
		return *this;
	}
	
	//move
	Mesh(Mesh &&other) noexcept
	{
		this->vao = other.vao;
		other.vao = 0;
		
		this->vboV = other.vboV;
		other.vboV = 0;
		
		this->vboU = other.vboU;
		other.vboU = 0;
		
		this->vboN = other.vboN;
		other.vboN = 0;
		
		this->vboI = other.vboI;
		other.vboI = 0;
		
		this->numVerts = other.numVerts;
		other.numVerts = 0;
		
		this->hasVerts = other.hasVerts;
		other.hasVerts = false;
		
		this->hasUVs = other.hasUVs;
		other.hasUVs = false;
		
		this->hasNormals = other.hasNormals;
		other.hasNormals = false;
	}
	
	Mesh& operator=(Mesh &&other) noexcept
	{
		this->vao = other.vao;
		other.vao = 0;
		
		this->vboV = other.vboV;
		other.vboV = 0;
		
		this->vboU = other.vboU;
		other.vboU = 0;
		
		this->vboN = other.vboN;
		other.vboN = 0;
		
		this->vboI = other.vboI;
		other.vboI = 0;
		
		this->numVerts = other.numVerts;
		other.numVerts = 0;
		
		this->hasVerts = other.hasVerts;
		other.hasVerts = false;
		
		this->hasUVs = other.hasUVs;
		other.hasUVs = false;
		
		this->hasNormals = other.hasNormals;
		other.hasNormals = false;
		return *this;
	}
	
	inline void use()
	{
		glBindVertexArray(this->vao);
	}
	
	uint32_t vao = 0, vboV = 0, vboU = 0, vboN = 0, vboI = 0;
	size_t numVerts = 0;
	bool hasVerts = false, hasUVs = false, hasNormals = false;

private:
	int32_t vertexStride = 3 * sizeof(float);
	int32_t uvStride = 2 * sizeof(float);
	int32_t normalStride = 3 * sizeof(float);
};
