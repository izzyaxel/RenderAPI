#pragma once

#include <initializer_list>
#include <vector>
#include <cstdint>
#include <array>

//TODO support index buffer
struct Mesh
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
	
	//Base constructors, all others should be able to delegate to these
	inline explicit Mesh(float const *verts, size_t vertsSize)
	{
		if(vertsSize != 0)
		{
			this->hasVerts = true;
			this->numVerts = vertsSize / 3;
		}
		glCreateVertexArrays(1, &this->vao);
		glCreateBuffers(1, &this->vboV);
		
		glNamedBufferData(this->vboV, vertsSize * sizeof(float), verts, GL_STATIC_DRAW);
		glVertexArrayAttribBinding(this->vao, 0, 0);
		glVertexArrayVertexBuffer(this->vao, 0, this->vboV, 0, this->vertexStride);
		glEnableVertexArrayAttrib(this->vao, 0);
		glVertexArrayAttribFormat(this->vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
	}
	
	inline explicit Mesh(float const *verts, size_t vertsSize, float const *uvs, size_t uvsSize)
	{
		if(vertsSize != 0)
		{
			this->hasVerts = true;
			this->numVerts = vertsSize / 3;
		}
		if(uvsSize != 0) this->hasUVs = true;
		glCreateVertexArrays(1, &this->vao);
		glCreateBuffers(1, &this->vboV);
		glCreateBuffers(1, &this->vboU);
		
		glNamedBufferData(this->vboV, vertsSize * sizeof(float), verts, GL_STATIC_DRAW);
		glVertexArrayAttribBinding(this->vao, 0, 0);
		glVertexArrayVertexBuffer(this->vao, 0, this->vboV, 0, this->vertexStride);
		glEnableVertexArrayAttrib(this->vao, 0);
		glVertexArrayAttribFormat(this->vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
		
		glNamedBufferData(this->vboU, uvsSize * sizeof(float), uvs, GL_STATIC_DRAW);
		glVertexArrayAttribBinding(this->vao, 1, 1);
		glVertexArrayVertexBuffer(this->vao, 1, this->vboU, 0, this->uvStride);
		glEnableVertexArrayAttrib(this->vao, 1);
		glVertexArrayAttribFormat(this->vao, 1, 2, GL_FLOAT, GL_FALSE, 0);
	}
	
	inline explicit Mesh(float const *verts, size_t vertsSize, float const *uvs, size_t uvsSize, float const *normals, size_t normalsSize)
	{
		if(vertsSize != 0)
		{
			this->hasVerts = true;
			this->numVerts = vertsSize / 3;
		}
		if(uvsSize != 0) this->hasUVs = true;
		if(normalsSize != 0) this->hasNormals = true;
		glCreateVertexArrays(1, &this->vao);
		glCreateBuffers(1, &this->vboV);
		glCreateBuffers(1, &this->vboU);
		glCreateBuffers(1, &this->vboN);
		
		glNamedBufferData(this->vboV, vertsSize * sizeof(float), verts, GL_STATIC_DRAW);
		glVertexArrayAttribBinding(this->vao, 0, 0);
		glVertexArrayVertexBuffer(this->vao, 0, this->vboV, 0, this->vertexStride);
		glEnableVertexArrayAttrib(this->vao, 0);
		glVertexArrayAttribFormat(this->vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
		
		glNamedBufferData(this->vboU, uvsSize * sizeof(float), uvs, GL_STATIC_DRAW);
		glVertexArrayAttribBinding(this->vao, 1, 1);
		glVertexArrayVertexBuffer(this->vao, 1, this->vboU, 0, this->uvStride);
		glEnableVertexArrayAttrib(this->vao, 1);
		glVertexArrayAttribFormat(this->vao, 1, 2, GL_FLOAT, GL_FALSE, 0);
		
		glNamedBufferData(this->vboN, normalsSize * sizeof(float), normals, GL_STATIC_DRAW);
		glVertexArrayAttribBinding(this->vao, 2, 2);
		glVertexArrayVertexBuffer(this->vao, 2, this->vboN, 0, this->normalStride);
		glEnableVertexArrayAttrib(this->vao, 2);
		glVertexArrayAttribFormat(this->vao, 2, 3, GL_FLOAT, GL_FALSE, 0);
	}
	
	//float
	inline explicit Mesh(std::vector<float> const &verts) : Mesh(verts.data(), verts.size()) {}
	inline Mesh(std::vector<float> const &verts, std::vector<float> const &uvs) : Mesh(verts.data(), verts.size(), uvs.data(), uvs.size()) {}
	inline Mesh(std::vector<float> const &verts, std::vector<float> const &uvs, std::vector<float> const &normals) : Mesh(verts.data(), verts.size(), uvs.data(), uvs.size(), normals.data(), normals.size()) {}
	
	inline Mesh(std::initializer_list<float> const &verts) : Mesh(verts.begin(), verts.size()) {}
	inline Mesh(std::initializer_list<float> const &verts, std::initializer_list<float> const &uvs) : Mesh(verts.begin(), verts.size(), uvs.begin(), uvs.size()) {}
	inline Mesh(std::initializer_list<float> const &verts, std::initializer_list<float> const &uvs, std::initializer_list<float> const &normals) : Mesh(verts.begin(), verts.size(), uvs.begin(), uvs.size(), normals.begin(), normals.size()) {}
	
	template <size_t N> inline explicit Mesh(std::array<float, N> const &verts) : Mesh(verts.data(), verts.size()) {}
	template <size_t N> inline Mesh(std::array<float, N> const &verts, std::array<float, N> const &uvs) : Mesh(verts.data(), verts.size(), uvs.data(), uvs.size()) {}
	template <size_t N> inline Mesh(std::array<float, N> const &verts, std::array<float, N> const &uvs, std::array<float, N> const &normals) : Mesh(verts.data(), verts.size(), uvs.data(), uvs.size(), normals.data(), normals.size()) {}
	
	//uint8, raw data read out of files
	inline explicit Mesh(std::vector<uint8_t> const &verts) : Mesh(reinterpret_cast<float const*>(verts.data()), verts.size() / 4) {}
	inline Mesh(std::vector<uint8_t> const &verts, std::vector<uint8_t> const &uvs) : Mesh(reinterpret_cast<float const*>(verts.data()), verts.size() / 4, reinterpret_cast<float const*>(uvs.data()), uvs.size() / 4) {}
	inline Mesh(std::vector<uint8_t> const &verts, std::vector<uint8_t> const &uvs, std::vector<uint8_t> const &normals) : Mesh(reinterpret_cast<float const*>(verts.data()), verts.size() / 4, reinterpret_cast<float const*>(uvs.data()), uvs.size() / 4, reinterpret_cast<float const*>(normals.data()), normals.size() / 4) {}
	
	inline Mesh(std::initializer_list<uint8_t> const &verts) : Mesh(reinterpret_cast<float const*>(verts.begin()), verts.size() / 4) {}
	inline Mesh(std::initializer_list<uint8_t> const &verts, std::initializer_list<uint8_t> const &uvs) : Mesh(reinterpret_cast<float const*>(verts.begin()), verts.size() / 4, reinterpret_cast<float const*>(uvs.begin()), uvs.size() / 4) {}
	inline Mesh(std::initializer_list<uint8_t> const &verts, std::initializer_list<uint8_t> const &uvs, std::initializer_list<uint8_t> const &normals) : Mesh(reinterpret_cast<float const*>(verts.begin()), verts.size() / 4, reinterpret_cast<float const*>(uvs.begin()), uvs.size() / 4, reinterpret_cast<float const*>(normals.begin()), normals.size() / 4) {}
	
	template <size_t N> inline explicit Mesh(std::array<uint8_t, N> const &verts) : Mesh(reinterpret_cast<float const*>(verts.data()), verts.size() / 4) {}
	template <size_t N>  inline Mesh(std::array<uint8_t, N> const &verts, std::array<uint8_t, N> const &uvs) : Mesh(reinterpret_cast<float const*>(verts.data()), verts.size() / 4, reinterpret_cast<float const*>(uvs.data()), uvs.size() / 4) {}
	template <size_t N>  inline Mesh(std::array<uint8_t, N> const &verts, std::array<uint8_t, N> const &uvs, std::array<uint8_t, N> const &normals) : Mesh(reinterpret_cast<float const*>(verts.data()), verts.size() / 4, reinterpret_cast<float const*>(uvs.data()), uvs.size() / 4, reinterpret_cast<float const*>(normals.data()), normals.size() / 4) {}
	
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
