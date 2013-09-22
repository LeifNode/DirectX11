#pragma once

#include <string>
#include <vector>
#include "Geometry.h"
#include "Events.h"

class BoidManager;
class KinematicComponent;

class BoidHandler
{
public:
	BoidHandler();
	~BoidHandler();

	void initialize(const Mesh& boidMesh, ID3D11VertexShader* vertShader, ID3D11PixelShader* pixelShader);
	
	void clear();

	void addBoid(float x, float y, float z);
	void addBoidGroup(unsigned count = 5);
	void removeRandomBoid();

	bool loadConfiguration();
	void saveConfiguration();

	void loadDefaultValues();

	void updateBoidParameters();

	void printParameters();

	void onKeyDown(IEventDataPtr e);

private:
	static const std::string BEHAVIOR_NAMES[3];
	static const std::string PARAMETER_NAMES[3];

private:
	void hookInputEvents();
	void unhookInputEvents();

	void addSteeringBehaviors(KinematicComponent*);

private:
	unsigned mCurrentID;

	BoidManager* mpBoidManager;
	std::vector<KinematicComponent*> mBoids;

	float mBoidParameters[3][3];

	int mSelectedBehavior;
	int mSelectedParameter;
	float mIncrementValue;

	Mesh mBoidMesh;
	ID3D11VertexShader* mpVertexShader;
	ID3D11PixelShader* mpPixelShader;

	//float mSeparationRadius, mSeparationMinDot, mSeparationWeight;
	//float mCohesionRadius, mCohesionMinDot, mCohesionWeight;
	//float mAlignRadius, mAlignMinDot, mAlignWeight;
};