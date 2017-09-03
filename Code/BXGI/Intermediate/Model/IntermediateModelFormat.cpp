#include "IntermediateModelFormat.h"
#include "IntermediateModel.h"
#include "Data/IntermediateGeometry.h"

using namespace bxgi;

void				IntermediateModelFormat::unload(void)
{
	for (auto pModel : getEntries())
	{
		for (auto pGeometry : pModel->getEntries())
		{
			pGeometry->getVertices().clear();
			pGeometry->getTriangles().clear();
			delete pGeometry;
		}

		pModel->removeAllEntries();
		delete pModel;
	}

	removeAllEntries();
}