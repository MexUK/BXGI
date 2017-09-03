#include "WDRManager.h"
#include "WDRFormat.h"
#include "Intermediate/Model/IntermediateModelFormat.h"
#include "Intermediate/Model/IntermediateModel.h"
#include "Intermediate/Model/Data/IntermediateGeometry.h"
#include "Intermediate/Model/Data/IntermediateBoundingObject.h"
#include "WDRBlock_Model.h"
#include "WDRBlock_Geometry.h"
#include "Static/Math.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

WDRManager::WDRManager(void)
{
}

void						WDRManager::init(void)
{
}
void						WDRManager::uninit(void)
{
}

WDRFormat*					WDRManager::convertIntermediateModelFileToWDRFile(IntermediateModelFormat *pGeneralModelFile)
{
	WDRFormat *pWDRFile = new WDRFormat;

	for (IntermediateModel *pGeneralModel : pGeneralModelFile->getEntries())
	{
		WDRBlock_Model *pWDRBlock_Model = new WDRBlock_Model;
		for (IntermediateGeometry *pGeneralGeometry : pGeneralModel->getEntries())
		{
			WDRBlock_Geometry *pWDRBlock_Geometry = WDRFormat::convertIntermediateGeometryToWDRGeometry(pGeneralGeometry);
			pWDRBlock_Model->m_vecGeometries.push_back(pWDRBlock_Geometry);
		}
		pWDRFile->m_vecModels.push_back(pWDRBlock_Model);
	}

	if (pGeneralModelFile->getEntryCount() > 0 && pGeneralModelFile->getEntryByIndex(0)->getEntryCount() > 0)
	{
		// bounding object
		IntermediateBoundingObject *pBoundingObject = pGeneralModelFile->getEntryByIndex(0)->getEntryByIndex(0)->getBoundingObject();
		if (pBoundingObject->getBoundingObjectType() == BOUNDING_OBJECT_TYPE_SPHERE)
		{
			Vec3f vecMin = Math::getBoundingCuboidMinFromSphere(pBoundingObject->getCenter(), pBoundingObject->getRadius());
			Vec3f vecMax = Math::getBoundingCuboidMinFromSphere(pBoundingObject->getCenter(), pBoundingObject->getRadius());
			pWDRFile->m_boundingObject.m_fMin[0] = vecMin.x;
			pWDRFile->m_boundingObject.m_fMin[1] = vecMin.y;
			pWDRFile->m_boundingObject.m_fMin[2] = vecMin.z;
			pWDRFile->m_boundingObject.m_fMin[3] = 1.0f;
			pWDRFile->m_boundingObject.m_fMax[0] = vecMax.x;
			pWDRFile->m_boundingObject.m_fMax[1] = vecMax.y;
			pWDRFile->m_boundingObject.m_fMax[2] = vecMax.z;
			pWDRFile->m_boundingObject.m_fMax[3] = 1.0f;
			// todo - this takes 4 floats so is probably in a different format
		}
		else if (pBoundingObject->getBoundingObjectType() == BOUNDING_OBJECT_TYPE_CUBOID)
		{
			pWDRFile->m_boundingObject.m_fMin[0] = pBoundingObject->getMin().x;
			pWDRFile->m_boundingObject.m_fMin[1] = pBoundingObject->getMin().y;
			pWDRFile->m_boundingObject.m_fMin[2] = pBoundingObject->getMin().z;
			pWDRFile->m_boundingObject.m_fMin[3] = pBoundingObject->getMin().w;
			pWDRFile->m_boundingObject.m_fMax[0] = pBoundingObject->getMax().x;
			pWDRFile->m_boundingObject.m_fMax[1] = pBoundingObject->getMax().y;
			pWDRFile->m_boundingObject.m_fMax[2] = pBoundingObject->getMax().z;
			pWDRFile->m_boundingObject.m_fMax[3] = pBoundingObject->getMax().w;
		}
	}

	return pWDRFile;
}

uint32						WDRManager::getPackedOffset(uint32 uiOffset)
{
	return uiOffset | (5 << 28);
}

uint32						WDRManager::getPackedDataOffset(uint32 uiOffset)
{
	return uiOffset | (6 << 28);
}