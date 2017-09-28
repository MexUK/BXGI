#include "IMGFormatVersionStories.h"
#include "Format/IMG/Regular/IMGEntry.h"
#include "Static/Math.h"
#include "Static/Path.h"
#include "Event/Events.h"
#include "../BXCF/Event/EEvent.h"
#include "../BXGI/Event/EEvent.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

IMGFormatVersionStories::IMGFormatVersionStories(void) :
	IMGFormat(IMG_STORIES)
{
}

IMGFormatVersionStories::IMGFormatVersionStories(string strFilePathOrData, bool bParam1IsFilePath) :
	IMGFormat(IMG_STORIES, strFilePathOrData, bParam1IsFilePath)
{
}

// serialization
void					IMGFormatVersionStories::_unserialize(void)
{
	IMGFormat::_unserializeBefore();

	//readWRLDFormat();
	//readGTAGFormat();
	readGameDTZ();
}

void					IMGFormatVersionStories::_serialize(void)
{
}

void					IMGFormatVersionStories::readWRLDFormat(void)
{
	/*
	todo
	// global WRLD header
	string strHeader4B = m_reader.readString(4); // DLRW
	uint32 uiWRLDType = m_reader.readUint32();
	uint32 uiWRLDSize = m_reader.readUint32();
	uint32 uiGlobalSectionStartAddress = m_reader.readUint32();
	uint32 uiGlobalSectionStartAddress2 = m_reader.readUint32();
	uint32 uiGlobalSectionOffsetCount = m_reader.readUint32();
	uint32 uiSlaveWRLDContinueAddress = m_reader.readUint32();
	uint32 uiNotUsed1 = m_reader.readUint32();

	// master WRLD header
	uint32 uiResourceIdTableAddress = m_reader.readUint32();
	for()
	{
	uint32 uiSlaveWRLDArrayAddress = m_reader.readUint32();
	uint32 uiWRLDGroupOffset = m_reader.readUint32();
	}
	uint32 uiResourceCountInLevel = m_reader.readUint32();
	string strPadding = m_reader.readStringUntilOffset(0x320);
	uint32 uiTimeObjectCountInTable = m_reader.readUint32();
	//...
	*/
}

void					IMGFormatVersionStories::readGameDTZ(void)
{
	string strDTZSignature = m_reader.readString(4);
	uint32 uiUnknown1 = m_reader.readUint32();
	uint32 uiDTZSize = m_reader.readUint32();
	uint32 uiDataSizeWithoutTablesSize = m_reader.readUint32();
	uint32 uiOffsetTable = m_reader.readUint32();
	uint32 uiOffsetTableEntryCount = m_reader.readUint32();
	uint32 uiOffsetTableHashesJenkins = m_reader.readUint32();
	int16 uiOffsetTableHashesJenkinsCountSection1 = m_reader.readUint32();
	int16 uiOffsetTableHashesJenkinsCountSection2 = m_reader.readUint32();
	uint32 uiThePathsFunction = m_reader.readUint32();
	uint32 uiBuildingsPool = m_reader.readUint32();
	uint32 uiRoadsPool = m_reader.readUint32();
	uint32 uiDynamicObjectssPool = m_reader.readUint32();
	uint32 uiEntryInfoNodes = m_reader.readUint32(); // 0x30
	uint32 uiPtrNodes = m_reader.readUint32();
	uint32 uiIDECount = m_reader.readUint32();
	uint32 uiIDETableOffset = m_reader.readUint32(); // relative to this property
	uint32 uiAutoClassTableOffset = m_reader.readUint32();
	uint32 uiAutoClassEntryCountTableOffset = m_reader.readUint32();
	uint32 uiZoneDataOffset = m_reader.readUint32();
	uint32 uiSectorsDataOffset = m_reader.readUint32();
	uint32 uiBigBuildingOffset = m_reader.readUint32();
	uint32 ui2dFXTableEntryCount = m_reader.readUint32();
	uint32 ui2dFXTableOffset = m_reader.readUint32();
	uint32 uiHardcodedModelIndicesOffset = m_reader.readUint32();
	uint32 uiTextureAchivesOffset = m_reader.readUint32();
	uint32 uiUnknown2 = m_reader.readUint32(); // 0
	uint32 uiCOLOffset = m_reader.readUint32(); // COL version 2
	uint32 uiUnknown3 = m_reader.readUint32(); // 0
	uint32 ui1COLOffset = m_reader.readUint32();
	uint32 uiAnalogObjectDATOffset = m_reader.readUint32();
	uint32 uiCarColDATColoursOffset = m_reader.readUint32();
	uint32 uiStreamingInfoForModelOffset = m_reader.readUint32();
	uint32 uiANIMOffset = m_reader.readUint32();
	uint32 uiFistFiteDATOffset = m_reader.readUint32();
	uint32 uiPedAnimInfoArrayOffset = m_reader.readUint32();
	uint32 uiPedDATOffset = m_reader.readUint32();
	uint32 uiPedStatsDATOffset = m_reader.readUint32();
	uint32 uiCULLIPLEntryCount = m_reader.readUint32();
	uint32 uiCULLIPLOffset = m_reader.readUint32();
	uint32 uiOcclusionsUnknown1 = m_reader.readUint32();
	uint32 uiOcclusionsUnknown2 = m_reader.readUint32();
	uint32 uiWaterProDATOffset = m_reader.readUint32();
	uint32 uiHandlingConfigDATOffset = m_reader.readUint32();
	uint32 uiSurfaceDAT = m_reader.readUint32();
	uint32 uiTimeCYCDATOffset = m_reader.readUint32();
	uint32 uiPedGrpDATOffset = m_reader.readUint32();
	uint32 uiParticleCFGOffset = m_reader.readUint32();
	uint32 uiWeaponDATOffset = m_reader.readUint32();
	uint32 uiFileClampsTableOffset = m_reader.readUint32();
	uint32 uiCUTSDIROffset = m_reader.readUint32();
	uint32 uiFerryDATOffset = m_reader.readUint32();
	uint32 uiTracksAndTracks2DATOffset = m_reader.readUint32();
	uint32 uiFlightDATOffset = m_reader.readUint32();
	uint32 uiMenuCHKOffset = m_reader.readUint32(); // compressed by ZLib Deflate
	uint32 uiFontsCHKSize = m_reader.readUint32();
	uint32 uiFontsCHKOffset = m_reader.readUint32();

	// IDE

}