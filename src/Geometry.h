#pragma once

#include "d3dStd.h"
#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <list>
#include "MathHelper.h"

class MatrixStack
{
public:
	MatrixStack();
	~MatrixStack();

	void push(const XMMATRIX&);
	void pushAndSet(const XMMATRIX&);
	void pop();
	XMMATRIX* getTop();
	void multMatrixLocal(const XMMATRIX&);

private:
	std::list<XMMATRIX> mMatrixStack;
};

class Vector3 : public XMFLOAT3
{
public:
	inline float length() const { XMVector3Length(XMLoadFloat3(this)); }
	inline void normalize() { XMStoreFloat3(this, XMVector3Normalize(XMLoadFloat3(this))); }
	inline float dot(const Vector3 &b) const {	return XMVectorGetX(XMVector3Dot(XMLoadFloat3(this), XMLoadFloat3(this))); }
	inline Vector3 cross(const Vector3&b) const;

	Vector3() { x = 0.0f, y = 0.0f, z = 0.0f; }
	Vector3(XMFLOAT3 &vector) : XMFLOAT3(vector) { }
	Vector3(const XMFLOAT3 &vector) : XMFLOAT3(vector) { }
	Vector3(XMVECTOR &vector) : XMFLOAT3(XMVectorGetX(vector), XMVectorGetY(vector), XMVectorGetZ(vector)) { }
	Vector3(const float _x, const float _y, const float _z) { x=_x; y=_y; z=_z; }
	Vector3(const double _x, const double _y, const double _z) { x = (float)_x; y = (float)_y; z = (float)_z; }
};

inline Vector3 Vector3::cross(const Vector3 &b) const
{
	Vector3 out;
	XMStoreFloat3(&out, XMVector3Cross(XMLoadFloat3(this), XMLoadFloat3(this)));
	return out;
}

struct Vertex
{
	Vertex():Color(1.0f, 1.0f, 1.0f, 1.0f){}
	Vertex(const XMFLOAT3& p, const XMFLOAT3& n, const XMFLOAT3& t, XMFLOAT2& uv)
		: Position(p), Normal(n), TangentU(t), TexCoord(uv), Color(1.0f, 1.0f, 1.0f, 1.0f) {}
	Vertex(
		float px, float py, float pz, 
		float nx, float ny, float nz,
		float tx, float ty, float tz,
		float u, float v)
		: Position(px,py,pz), Normal(nx,ny,nz),
			TangentU(tx, ty, tz), TexCoord(u,v), Color(1.0f, 1.0f, 1.0f, 1.0f){}

	XMFLOAT3 Position;
	XMFLOAT4 Color;
	XMFLOAT3 Normal;
	XMFLOAT3 TangentU;
	XMFLOAT2 TexCoord;
};

struct SimpleVertex
{
	SimpleVertex():Color(1.0f, 1.0f, 1.0f, 1.0f){}
	SimpleVertex(const XMFLOAT3& p, const XMFLOAT3& n)
		:Position(p), Normal(n){}
	SimpleVertex(
		float px, float py, float pz,
		float nx, float ny, float nz)
		: Position(px,py,pz), Normal(nx,ny,nz), Color(1.0f, 1.0f, 1.0f, 1.0f){}

	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT4 Color;
};

struct Mesh
{
	std::vector<Vertex> Vertices;
	std::vector<UINT> Indices;
};

struct SimpleMesh
{
	std::vector<SimpleVertex> Vertices;
	std::vector<UINT> Indices;
};

static const D3D11_INPUT_ELEMENT_DESC SimpleVertexDescription[] =
{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

static const D3D11_INPUT_ELEMENT_DESC VertexDescription[] =
{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 52, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

class GeometryGenerator
{
public:
	///<summary>
	/// Creates a box centered at the origin with the given dimensions.
	///</summary>
	static void CreateBox(float width, float height, float depth, Mesh& mesh);

	static void CreateBox(float width, float height, float depth, SimpleMesh& mesh);

	///<summary>
	/// Creates a sphere centered at the origin with the given radius.  The
	/// slices and stacks parameters control the degree of tessellation.
	///</summary>
	static void CreateSphere(float radius, UINT sliceCount, UINT stackCount, Mesh& mesh);

	///<summary>
	/// Creates a geosphere centered at the origin with the given radius.  The
	/// depth controls the level of tessellation.
	///</summary>
	void CreateGeosphere(float radius, UINT numSubdivisions, Mesh& mesh);

	///<summary>
	/// Creates a cylinder parallel to the y-axis, and centered about the origin.  
	/// The bottom and top radius can vary to form various cone shapes rather than true
	// cylinders.  The slices and stacks parameters control the degree of tessellation.
	///</summary>
	void CreateCylinder(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, Mesh& mesh);

	///<summary>
	/// Creates an mxn grid in the xz-plane with m rows and n columns, centered
	/// at the origin with the specified width and depth.
	///</summary>
	void CreateGrid(float width, float depth, UINT m, UINT n, Mesh& mesh);

	///<summary>
	/// Creates a quad covering the screen in NDC coordinates.  This is useful for
	/// postprocessing effects.
	///</summary>
	void CreateFullscreenQuad(Mesh& mesh);

private:
	void Subdivide(Mesh& meshData);
	void BuildCylinderTopCap(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, Mesh& mesh);
	void BuildCylinderBottomCap(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, Mesh& mesh);
};