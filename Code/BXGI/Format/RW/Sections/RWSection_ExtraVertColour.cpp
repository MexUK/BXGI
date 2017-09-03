#include "RWSection_ExtraVertColour.h"
#include "RWSection_Geometry.h"
#include "Format/RW/RWFormat.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

RWSection_ExtraVertColour::RWSection_ExtraVertColour(void) :
	m_uiMagicNumber(0)
{
	setSectionId(RW_SECTION_EXTRA_VERT_COLOUR);
}

void							RWSection_ExtraVertColour::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();

	m_uiMagicNumber = pDataReader->readUint32();

	RWFormat *pRWFormat = getRWFormat();
	RWSection *pLatestClump = pRWFormat->getLatestClump();
	//uint32 uiGeometryVertexCount = ((RWSection_Geometry*)pRWSection_ExtraVertColour->getParentNode()->getParentNode())->getVertexColours().size();
	uint32 uiGeometryVertexCount = pLatestClump->getSectionsByType(RW_SECTION_GEOMETRY).size() > 0 ? ((RWSection_Geometry*)(pLatestClump->getSectionsByType(RW_SECTION_GEOMETRY)[0]))->getVertexColours().size() : 0;
	m_vecNightVertexColours.resize(uiGeometryVertexCount);
	for (uint32 i = 0; i < uiGeometryVertexCount; i++)
	{
		m_vecNightVertexColours[i] = pDataReader->readVector4ui8();
	}
}

void							RWSection_ExtraVertColour::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	pDataWriter->writeUint32(m_uiMagicNumber);

	for (Vec4u8& uiVertexColour : getVertexColours())
	{
		pDataWriter->writeVector4ui8(uiVertexColour);
	}
}

void							RWSection_ExtraVertColour::setVertexColours(vector<Vec4u8>& vecNightVertexColours)
{
	RWSection_Geometry *pRWSection_Geometry = (RWSection_Geometry*)getParentNode()->getParentNode();

	m_vecNightVertexColours.clear();
	m_vecNightVertexColours.resize(pRWSection_Geometry->getVertexCount());

	uint32 i = 0, j = vecNightVertexColours.size();
	if (j > pRWSection_Geometry->getVertexCount())
	{
		j = pRWSection_Geometry->getVertexCount();
	}
	for (; i < j; i++)
	{
		m_vecNightVertexColours[i] = vecNightVertexColours[i];
	}

	Vec4u8 vecColour;
	vecColour.x = 0xFF;
	vecColour.y = 0xFF;
	vecColour.z = 0xFF;
	vecColour.w = 0xFF;
	for (i = j, j = pRWSection_Geometry->getVertexCount(); i < j; i++)
	{
		m_vecNightVertexColours[i] = vecColour;
	}
}