#include "d3dStd.h"
#include "MeshRenderComponent.h"
#include "Scene.h"
#include "D3DRenderer.h"
#include "CameraNode.h"
#include "ConstantBuffers.h"
#include <iostream>

const char* MeshRenderComponent::gName = "MeshRender";

MeshRenderComponent::MeshRenderComponent(const Mesh& mesh, ID3D11VertexShader* pVertexShader, ID3D11PixelShader* pPixelShader)
	:mpVertexShader(pVertexShader),
	mpPixelShader(pPixelShader),
	mIndexCount((UINT)mesh.Indices.size())
{
	D3DRenderer* renderer = Scene::getScene()->getRenderer();

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(Vertex) * (UINT)mesh.Vertices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = &mesh.Vertices[0];
	HR(renderer->device()->CreateBuffer(&bd, &initData, &mMeshVB));


	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * (UINT)mesh.Indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &mesh.Indices[0];
	HR(renderer->device()->CreateBuffer(&ibd, &iinitData, &mMeshIB));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CBPerObject);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;

	renderer->device()->CreateBuffer(&bd, NULL, &mTransformBuffer);
}

MeshRenderComponent::~MeshRenderComponent()
{
	ReleaseCOM(mMeshVB);
	ReleaseCOM(mMeshIB);
	ReleaseCOM(mTransformBuffer);

	//std::cout << "Deleted MeshRenderComponent at " << this << std::endl;
}

void MeshRenderComponent::VRender()
{
	ID3D11DeviceContext* context = Scene::getScene()->getRenderer()->context();

	context->IASetInputLayout(Scene::getScene()->getRenderer()->getInputLayout("Vertex"));
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &mMeshVB, &stride, &offset);
	context->IASetIndexBuffer(mMeshIB, DXGI_FORMAT_R32_UINT, 0);

	XMMATRIX world = *(Scene::getScene()->getTopMatrix());
	XMMATRIX view = *(Scene::getScene()->getCamera()->getView());
	XMMATRIX proj = *(Scene::getScene()->getCamera()->getProjection());

	CBPerObject cb;
	cb.World = world;
	cb.WorldViewProj = world * view * proj;
	cb.WorldInvTranspose = XMMatrixTranspose(XMMatrixInverse(NULL, world));
	cb.TextureTransform = XMMatrixIdentity();

	context->UpdateSubresource(mTransformBuffer, 0, NULL, &cb, 0, 0);

	context->VSSetShader(mpVertexShader, NULL, 0);
	context->VSSetConstantBuffers(0, 1, &mTransformBuffer);
	context->PSSetShader(mpPixelShader, NULL, 0);
	context->PSSetConstantBuffers(0, 1, &mTransformBuffer); //was CSSetConstantBuffers would probably have errored on use
	context->DrawIndexed(mIndexCount, 0, 0);
}