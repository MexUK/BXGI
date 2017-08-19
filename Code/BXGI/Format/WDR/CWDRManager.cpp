#include "CWDRManager.h"
#include "CWDRFormat.h"
#include "Intermediate/Model/CIntermediateModelFormat.h"
#include "Intermediate/Model/CIntermediateModel.h"
#include "Intermediate/Model/Data/CIntermediateGeometry.h"
#include "Intermediate/Model/Data/CIntermediateBoundingObject.h"
#include "CWDRBlock_Model.h"
#include "CWDRBlock_Geometry.h"
#include "Static/CMath.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

CWDRManager::CWDRManager(void)
{
}

void						CWDRManager::init(void)
{
}
void						CWDRManager::uninit(void)
{
}

CWDRFormat*					CWDRManager::convertIntermediateModelFileToWDRFile(CIntermediateModelFormat *pGeneralModelFile)
{
	CWDRFormat *pWDRFile = new CWDRFormat;

	for (CIntermediateModel *pGeneralModel : pGeneralModelFile->getEntries())
	{
		CWDRBlock_Model *pWDRBlock_Model = new CWDRBlock_Model;
		for (CIntermediateGeometry *pGeneralGeometry : pGeneralModel->getEntries())
		{
			CWDRBlock_Geometry *pWDRBlock_Geometry = CWDRFormat::convertIntermediateGeometryToWDRGeometry(pGeneralGeometry);
			pWDRBlock_Model->m_vecGeometries.push_back(pWDRBlock_Geometry);
		}
		pWDRFile->m_vecModels.push_back(pWDRBlock_Model);
	}

	if (pGeneralModelFile->getEntryCount() > 0 && pGeneralModelFile->getEntryByIndex(0)->getEntryCount() > 0)
	{
		// bounding object
		CIntermediateBoundingObject *pBoundingObject = pGeneralModelFile->getEntryByIndex(0)->getEntryByIndex(0)->getBoundingObject();
		if (pBoundingObject->getBoundingObjectType() == BOUNDING_OBJECT_TYPE_SPHERE)
		{
			Vec3f vecMin = CMath::getBoundingCuboidMinFromSphere(pBoundingObject->getCenter(), pBoundingObject->getRadius());
			Vec3f vecMax = CMath::getBoundingCuboidMinFromSphere(pBoundingObject->getCenter(), pBoundingObject->getRadius());
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

uint32						CWDRManager::getPackedOffset(uint32 uiOffset)
{
	return uiOffset | (5 << 28);
}

uint32						CWDRManager::getPackedDataOffset(uint32 uiOffset)
{
	return uiOffset | (6 << 28);
}