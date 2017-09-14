#include "DFFFormat.h"
#include "Format/RW/TextureEntry.h"
#include "Static/String.h"
#include "Engine/RW/RWVersion.h"
#include "Format/RW/Sections/RWSection_Geometry.h"
#include "Static/File.h"
#include "Intermediate/Model/IntermediateModel.h"
#include "Format/RW/Sections/RWSection_ExtraVertColour.h"
#include "Format/RW/Sections/RWSection_2dEffect.h"
#include "Format/RW/Sections/RWSection_Extension.h"
#include "Format/RW/Sections/RWSection_Frame.h"
#include "Static/Debug.h"
#include "Format/RW/RWSection.h"

using namespace std;
using namespace bxgi;

// converting
IntermediateModelFormat*	DFFFormat::convertToIntermediateModelFormat(void)
{
	IntermediateModelFormat *pGeneralModelFile = new IntermediateModelFormat;

	IntermediateModel *pGeneralModel = new IntermediateModel;
	for (RWSection *pRWSection : getSectionsByType(RW_SECTION_GEOMETRY))
	{
		RWSection_Geometry *pRWSection_Geometry = (RWSection_Geometry*) pRWSection;
		IntermediateGeometry *pGeneralGeometry = pRWSection_Geometry->convertToIntermediateGeometry();
		pGeneralModel->addEntry(pGeneralGeometry);
	}
	pGeneralModelFile->addEntry(pGeneralModel);

	return pGeneralModelFile;
}

// model name fetching
vector<string>				DFFFormat::getModelNames(void)
{
	vector<string> vecModelNames;
	for (RWSection *pRWSection : getSectionsByType(RW_SECTION_FRAME))
	{
		RWSection_Frame *pFrame = (RWSection_Frame*)pRWSection;
		vecModelNames.push_back(pFrame->getData());
	}
	return vecModelNames;
}