#include "d3dStd.h"
#include "SphereRenderComponent.h"
#include "D3DRenderer.h"
#include "Scene.h"
#include "ConstantBuffers.h"
#include "CameraNode.h"

const char* SphereRenderComponent::gName = "SphereRender";

SphereRenderComponent::SphereRenderComponent(ActorPtr pOwner, ID3D11VertexShader* pVertShader, ID3D11PixelShader* pPixelShader, float radius = 1.0f, int sliceCount = 10, int stackCount = 10)
	:ActorComponent(pOwner),
	mpVertexShader(pVertShader),
	mpPixelShader(pPixelShader)
{
	Mesh sphereMesh;
	GeometryGenerator::CreateSphere(radius, sliceCount, stackCount, sphereMesh);

	D3DRenderer* renderer = Scene::getScene()->getRenderer();

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(Vertex) * sphereMesh.Vertices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = &sphereMesh.Vertices[0];
	HR(renderer->device()->CreateBuffer(&bd, &initData, &mSphereVB));


	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * sphereMesh.Indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &sphereMesh.Indices[0];
	HR(renderer->device()->CreateBuffer(&ibd, &iinitData, &mSphereIB));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CBPerObject);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;

	renderer->device()->CreateBuffer(&bd, NULL, &mTransformBuffer);
}

SphereRenderComponent::~SphereRenderComponent()
{
	ReleaseCOM(mSphereVB);
	ReleaseCOM(mSphereIB);
	ReleaseCOM(mTransformBuffer);
}

void SphereRenderComponent::VRender()
{
	ID3D11DeviceContext* context = Scene::getScene()->getRenderer()->context();

	context->IASetInputLayout(Scene::getScene()->getRenderer()->getInputLayout("Vertex"));
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &mSphereVB, &stride, &offset);
	context->IASetIndexBuffer(mSphereIB, DXGI_FORMAT_R32_UINT, 0);

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
	context->DrawIndexed(36, 0, 0);
}