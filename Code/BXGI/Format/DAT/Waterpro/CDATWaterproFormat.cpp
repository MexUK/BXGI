#include "CDATWaterproFormat.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

CDATWaterproFormat::CDATWaterproFormat(void) :
	CFormat(true, LITTLE_ENDIAN)
{
}

void					CDATWaterproFormat::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();
	
	/*
	todo

	uint32 uiWaterLevelCount = pDataReader->readUint32();
	
	vector<float32> vecWaterHeights = pDataReader->readStdVectorFloat32(uiWaterLevelCount);
	pDataReader->increaseSeek((MAX_WATER_LEVEL_COUNT - uiWaterLevelCount) * 4);
	
	vector<Vec2f> vecWaterZones = pDataReader->readStdVectorVector2D(uiWaterLevelCount * 2);
	pDataReader->increaseSeek((MAX_WATER_LEVEL_COUNT - uiWaterLevelCount) * 16);
	
	vector<uint8> vecVisibleWaterLevelMap = pDataReader->readStdVectorUint8(64 * 64);
	setVisibleWaterLevelMap(vecVisibleWaterLevelMap);
	
	vector<uint8> vecPhysicalWaterLevelMap = pDataReader->readStdVectorUint8(128 * 128);
	setPhysicalWaterLevelMap(vecPhysicalWaterLevelMap);
	
	m_vecEntries.resize(uiWaterLevelCount);
	for(uint32 i = 0; i < uiWaterLevelCount; i++)
	{
		CDATWaterproEntry *pDATWaterproEntry = new CDATWaterproEntry;
		pDATWaterproEntry->setFormat(this);
		
		pDATWaterproEntry->setWaterLevel(vecWaterHeights[i]);
		pDATWaterproEntry->setWaterZoneStart(vecWaterZones[i] * 2);
		pDATWaterproEntry->setWaterZoneEnd((vecWaterZones[i] * 2) + 1);
		
		setEntryByIndex(i, pDATWaterproEntry);
	}
	*/
}

void					CDATWaterproFormat::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();
	//uint32 i;
	
	/*
	todo

	uint32 uiWaterLevelCount = getEntryCount();
	pDataWriter->writeUint32(uiWaterLevelCount);
	
	// write water heights
	pDataWriter->writeStdVectorFloat32(forEachEntry<float32>([](CDATWaterproEntry *pDATWaterproEntry) -> float32 { return pDATWaterproEntry->getWaterHeight(); }));
	pDataWriter->writeString((MAX_WATER_LEVEL_COUNT - getEntryCount()) * 4);
	
	// write water zone start and end positions
	pDataWriter->writeStdVectorVector4D(forEachEntry<Vec4f>([](CDATWaterproEntry *pDATWaterproEntry) -> Vec4f {
		return Vec4f(pDATWaterproEntry->getWaterZoneStart(), pDATWaterproEntry->getWaterZoneEnd());
	}));
	pDataWriter->writeString((MAX_WATER_LEVEL_COUNT - getEntryCount()) * 16);
	
	// write visible water level map
	pDataWriter->writeStdVectorUint8(getVisibleWaterLevelMap());
	
	// write physical water level map
	pDataWriter->writeStdVectorUint8(getPhysicalWaterLevelMap());
	*/
}