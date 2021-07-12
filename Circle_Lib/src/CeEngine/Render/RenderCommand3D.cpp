#include "RenderCommand3D.h"
#include "Renderer.h"
RenderCommand3D::RenderCommand3D(const GameObjectInfo & aGOInfo, const MaterialInof & aMTInfo, const MeshInfo & aMSInfo, const CameraInfo & aCInfo):
	mGOInfo(aGOInfo), mMTInfo(aMTInfo), mMSInfo(aMSInfo), mCInfo(aCInfo)
{
}

RenderCommand3D::~RenderCommand3D()
{
}

