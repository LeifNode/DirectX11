#include <iostream>
#include <fstream>
#include <math.h>
#include "BoidHandler.h"
#include "BoidManager.h"
#include "Scene.h"
#include "Actor.h"
#include "TransformComponent.h"
#include "MeshRenderComponent.h"
#include "KinematicComponent.h"
#include "Geometry.h"
#include "BlendedSteering.h"
#include "AlignSteering.h"
#include "SeparationSteering.h"
#include "CohesionSteering.h"
#include "MathHelper.h"
#include "EventSystem.h"
#include "..\3rdParty\FastDelegate\FastDelegate.h"

const std::string BoidHandler::BEHAVIOR_NAMES[3] = {"separation", "cohesion", "align" };
const std::string BoidHandler::PARAMETER_NAMES[3] = { "radius", "min dot product", "weight" };

BoidHandler::BoidHandler()
	:mCurrentID(0),
	mpBoidManager(nullptr),
	mIncrementValue(1.0f),
	mSelectedBehavior(0),
	mSelectedParameter(0)
{
	mpBoidManager = new BoidManager();
}

BoidHandler::~BoidHandler()
{
	clear();

	delete mpBoidManager;

	unhookInputEvents();
	saveConfiguration();
}

void BoidHandler::initialize(const Mesh& boidMesh, ID3D11VertexShader* vertShader, ID3D11PixelShader* pixelShader)
{
	mBoidMesh = boidMesh;
	mpVertexShader = vertShader;
	mpPixelShader = pixelShader;

	loadConfiguration();
	hookInputEvents();
}

void BoidHandler::hookInputEvents()
{
	EventSystem::get()->addListener(EventData_KeyboardDown::skEventType, fastdelegate::MakeDelegate(this, &BoidHandler::onKeyDown));
}

void BoidHandler::unhookInputEvents()
{
	EventSystem::get()->removeListener(EventData_KeyboardDown::skEventType, fastdelegate::MakeDelegate(this, &BoidHandler::onKeyDown));
}

void BoidHandler::clear()
{
	for (auto it = mBoids.begin(); it != mBoids.end(); ++it)
	{
		Scene::getScene()->removeChild((*it)->getOwner()->getId());
	}

	mpBoidManager->clear();
}

void BoidHandler::addBoid(float x, float y, float z)
{
	ActorPtr newBoid = ActorPtr(new Actor(++mCurrentID));

	TransformComponent* boidTransform = new TransformComponent(Vector3(x, y, z),
																Vector3(), 
																Vector3(0.5f));
	MeshRenderComponent* BoidMesh = new MeshRenderComponent(mBoidMesh, mpVertexShader, mpPixelShader);
	KinematicComponent* boidKinematic = new KinematicComponent(mpBoidManager, Vector3(), 10.0f, 15.0f);
	
	addSteeringBehaviors(boidKinematic);

	newBoid->addComponent(ActorComponentPtr(boidTransform));
	newBoid->addComponent(ActorComponentPtr(BoidMesh));
	newBoid->addComponent(ActorComponentPtr(boidKinematic));

	newBoid->initialize(NULL);

	Scene::getScene()->addChild(newBoid);
	mBoids.push_back(boidKinematic);
}

void BoidHandler::addSteeringBehaviors(KinematicComponent* kinematic)
{
	BlendedSteering* blendedSteering = new BlendedSteering();
	kinematic->addSteering(blendedSteering);

	blendedSteering->addSteering(new SeparationSteering(mBoidParameters[0][0], mBoidParameters[0][1]), mBoidParameters[0][2]);
	blendedSteering->addSteering(new CohesionSteering(mBoidParameters[1][0], mBoidParameters[1][1]), mBoidParameters[1][2]);
	blendedSteering->addSteering(new AlignSteering(mBoidParameters[2][0], mBoidParameters[2][1]), mBoidParameters[2][2]);
}

void BoidHandler::addBoidGroup(unsigned count)
{
	const float MAX_VARIATION = 50.0f;
	const float MAX_DISPLACEMENT = 4.0f;

	Vector3 position = Vector3((rand()/(float)RAND_MAX) - (rand()/(float)RAND_MAX),
							   (rand()/(float)RAND_MAX) - (rand()/(float)RAND_MAX),
							   (rand()/(float)RAND_MAX) - (rand()/(float)RAND_MAX));
	position *= MAX_VARIATION;

	for (unsigned i = 0; i < count; i++)
	{
		XMFLOAT3 randDisplacement;
		XMStoreFloat3(&randDisplacement, XMVectorMultiply(MathHelper::RandUnitVec3(), XMVectorSet(MAX_DISPLACEMENT, MAX_DISPLACEMENT, MAX_DISPLACEMENT, MAX_DISPLACEMENT)));

		addBoid(position.x + randDisplacement.x, position.y + randDisplacement.y, position.z + randDisplacement.z);
	}
}

void BoidHandler::removeRandomBoid()
{
	if (mBoids.size() > 0)
	{
		unsigned boidPos = rand() % mBoids.size();

		Actor* boid = mBoids[boidPos]->getOwner();
	
		auto iterator = mBoids.begin()++;

		for (unsigned i = 0; i < boidPos; i++)
			++iterator;

		mBoids.erase(iterator);

		Scene::getScene()->removeChild(boid->getId());
	}
}

bool BoidHandler::loadConfiguration()
{
	std::fstream configFile("boidConfig.cfg", std::ios::in | std::ios::binary);

	if (configFile)
	{
		for (unsigned b = 0; b < 3; b++)
		{
			for (unsigned p = 0; p < 3; p++)
			{
				configFile.read(reinterpret_cast<char*>(&mBoidParameters[b][p]), sizeof(float));
			}
		}

		configFile.close();
		printParameters();
	}
	else
	{
		std::cout << "Failed to load parameters from file. Loading default values." << std::endl;
		loadDefaultValues();

		printParameters();

		return false;
	}

	return true;
}

void BoidHandler::saveConfiguration()
{
	std::fstream configFile("boidConfig.cfg", std::ios::out | std::ios::binary);

	assert(configFile);

	for (unsigned b = 0; b < 3; b++)
	{
		for (unsigned p = 0; p < 3; p++)
		{
			configFile.write(reinterpret_cast<char*>(&mBoidParameters[b][p]), sizeof(float));
		}
	}

	configFile.close();
}

void BoidHandler::loadDefaultValues()
{
	//Separation parameters
	mBoidParameters[0][0] = 3.0f;
	mBoidParameters[0][1] = -1.0f;
	mBoidParameters[0][2] = 15.0f;

	//Cohesion parameters
	mBoidParameters[1][0] = 14.0f;
	mBoidParameters[1][1] = -0.7f;
	mBoidParameters[1][2] = 2.0f;

	//Align parameters
	mBoidParameters[2][0] = 18.0f;
	mBoidParameters[2][1] = -1.0f;
	mBoidParameters[2][2] = 6.0f;
}

void BoidHandler::updateBoidParameters()
{
	for (auto it = mBoids.begin(); it != mBoids.end(); ++it)
	{
		(*it)->clear();
		addSteeringBehaviors((*it));
	}
}

void BoidHandler::printParameters()
{
	for (unsigned b = 0; b < 3; b++)
	{
		for (unsigned p = 0; p < 3; p++)
		{
			std::cout << BEHAVIOR_NAMES[b] << " " << PARAMETER_NAMES[p] << " = " << mBoidParameters[b][p] << std::endl;
		}
	}
}

void BoidHandler::onKeyDown(IEventDataPtr e)
{
	shared_ptr<EventData_KeyboardDown> dataPtr = static_pointer_cast<EventData_KeyboardDown>(e);

	switch (dataPtr->getKey())
	{
	case KEY_P:
		printParameters();
		break;
	case KEY_C:
		std::cout << "Current number of boids: " << mBoids.size() << std::endl;
		break;
	case KEY_A:
		addBoidGroup();
		break;
	case KEY_D:
		removeRandomBoid();
		break;
	case KEY_Z:
		loadDefaultValues();
		updateBoidParameters();
		std::cout << "Loaded default values." << std::endl;
		break;
	case KEY_R:
		updateBoidParameters();
		break;
	case KEY_S:
		saveConfiguration();
		break;
	case KEY_L:
		loadConfiguration();
		break;
	case KEY_UP:
		mSelectedBehavior++;
		if (mSelectedBehavior > 2)
			mSelectedBehavior = 0;
		std::cout << "Selected " << BEHAVIOR_NAMES[mSelectedBehavior] << " behavior" << std::endl;
		break;
	case KEY_DOWN:
		mSelectedBehavior--;
		if (mSelectedBehavior < 0)
			mSelectedBehavior = 2;
		std::cout << "Selected " << BEHAVIOR_NAMES[mSelectedBehavior] << " behavior" << std::endl;
		break;
	case KEY_RIGHT:
		mSelectedParameter++;
		if (mSelectedParameter > 2)
			mSelectedParameter = 0;
		std::cout << "Selected " << PARAMETER_NAMES[mSelectedParameter] << " parameter" << std::endl;
		break;
	case KEY_LEFT:
		mSelectedParameter--;
		if (mSelectedParameter < 0)
			mSelectedParameter = 2;
		std::cout << "Selected " << PARAMETER_NAMES[mSelectedParameter] << " parameter" << std::endl;
		break;
	case KEY_SHIFT:
		mIncrementValue += 0.1f;
		std::cout << "Increment value increased to " << mIncrementValue << std::endl;
		break;
	case KEY_CONTROL:
		mIncrementValue -= 0.1f;
		std::cout << "Increment value decreased to " << mIncrementValue << std::endl;
		break;
	case KEY_ADD:
	case KEY_RETURN:
		mBoidParameters[mSelectedBehavior][mSelectedParameter] += mIncrementValue;

		if (mSelectedParameter == 0 && mBoidParameters[mSelectedBehavior][0] < 0.0f)
			mBoidParameters[mSelectedBehavior][0] = 0.0f; //Limit radius to 0
		else if (mSelectedParameter == 1 && mBoidParameters[mSelectedBehavior][1] < -1.0f)
			mBoidParameters[mSelectedBehavior][1] = -1.0f; //Limit min dot product to 0
		else if (mSelectedParameter == 1 && mBoidParameters[mSelectedBehavior][1] > 1.0f)
			mBoidParameters[mSelectedBehavior][1] = 1.0f; //Limit min dot product to 1

		std::cout << BEHAVIOR_NAMES[mSelectedBehavior] << " " << PARAMETER_NAMES[mSelectedParameter] << " incremented to " << mBoidParameters[mSelectedBehavior][mSelectedParameter] << std::endl;
		updateBoidParameters();
		break;
	case KEY_SUBTRACT:
		mBoidParameters[mSelectedBehavior][mSelectedParameter] -= mIncrementValue;

		if (mSelectedParameter == 0 && mBoidParameters[mSelectedBehavior][0] < 0.0f)
			mBoidParameters[mSelectedBehavior][0] = 0.0f; //Limit radius to 0
		else if (mSelectedParameter == 1 && mBoidParameters[mSelectedBehavior][1] < -1.0f)
			mBoidParameters[mSelectedBehavior][1] = -1.0f; //Limit min dot product to 0
		else if (mSelectedParameter == 1 && mBoidParameters[mSelectedBehavior][1] > 1.0f)
			mBoidParameters[mSelectedBehavior][1] = 1.0f; //Limit min dot product to 1

		std::cout << BEHAVIOR_NAMES[mSelectedBehavior] << " " << PARAMETER_NAMES[mSelectedParameter] << " incremented to " << mBoidParameters[mSelectedBehavior][mSelectedParameter] << std::endl;
		updateBoidParameters();
		break;
	}
}