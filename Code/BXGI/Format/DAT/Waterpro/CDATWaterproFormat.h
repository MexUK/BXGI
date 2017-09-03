#pragma once

#define MAX_WATER_LEVEL_COUNT 48

#include "nsbxgi.h"
#include "Type/Types.h"
#include "Format/Format.h"
#include "CDATWaterproEntry.h"
#include "Pool/VectorPool.h"
#include <vector>

class bxgi::CDATWaterproFormat : public bxcf::Format, public bxcf::VectorPool<bxgi::CDATWaterproEntry*>
{
public:
	CDATWaterproFormat(void);
	
	void					unserialize(void);
	void					serialize(void);
	
	void					setVisibleWaterLevelMap(std::vector<uint8>& vecVisibleWaterLevelMap) { m_vecVisibleWaterLevelMap = vecVisibleWaterLevelMap; }
	std::vector<uint8>&		getVisibleWaterLevelMap(void) { return m_vecVisibleWaterLevelMap; }
	
	void					setPhysicalWaterLevelMap(std::vector<uint8>& vecPhysicalWaterLevelMap) { m_vecPhysicalWaterLevelMap = vecPhysicalWaterLevelMap; }
	std::vector<uint8>&		getPhysicalWaterLevelMap(void) { return m_vecPhysicalWaterLevelMap; }
	
private:
	std::vector<uint8>		m_vecVisibleWaterLevelMap;
	std::vector<uint8>		m_vecPhysicalWaterLevelMap;
};