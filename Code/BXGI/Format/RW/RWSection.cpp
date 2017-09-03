#include "RWSection.h"
#include "Engine/RW/RWManager.h"
#include "Stream/DataWriter.h"
#include "Sections/RWSection_2dEffect.h"
#include "Sections/RWSection_Atomic.h"
#include "Sections/RWSection_BinMeshPLG.h"
#include "Sections/RWSection_Clump.h"
#include "Sections/RWSection_Extension.h"
#include "Sections/RWSection_ExtraVertColour.h"
#include "Sections/RWSection_Frame.h"
#include "Sections/RWSection_FrameList.h"
#include "Sections/RWSection_Geometry.h"
#include "Sections/RWSection_GeometryList.h"
#include "Sections/RWSection_Light.h"
#include "Sections/RWSection_Material.h"
#include "Sections/RWSection_MaterialList.h"
#include "Sections/RWSection_String.h"
#include "Sections/RWSection_Texture.h"
#include "Sections/RWSection_TextureDictionary.h"
#include "Sections/RWSection_TextureNative.h"
#include "Sections/RWSection_UnknownSection.h"
#include "Static/String.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

unordered_map<ERWSection, bool>		RWSection::m_umapRWSectionsContainingStruct;

RWSection::RWSection(void) :
	m_uiSectionId(RW_SECTION_UNKNOWN),
	m_uiSectionSize(0),
	m_uiSectionRWVersion(0),
	m_bSectionHeaderSkipped(false),
	m_uiStructSectionSize(0),
	m_bUnknownSection(false)
{
}

void				RWSection::initStatic(void)
{
	initRWSections();
}

void				RWSection::initRWSections(void)
{
	vector<ERWSection> vecRWSectionIds = getRWSections();
	for (ERWSection ERWSectionValue : vecRWSectionIds)
	{
		m_umapRWSectionsContainingStruct[ERWSectionValue] = doesRWSectionContainStruct_BeforeInit(ERWSectionValue);
	}
}

void				RWSection::serialize(void)
{
	/*
	todo
	if (m_uiSectionId == RW_SECTION_UNKNOWN)
	{
		return;
	}
	*/
	
	DataWriter *pDataWriter = DataWriter::get();
	uint32 uiVersionCC = RWManager::get()->getSerializationRWVersion();
	
	if (!isSectionHeaderSkipped())
	{
		pDataWriter->writeUint32(m_uiSectionId);
		pDataWriter->writeUint32(0); // placeholder
		pDataWriter->writeUint32(uiVersionCC);
	}
	
	if (doesRWSectionContainStruct(m_uiSectionId))
	{
		pDataWriter->writeUint32(RW_SECTION_STRUCT);
		pDataWriter->writeUint32(0); // placeholder
		pDataWriter->writeUint32(uiVersionCC);
	}
}

RWSection*			RWSection::addSection(ERWSection ERWSectionValue, ERWVersion ERWVersionValue)
{
	RWSection *pRWSection = RWSectionContainer::addSection(ERWSectionValue, ERWVersionValue);
	pRWSection->setParentNode(this);
	return pRWSection;
}

void				RWSection::removeSection(void)
{
	uint32 uiSectionIndex = getSectionIndex();
	if (uiSectionIndex == -1)
	{
		return;
	}
	getParentNode()->removeSectionByIndex(uiSectionIndex);
}

uint32				RWSection::getSectionIndex(void)
{
	uint32 i = 0;
	for (RWSection *pRWSection : getParentNode()->getEntries())
	{
		if (pRWSection == this)
		{
			return i;
		}
		i++;
	}
	return -1;
}

void				RWSection::fillPlaceholdersForSerialization(uint32 uiSectionByteCount, uint32 uiSectionStructByteCount)
{
	DataWriter *pDataWriter = DataWriter::get();

	string &strData = pDataWriter->getData();

	if (!isSectionHeaderSkipped())
	{
		uint32 uiSectionStartPosition = uiSectionByteCount + 12;
		strData = strData.substr(0, (strData.length() - uiSectionStartPosition) + 4) + String::packUint32(uiSectionByteCount, false) + strData.substr((strData.length() - uiSectionStartPosition) + 8);
	}

	if (doesRWSectionContainStruct(m_uiSectionId))
	{
		uint32 uiStructStartPosition = uiSectionByteCount;
		strData = strData.substr(0, (strData.length() - uiStructStartPosition) + 4) + String::packUint32(uiSectionStructByteCount, false) + strData.substr((strData.length() - uiStructStartPosition) + 8);
	}
}

RWSection*			RWSection::creatERWSection(ERWSection ERWSectionValue)
{
	switch (ERWSectionValue)
	{
	case RW_SECTION_2D_EFFECT:			return new RWSection_2dEffect;
	case RW_SECTION_ATOMIC:				return new RWSection_Atomic;
	case RW_SECTION_BIN_MESH_PLG:		return new RWSection_BinMeshPLG;
	case RW_SECTION_CLUMP:				return new RWSection_Clump;
	case RW_SECTION_EXTENSION:			return new RWSection_Extension;
	case RW_SECTION_EXTRA_VERT_COLOUR:	return new RWSection_ExtraVertColour;
	case RW_SECTION_FRAME:				return new RWSection_Frame;
	case RW_SECTION_FRAME_LIST:			return new RWSection_FrameList;
	case RW_SECTION_GEOMETRY:			return new RWSection_Geometry;
	case RW_SECTION_GEOMETRY_LIST:		return new RWSection_GeometryList;
	case RW_SECTION_LIGHT:				return new RWSection_Light;
	case RW_SECTION_MATERIAL:			return new RWSection_Material;
	case RW_SECTION_MATERIAL_LIST:		return new RWSection_MaterialList;
	case RW_SECTION_STRING:				return new RWSection_String;
	case RW_SECTION_TEXTURE:			return new RWSection_Texture;
	case RW_SECTION_TEXTURE_DICTIONARY:	return new RWSection_TextureDictionary;
	case RW_SECTION_TEXTURE_NATIVE:		return new RWSection_TextureNative;
	case RW_SECTION_UNKNOWN:			return new RWSection_UnknownSection;
	}

	RWSection *pRWSection = new RWSection_UnknownSection;
	pRWSection->setSectionId(ERWSectionValue);
	return pRWSection;
}

bool				RWSection::doesRWSectionContainStruct_BeforeInit(ERWSection ERWSectionValue)
{
	return ERWSectionValue == RW_SECTION_ATOMIC
		|| ERWSectionValue == RW_SECTION_CLUMP
		|| ERWSectionValue == RW_SECTION_GEOMETRY
		|| ERWSectionValue == RW_SECTION_GEOMETRY_LIST
		|| ERWSectionValue == RW_SECTION_FRAME_LIST
		|| ERWSectionValue == RW_SECTION_MATERIAL
		|| ERWSectionValue == RW_SECTION_MATERIAL_LIST
		|| ERWSectionValue == RW_SECTION_TEXTURE
		|| ERWSectionValue == RW_SECTION_TEXTURE_DICTIONARY
		|| ERWSectionValue == RW_SECTION_TEXTURE_NATIVE;
}
bool				RWSection::doesRWSectionContainStruct(ERWSection ERWSectionValue)
{
	return m_umapRWSectionsContainingStruct.count(ERWSectionValue) == 1 && m_umapRWSectionsContainingStruct[ERWSectionValue] == true;
}

vector<ERWSection>	RWSection::getRWSections(void)
{
	return {
		RW_SECTION_STRUCT,
		RW_SECTION_STRING,
		RW_SECTION_EXTENSION,
		RW_SECTION_TEXTURE,
		RW_SECTION_MATERIAL,
		RW_SECTION_MATERIAL_LIST,
		RW_SECTION_FRAME_LIST,
		RW_SECTION_GEOMETRY,
		RW_SECTION_CLUMP,
		RW_SECTION_LIGHT,
		RW_SECTION_ATOMIC,
		RW_SECTION_TEXTURE_NATIVE,
		RW_SECTION_TEXTURE_DICTIONARY,
		RW_SECTION_GEOMETRY_LIST,
		RW_SECTION_BIN_MESH_PLG,
		RW_SECTION_2D_EFFECT,
		RW_SECTION_EXTRA_VERT_COLOUR,
		RW_SECTION_FRAME
	};
}

RWSection*						RWSection::getNextParentNodeWithSectionType(ERWSection eSection)
{
	RWSection *pRWSection = this;
	do
	{
		pRWSection = pRWSection->getParentNode();
	}
	while (pRWSection && pRWSection->getSectionId() != eSection);
	return pRWSection;
}