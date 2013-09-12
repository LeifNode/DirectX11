
#include "TestApp1.h"
#include "MathHelper.h"
#include "D3DRenderer.h"
#include "Geometry.h"
#include "Scene.h"
#include "CameraNode.h"
#include "Actor.h"
#include "TransformComponent.h"
#include "RotationAnimatorComponent.h"
#include "MeshRenderComponent.h"
#include "BoidManager.h"
#include "CohesionSteering.h"
#include "SeparationSteering.h"
#include "AlignSteering.h"
#include "KinematicComponent.h"
#include "Events.h"
#include <iostream>
#include <DirectXMath.h>
#include "..\3rdParty\FastDelegate\FastDelegate.h"

struct CBChangesPerObject
{
	XMMATRIX ToWorld;
	XMMATRIX WorldViewProj;
};

struct CBChangesPerFrame
{

};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	UNREFERENCED_PARAMETER( prevInstance );
    UNREFERENCED_PARAMETER( cmdLine );

#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	TestApp1* application = new TestApp1(hInstance);

	if (!application->Initialize())
		return 0;

	int returnVal = application->Run();
//
//#if defined(DEBUG) | defined(_DEBUG)
//	ID3D11Debug* debugInterface;
//
//	application->getRenderer()->device()->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&debugInterface));
//
//	delete application;
//	application = nullptr;
//
//	debugInterface->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
//#endif

	delete application;

	return returnVal;
}

TestApp1::TestApp1(HINSTANCE hInstance)
	:
	D3DApp(hInstance),
	mpPixelShader(NULL),
	mpVertexShader(NULL),
	mInputLayout(NULL),
	mTheta(1.5f*MathHelper::Pi), mPhi(0.25f*MathHelper::Pi), mRadius(10.0f), currentRot(0.0f)
{
	mMainWndCaption = L"Test App";

	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mView, I);
	XMStoreFloat4x4(&mProj, I);

	mLastMousePos.x = 0;
	mLastMousePos.y = 0;
}

TestApp1::~TestApp1()
{
	delete mpScene;
	mpScene = nullptr;

	unhookInputEvents();

	ReleaseCOM(mpVertexShader);
	ReleaseCOM(mpPixelShader);
}

bool TestApp1::Initialize()
{
	if (!D3DApp::Initialize())
		return false;

	onResize();

	mpScene = new Scene(mpRenderer);

	buildFX();
	createActors();

	hookInputEvents();

	return true;
}

void TestApp1::hookInputEvents()
{
	EventSystem::get()->addListener(EventData_KeyboardDown::skEventType, fastdelegate::MakeDelegate(this, &TestApp1::onKeyDown));
}

void TestApp1::unhookInputEvents()
{
	EventSystem::get()->removeListener(EventData_KeyboardDown::skEventType, fastdelegate::MakeDelegate(this, &TestApp1::onKeyDown));
}

void TestApp1::onKeyDown(IEventDataPtr eventData)
{
	shared_ptr<EventData_KeyboardDown> dataPtr = static_pointer_cast<EventData_KeyboardDown>(eventData);

	if (dataPtr->getKey() == KEY_G)
		std::cout << (char)dataPtr->getKey() << std::endl;
}

void TestApp1::buildFX()
{
	ID3DBlob* VSBlob = NULL;

	HRESULT hr = D3DRenderer::compileShaderFromFile(L"color.fx", "VS", "vs_5_0", &VSBlob);
	if (FAILED(hr))
	{
		MessageBox(0, L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );

		return;
	}

	hr = mpRenderer->device()->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), NULL, &mpVertexShader);
	if (FAILED(hr))
	{
		VSBlob->Release();
		
		MessageBox(0, L"Unable to create vertex shader.", L"Error", MB_OK);
	}

	mpRenderer->buildInputLayouts(VSBlob);
	mInputLayout = mpRenderer->getInputLayout("Vertex");

	VSBlob->Release();

	ID3DBlob* PSBlob = NULL;
	hr = D3DRenderer::compileShaderFromFile(L"color.fx", "PS", "ps_5_0", &PSBlob);
	if (FAILED(hr))
	{
		MessageBox( NULL,
                    L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
		return;
	}

	HR(mpRenderer->device()->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), NULL, &mpPixelShader));
	PSBlob->Release();
}

void TestApp1::createActors()
{
	mpBoidManager = new BoidManager();
	unsigned currentId = 1;

	Mesh boxMesh;
	GeometryGenerator::CreateBox(1.0f, 1.0f, 1.0f, boxMesh);

	for (unsigned i = 0; i < 1; i++)
	{
		ActorPtr newBoid = ActorPtr(new Actor(currentId++));

		TransformComponent* boidTransform = new TransformComponent(Vector3((rand()/RAND_MAX)*5.0f, (rand()/RAND_MAX)*5.0f, (rand()/RAND_MAX)*5.0f),
																   Vector3(), 
																   Vector3(5.0f));
		MeshRenderComponent* BoidMesh = new MeshRenderComponent(boxMesh, mpVertexShader, mpPixelShader);
		KinematicComponent* boidKinematic = new KinematicComponent(mpBoidManager, 0.01f, 0.4f);
		boidKinematic->addSteering(new SeparationSteering(5.0f), 1.0f);
		boidKinematic->addSteering(new CohesionSteering(10.0f, 0.0f), 1.0f);
		boidKinematic->addSteering(new AlignSteering(15.0f, 0.0f), 2.0f);

		newBoid->addComponent(ActorComponentPtr(boidTransform));
		newBoid->addComponent(ActorComponentPtr(BoidMesh));
		newBoid->addComponent(ActorComponentPtr(boidKinematic));

		newBoid->initialize(NULL);

		mpScene->addChild(newBoid);
	}
}

void TestApp1::onResize()
{
	D3DApp::onResize();
	
	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f*MathHelper::Pi, aspectRatio(), 1.0f, 1000.0f);
	//P = XMMatrixTranspose(P);
	XMStoreFloat4x4(&mProj, P);
}

void TestApp1::Update(float dt)
{
	float x = mRadius*sinf(mPhi)*cosf(mTheta);
	float z = mRadius*sinf(mPhi)*sinf(mTheta);
	float y = mRadius*cosf(mPhi);

	XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX view = XMMatrixLookAtLH(pos, target, up);
	XMStoreFloat4x4(&mView, view);

	mpScene->update(dt);

	mpScene->getCamera()->setProj(XMLoadFloat4x4(&mProj));
	mpScene->getCamera()->setView(XMLoadFloat4x4(&mView));
}

void TestApp1::Draw()
{
	mpRenderer->preRender();
	
	mpScene->render();

	mpRenderer->postRender();
}

void TestApp1::onMouseDown(WPARAM btnState, int x, int y)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;

	SetCapture(mhWnd);
}

void TestApp1::onMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void TestApp1::onMouseMove(WPARAM btnState, int x, int y)
{
	if( (btnState & MK_LBUTTON) != 0 )
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePos.y));

		// Update angles based on input to orbit camera around box.
		mTheta += dx;
		mPhi   += dy;

		// Restrict the angle mPhi.
		mPhi = MathHelper::Clamp(mPhi, 0.1f, MathHelper::Pi-0.1f);
	}
	else if( (btnState & MK_RBUTTON) != 0 )
	{
		// Make each pixel correspond to 0.005 unit in the scene.
		float dx = 0.005f*static_cast<float>(x - mLastMousePos.x);
		float dy = 0.005f*static_cast<float>(y - mLastMousePos.y);

		// Update the camera radius based on input.
		mRadius += dx - dy;

		// Restrict the radius.
		mRadius = MathHelper::Clamp(mRadius, 3.0f, 15.0f);
	}

	mLastMousePos.x = x;
	mLastMousePos.y = y;
}