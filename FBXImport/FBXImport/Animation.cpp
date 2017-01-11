#include "Animation.h"
#include "AnimationCurve.h"
#include "FbxDXUtil.h"

Animation::Animation()
{
}


Animation::~Animation()
{
}

void Animation::Load(FbxNode& fbxNode, void* pData)
{
	FbxAnimLayer* animLayer = static_cast<FbxAnimLayer*>(pData);

	animLayer->GetName(); // Get the layer's name if needed			

	
	FbxAnimCurve *scalingCurve = fbxNode.LclScaling.GetCurve(animLayer);	
	FbxAnimCurve *rotationCurve = fbxNode.LclRotation.GetCurve(animLayer);
	FbxAnimCurve *translationCurve = fbxNode.LclTranslation.GetCurve(animLayer);

	DebugBox(scalingCurve->KeyGetCount(), 0);
	DebugBox(rotationCurve->KeyGetCount(), 0);
	DebugBox(translationCurve->KeyGetCount(), 0);

	//if (scalingCurve != 0)
	//{
	//	int numKeys = scalingCurve->KeyGetCount();
	//	for (int keyIndex = 0; keyIndex < numKeys; keyIndex++)
	//	{
	//		FbxTime frameTime = scalingCurve->KeyGetTime(keyIndex);
	//		FbxDouble3 scalingVector = fbxNode.EvaluateLocalScaling(frameTime);
	//		float x = (float)scalingVector[0];
	//		float y = (float)scalingVector[1];
	//		float z = (float)scalingVector[2];

	//		float frameSeconds = (float)frameTime.GetSecondDouble(); // If needed, get the time of the scaling keyframe, in seconds
	//	}
	//}
	//else
	//{
	//	// If this animation layer has no scaling curve, then use the default one, if needed
	//	FbxDouble3 scalingVector = fbxNode.LclScaling.Get();
	//	float x = (float)scalingVector[0];
	//	float y = (float)scalingVector[1];
	//	float z = (float)scalingVector[2];
	//}
}

void Animation::Save(FbxNode& fbxNode)
{

}
