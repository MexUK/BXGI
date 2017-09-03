#include "CDFFFormat.h"
#include "Format/RW/CTextureEntry.h"
#include "Static/String2.h"
#include "Engine/RW/CRWVersion.h"
#include "Format/RW/Sections/CRWSection_Geometry.h"
#include "Static/File.h"
#include "Intermediate/Model/CIntermediateModel.h"
#include "Format/RW/Sections/CRWSection_ExtraVertColour.h"
#include "Format/RW/Sections/CRWSection_2dEffect.h"
#include "Format/RW/Sections/CRWSection_Extension.h"
#include "Static/Debug.h"
#include "Format/RW/CRWSection.h"

using namespace std;
using namespace bxgi;

CIntermediateModelFormat*	CDFFFormat::convertToIntermediateModelFormat(void)
{
	CIntermediateModelFormat *pGeneralModelFile = new CIntermediateModelFormat;

	CIntermediateModel *pGeneralModel = new CIntermediateModel;
	for (CRWSection *pRWSection : getSectionsByType(RW_SECTION_GEOMETRY))
	{
		CRWSection_Geometry *pRWSection_Geometry = (CRWSection_Geometry*) pRWSection;
		CIntermediateGeometry *pGeneralGeometry = pRWSection_Geometry->convertToIntermediateGeometry();
		pGeneralModel->addEntry(pGeneralGeometry);
	}
	pGeneralModelFile->addEntry(pGeneralModel);

	return pGeneralModelFile;
}