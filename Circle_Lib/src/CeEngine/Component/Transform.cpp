#include "Transform.h"
#include <CeCommon/Tool/CeVM.h>


 
//注意2D模式目前还是要乘以单位长度
Transform::Transform(GameObject* aOwner,const string& aName):
	Component(aOwner, aName), mPosition(0.0f),mRotation(0.0f),mSize(1.0f){}

Transform::~Transform(){}

void Transform::SetCommand(const string & aCmd)
{
	if (aCmd.empty())return;
	 
	vector<Vec3> temp_trans = CeVM::StrToTransform(aCmd);
	if (!temp_trans.size() >= 3)return;
	mPosition = temp_trans[0];	
	mRotation = temp_trans[1];
	mSize = temp_trans[2];
	mCurrentCommand = aCmd;
}

string Transform::GetCommand()
{
	mCurrentCommand = std::to_string(mPosition.x) + ',' +
		std::to_string(mPosition.y) + ',' +
		std::to_string(mPosition.z) + ',' +
		std::to_string(mRotation.x) + ',' +
		std::to_string(mRotation.y) + ',' +
		std::to_string(mRotation.z) + ',' +
		std::to_string(mSize.x) + ',' +
		std::to_string(mSize.y) + ',' +
		std::to_string(mSize.z);
	return mCurrentCommand;
}

void Transform::Start()
{
}

void Transform::Update()
{
}



