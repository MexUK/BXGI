#include "RWSectionContainer.h"
#include "Format/RW/RWSection.h"
#include "Engine/RW/RWManager.h"
#include "Sections/RWSection_Geometry.h"
#include "Sections/RWSection_ExtraVertColour.h"
#include "Sections/RWSection_Extension.h"
#include "Sections/RWSection_2dEffect.h"
#include "Static/String.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Static/Debug.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

RWSectionContainer::RWSectionContainer(void) :
	m_pParentNode(nullptr)
{
}

void				RWSectionContainer::unload(void)
{
	for (RWSection *pRWSection : getEntries())
	{
		pRWSection->unload();
		delete pRWSection;
	}
}

void				RWSectionContainer::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();

	RWSectionContainer *pParentRWSection = this;
	RWSectionContainer *pPreviousRWSection = this;
	uint32 uiSectionDepth = 0;
	vector<uint32>
		vecSectionSizes,
		vecByteCountsRead;
	bool bParentRWSectionChanged = false;
	while (!pDataReader->isEOF())
	{
		// read section header
		uint32
			uiRWSectionId = pDataReader->readUint32(),
			uiRWSectionSize = pDataReader->readUint32(),
			uiRWSectionRWVersion = pDataReader->readUint32();
		ERWSection ERWSectionValue = (ERWSection)uiRWSectionId;
		if (vecByteCountsRead.size() > 0)
		{
			// todo vecByteCountsRead[vecByteCountsRead.size() - 1] += 12;

			for (unsigned long i = 0; i < uiSectionDepth; i++)
			{
				vecByteCountsRead[i] += 12;
			}
		}

		// enter new section
		if (uiRWSectionId == 0)
		{
			// assuming end of data
			break;
		}
		else if (uiRWSectionId != 1)
		{
			vecSectionSizes.push_back(uiRWSectionSize);
			vecByteCountsRead.push_back(0);
			uiSectionDepth++;
			if (!bParentRWSectionChanged)
			{
				pParentRWSection = pPreviousRWSection;
			}
		}

		// read struct header
		uint32 uiStructSectionSize = 0;
		if (RWSection::doesRWSectionContainStruct(ERWSectionValue))
		{
			uint32
				uiRWSectionId2 = pDataReader->readUint32(),
				uiRWSectionSize2 = pDataReader->readUint32(),
				uiRWSectionRWVersion2 = pDataReader->readUint32();
			uiStructSectionSize = uiRWSectionSize2;
			// todo vecByteCountsRead[vecByteCountsRead.size() - 1] += 12;

			for (unsigned long i = 0; i < uiSectionDepth; i++)
			{
				vecByteCountsRead[i] += 12;
			}
		}

		// create RW section
		RWSection *pRWSection = RWSection::creatERWSection(ERWSectionValue);
		/*
		todo
		if (!pRWSection->isUnknownSection())
		{
			pRWSection->setSectionEnumId(ERWSectionValue);
		}
		*/
		pRWSection->setSectionId(ERWSectionValue);
		pRWSection->setSectionSize(uiRWSectionSize);
		pRWSection->setStructSectionSize(uiStructSectionSize);
		pRWSection->setSectionRWVersion(uiRWSectionRWVersion);
		if (pParentRWSection != this)
		{
			pRWSection->setParentNode((RWSection*)pParentRWSection);
		}

		// unserialize RW section
		Debug::log("Section ID: " + String::toString(uiRWSectionId));
		uint64 uiByteCountBefore = pDataReader->getSeek();
		pRWSection->unserialize();
		uint64 uiByteCountAfter = pDataReader->getSeek();
		
		uint32 uiByteCountDifference = (uint32)(uiByteCountAfter - uiByteCountBefore);
		for (unsigned long i = 0; i < uiSectionDepth; i++)
		{
			vecByteCountsRead[i] += uiByteCountDifference;
		}

		// store RW section
		pParentRWSection->addEntry(pRWSection);

		// reset section depth
		/*
		if (pParentRWSection->getSectionCountByType(RW_SECTION_EXTENSION) == 31) // temp
		{
			int a = 1;
		}
		*/

		bool bParentRWSectionChanged2 = false;
		RWSectionContainer *pRWSection2 = pRWSection;
		for (int32 iSectionIndex = (vecByteCountsRead.size() - 1), j = 0; iSectionIndex >= j; iSectionIndex--)
		{
			if (vecByteCountsRead[iSectionIndex] >= vecSectionSizes[iSectionIndex])
			{
				pRWSection2 = pRWSection2->getParentNode();// pPreviousRWSection;// pParentRWSectionContainer->getParentNode();
				pParentRWSection = pRWSection2;
				vecSectionSizes.pop_back();
				//vecByteCountsRead[uiSectionDepth - 2] += vecByteCountsRead[uiSectionDepth - 1];
				vecByteCountsRead.pop_back();
				uiSectionDepth--;
				bParentRWSectionChanged2 = true;
			}
			else
			{
				//pParentRWSection = pRWSection;
				break;
			}
		}
		bParentRWSectionChanged = bParentRWSectionChanged2;

		pPreviousRWSection = pRWSection;
	}
}

void				RWSectionContainer::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();
	EDataStreamType ePreviousStreamType = pDataWriter->getStreamType();

	pDataWriter->setStreamType(DATA_STREAM_MEMORY);
	serializERWSectionContainer(this);
	string strData = pDataWriter->getData();

	pDataWriter->setStreamType(ePreviousStreamType);
	pDataWriter->setSeek(0);
	pDataWriter->write(strData);
}

void				RWSectionContainer::serializERWSectionContainer(RWSectionContainer *pRWSectionContainer)
{
	DataWriter *pDataWriter = DataWriter::get();
	for (RWSection *pRWSection : pRWSectionContainer->getEntries())
	{
		uint64 uiSeek1 = pDataWriter->getSeek();
		pRWSection->RWSection::serialize();	// base method,              RW section header
		uint64 uiSeek2 = pDataWriter->getSeek();
		pRWSection->serialize();				// derived method (virtual), RW section body
		uint64 uiSeek3 = pDataWriter->getSeek();

		if (!pRWSection->isUnknownSection())
		{
			serializERWSectionContainer(pRWSection);
		}
		uint64 uiSeek4 = pDataWriter->getSeek();
		
		uint64 uiSectionByteCount = uiSeek4 - uiSeek1;
		uint64 uiSectionStructByteCount = uiSeek3 - uiSeek2;

		if (!pRWSection->isSectionHeaderSkipped())
		{
			uiSectionByteCount -= 12;
		}

		pRWSection->fillPlaceholdersForSerialization((uint32)uiSectionByteCount, (uint32)uiSectionStructByteCount);
	}
}

RWSection*			RWSectionContainer::addSection(ERWSection ERWSectionValue, ERWVersion ERWVersionValue)
{
	RWSection *pRWSection = RWSection::creatERWSection(ERWSectionValue);
	if (pRWSection == nullptr)
	{
		return nullptr;
	}
	pRWSection->setParentNode(nullptr);
	pRWSection->setSectionId(ERWSectionValue);
	pRWSection->setSectionSize(0);
	pRWSection->setSectionRWVersion(ERWVersionValue);
	addEntry(pRWSection);
	return pRWSection;
}

void				RWSectionContainer::removeSectionByIndex(uint32 uiSectionIndex)
{
	RWSection *pRWSection = m_vecEntries[uiSectionIndex];
	auto it = std::find(m_vecEntries.begin(), m_vecEntries.end(), pRWSection);
	if (it != m_vecEntries.end())
	{
		m_vecEntries.erase(it);
	}
	pRWSection->unload();
	delete pRWSection;
}

vector<RWSection*>	RWSectionContainer::getSectionsByType(ERWSection ERWSectionValue, bool bCheckRecursiveSections)
{
	vector<RWSection*>
		vecRWSectionsToSearch = getEntries(),
		vecRWSectionsToReturn;
	for (uint32 i = 0; i < vecRWSectionsToSearch.size(); i++)
	{
		RWSection *pRWSection = vecRWSectionsToSearch[i];
		if (bCheckRecursiveSections)
		{
			for (RWSection *pRWSection2 : pRWSection->getEntries())
			{
				vecRWSectionsToSearch.push_back(pRWSection2);
			}
		}

		if (pRWSection->getSectionId() == ERWSectionValue)
		{
			vecRWSectionsToReturn.push_back(pRWSection);
		}
	}
	return vecRWSectionsToReturn;
}

uint32				RWSectionContainer::getSectionCountByType(ERWSection ERWSectionValue, bool bCheckRecursiveSections)
{
	vector<RWSection*> vecRWSectionsToSearch = getEntries();
	uint32 uiSectionCount = 0;
	for (uint32 i = 0; i < vecRWSectionsToSearch.size(); i++)
	{
		RWSection *pRWSection = vecRWSectionsToSearch[i];
		if (bCheckRecursiveSections)
		{
			for (RWSection *pRWSection2 : pRWSection->getEntries())
			{
				vecRWSectionsToSearch.push_back(pRWSection2);
			}
		}

		if (pRWSection->getSectionId() == ERWSectionValue)
		{
			uiSectionCount++;
		}
	}
	return uiSectionCount;
}

void				RWSectionContainer::removePrelightning(void)
{
	if (((RWSection*)this)->getSectionId() == RW_SECTION_GEOMETRY)
	{
		((RWSection_Geometry*)this)->removePrelightning();
	}

	for (auto pRWSection : getEntries())
	{
		pRWSection->removePrelightning();
	}
}

void				RWSectionContainer::setPrelightningColour(int16 ssRed, int16 ssGreen, int16 ssBlue, int16 ssAlpha)
{
	Vec4u8 vecColour; // todo - add Colour4uc : Vec4u8
	vecColour.x = (uint8) ssRed;
	vecColour.y = (uint8) ssGreen;
	vecColour.z = (uint8) ssBlue;
	vecColour.w = (uint8) ssAlpha;

	vector<Vec4u8> vecColours = { vecColour }; // todo - make setVertexColours take like COLOUR_MODE_COPY or COLOUR_MODE_ZERO - so that setVertexColours doesn't just apply new vertex colour to 1 vertex.

	if (((RWSection*)this)->getSectionId() == RW_SECTION_GEOMETRY)
	{
		RWSection_Geometry *pRWSection_Geometry = (RWSection_Geometry*)this;
		pRWSection_Geometry->setVertexColours(vecColours);
	}

	for (auto pRWSection : getEntries())
	{
		pRWSection->setPrelightningColour(ssRed, ssGreen, ssBlue, ssAlpha);
	}
}

void				RWSectionContainer::applyPrelightningColourOffset(int16 ssRed, int16 ssGreen, int16 ssBlue, int16 ssAlpha)
{
	if (((RWSection*)this)->getSectionId() == RW_SECTION_GEOMETRY)
	{
		RWSection_Geometry *pRWSection_Geometry = (RWSection_Geometry*)this;
		string strNewVertexColours;
		for (uint32 i = 0, j = pRWSection_Geometry->getVertexCount(); i < j; i++)
		{
			Vec4u8 vecColour = pRWSection_Geometry->getVertexColours()[i];
			vecColour.x += ssRed;
			vecColour.y += ssGreen;
			vecColour.z += ssBlue;
			vecColour.w += ssAlpha;
			pRWSection_Geometry->getVertexColours()[i] = vecColour;
		}
	}

	for (auto pRWSection : getEntries())
	{
		pRWSection->applyPrelightningColourOffset(ssRed, ssGreen, ssBlue, ssAlpha);
	}
}

vector<Vec4u8>		RWSectionContainer::getDVColours(void)
{
	vector<Vec4u8> vecDVColours;
	for (RWSection *pRWSection : getSectionsByType(RW_SECTION_GEOMETRY))
	{
		RWSection_Geometry *pRWSection_Geometry = (RWSection_Geometry*)pRWSection;
		vecDVColours = pRWSection_Geometry->getVertexColours();
		break;
	}
	return vecDVColours;
}
vector<Vec4u8>		RWSectionContainer::getNVColours(void)
{
	vector<Vec4u8> vecNVColours;
	for (RWSection *pRWSection : getSectionsByType(RW_SECTION_EXTRA_VERT_COLOUR))
	{
		RWSection_ExtraVertColour *pRWSection_ExtraVertColour = (RWSection_ExtraVertColour*)pRWSection;
		vecNVColours = pRWSection_ExtraVertColour->getVertexColours();
		break;
	}
	return vecNVColours;
}

void					RWSectionContainer::setDVColours(vector<Vec4u8>& vecDVColours)
{
	for (RWSection *pRWSection : getSectionsByType(RW_SECTION_GEOMETRY))
	{
		RWSection_Geometry *pRWSection_Geometry = (RWSection_Geometry*)pRWSection;
		pRWSection_Geometry->setVertexColours(vecDVColours);
	}
}
void					RWSectionContainer::setNVColours(vector<Vec4u8>& vecNVColours)
{
	for (RWSection *pRWSection1 : getSectionsByType(RW_SECTION_GEOMETRY))
	{
		vector<RWSection*> vecNVCSections = pRWSection1->getSectionsByType(RW_SECTION_EXTRA_VERT_COLOUR);
		if (vecNVCSections.size() == 0)
		{
			vector<RWSection*> vecRWSections_Extension = pRWSection1->getSectionsByType(RW_SECTION_EXTENSION);
			RWSection_Extension *pRWSection_Extension;
			if (vecRWSections_Extension.size() == 0)
			{
				pRWSection_Extension = (RWSection_Extension*)pRWSection1->addSection(RW_SECTION_EXTENSION, RW_3_4_0_3); // todo - don't use hardcoded RW version
			}
			else
			{
				pRWSection_Extension = (RWSection_Extension*)vecRWSections_Extension[0];
			}
			RWSection_ExtraVertColour *pRWSection_ExtraVertColour = (RWSection_ExtraVertColour*)pRWSection_Extension->addSection(RW_SECTION_EXTRA_VERT_COLOUR, RW_3_4_0_3); // todo - don't use hardcoded RW version
			pRWSection_ExtraVertColour->setVertexColours(vecNVColours);
		}
		else
		{
			for (RWSection *pRWSection2 : vecNVCSections)
			{
				RWSection_ExtraVertColour *pRWSection_ExtraVertColour = (RWSection_ExtraVertColour*)pRWSection2;
				pRWSection_ExtraVertColour->setVertexColours(vecNVColours);
			}
		}
	}
}

void					RWSectionContainer::set2dEffects(vector<_2dEffect*>& vec2dEffects)
{
	vector<vector<_2dEffect*>> vec2dEffects2;
	for (RWSection *pRWSection1 : getSectionsByType(RW_SECTION_GEOMETRY))
	{
		vec2dEffects2.push_back(vec2dEffects);
	}
	set2dEffects(vec2dEffects2);
}
void					RWSectionContainer::set2dEffects(vector<vector<_2dEffect*>>& vec2dEffects)
{
	if (vec2dEffects.size() == 0)
	{
		return;
	}

	uint32 ui2dEffectsSectionOccurrenceIndex = 0;
	for (RWSection *pRWSection1 : getSectionsByType(RW_SECTION_GEOMETRY))
	{
		vector<RWSection*> vecRWSections_2dEffects = pRWSection1->getSectionsByType(RW_SECTION_2D_EFFECT);
		if (vecRWSections_2dEffects.size() == 0)
		{
			vector<RWSection*> vecRWSections_Extension = pRWSection1->getSectionsByType(RW_SECTION_EXTENSION);
			RWSection_Extension *pRWSection_Extension;
			if (vecRWSections_Extension.size() == 0)
			{
				pRWSection_Extension = (RWSection_Extension*)pRWSection1->addSection(RW_SECTION_EXTENSION, RW_3_4_0_3); // todo - don't use hardcoded RW version
			}
			else
			{
				pRWSection_Extension = (RWSection_Extension*)vecRWSections_Extension[0];
			}
			RWSection_2dEffect *pRWSection_2dEffects = (RWSection_2dEffect*)pRWSection_Extension->addSection(RW_SECTION_2D_EFFECT, RW_3_4_0_3); // todo - don't use hardcoded RW version

			for (auto p2dEffect : (ui2dEffectsSectionOccurrenceIndex < vec2dEffects.size() ? vec2dEffects[ui2dEffectsSectionOccurrenceIndex] : vec2dEffects[0]))
			{
				pRWSection_2dEffects->get2dEffects()->addEntry(p2dEffect);
			}
			ui2dEffectsSectionOccurrenceIndex++;
		}
		else
		{
			for (RWSection *pRWSection2 : vecRWSections_2dEffects)
			{
				RWSection_2dEffect *pRWSection_2dEffects = (RWSection_2dEffect*)pRWSection2;
				pRWSection_2dEffects->removeAllEntries();
				for (auto p2dEffect : (ui2dEffectsSectionOccurrenceIndex < vec2dEffects.size() ? vec2dEffects[ui2dEffectsSectionOccurrenceIndex] : vec2dEffects[0]))
				{
					pRWSection_2dEffects->get2dEffects()->addEntry(p2dEffect);
				}
				ui2dEffectsSectionOccurrenceIndex++;
			}
		}
	}
}

vector<vector<_2dEffect*>>	RWSectionContainer::get2dEffects(void)
{
	vector<RWSection_2dEffect*> vecRWSections_2dEffect = (vector<RWSection_2dEffect*>&) getSectionsByType(RW_SECTION_2D_EFFECT);
	vector<vector<_2dEffect*>> vec2dEffects;
	for (RWSection_2dEffect* pRWSection_2dEffect : vecRWSections_2dEffect)
	{
		vec2dEffects.push_back(pRWSection_2dEffect->get2dEffects()->getEntries());
	}
	return vec2dEffects;
}