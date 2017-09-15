#include "DATWaterproFormat.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

DATWaterproFormat::DATWaterproFormat(void) :
	Format(true, LITTLE_ENDIAN)
{
}

void					DATWaterproFormat::unserialize(void)
{
	DataReader *pDataReader = &m_reader;
	
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
		DATWaterproEntry *pDATWaterproEntry = new DATWaterproEntry;
		pDATWaterproEntry->setFormat(this);
		
		pDATWaterproEntry->setWaterLevel(vecWaterHeights[i]);
		pDATWaterproEntry->setWaterZoneStart(vecWaterZones[i] * 2);
		pDATWaterproEntry->setWaterZoneEnd((vecWaterZones[i] * 2) + 1);
		
		setEntryByIndex(i, pDATWaterproEntry);
	}
	*/
}

void					DATWaterproFormat::serialize(void)
{
	DataWriter *pDataWriter = &m_writer;
	//uint32 i;
	
	/*
	todo

	uint32 uiWaterLevelCount = getEntryCount();
	pDataWriter->writeUint32(uiWaterLevelCount);
	
	// write water heights
	pDataWriter->writeStdVectorFloat32(forEachEntry<float32>([](DATWaterproEntry *pDATWaterproEntry) -> float32 { return pDATWaterproEntry->getWaterHeight(); }));
	pDataWriter->writeString((MAX_WATER_LEVEL_COUNT - getEntryCount()) * 4);
	
	// write water zone start and end positions
	pDataWriter->writeStdVectorVector4D(forEachEntry<Vec4f>([](DATWaterproEntry *pDATWaterproEntry) -> Vec4f {
		return Vec4f(pDATWaterproEntry->getWaterZoneStart(), pDATWaterproEntry->getWaterZoneEnd());
	}));
	pDataWriter->writeString((MAX_WATER_LEVEL_COUNT - getEntryCount()) * 16);
	
	// write visible water level map
	pDataWriter->writeStdVectorUint8(getVisibleWaterLevelMap());
	
	// write physical water level map
	pDataWriter->writeStdVectorUint8(getPhysicalWaterLevelMap());
	*/
}