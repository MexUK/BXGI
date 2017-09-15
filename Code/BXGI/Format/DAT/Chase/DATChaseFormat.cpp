#include "DATChaseFormat.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

DATChaseFormat::DATChaseFormat(void) :
	Format(true, LITTLE_ENDIAN)
{
}

void					DATChaseFormat::unserialize(void)
{
	DataReader *pDataReader = &m_reader;
	
	/*
	todo
	uint32 uiEntryCount = getTotalStreamLength() / 28;
	vector<DATChaseEntry*> vecEntries = pDataReader->readStructMultiple<DATChaseEntry>(uiEntryCount);
	
	m_vecEntries.resize(uiEntryCount);
	for(uint32 i = 0; i < uiEntryCount; i++)
	{
		DATChaseEntry_Raw *pDATChaseEntry_Raw = vecEntries[i];
		DATChaseEntry *pDATChaseEntry = new DATChaseEntry;
		
		pDATChaseEntry->setFormat(this);
		pDATChaseEntry->setVelocity(pDATChaseEntry_Raw->m_vecVelocity);
		pDATChaseEntry->setRight(pDATChaseEntry_Raw->m_vecRight);
		pDATChaseEntry->setTop(pDATChaseEntry_Raw->m_vecTop);
		pDATChaseEntry->setSteeringAngle(pDATChaseEntry_Raw->m_iSteeringAngle);
		pDATChaseEntry->setGasPedalPower(pDATChaseEntry_Raw->m_iGasPedalPower);
		pDATChaseEntry->setBrakePedalPower(pDATChaseEntry_Raw->m_iBrakePedalPower);
		pDATChaseEntry->setHandbrakeUsed(pDATChaseEntry_Raw->m_iHandbrakeUsed);
		pDATChaseEntry->setPosition(pDATChaseEntry_Raw->m_vecPosition);
		
		setEntry(i, pDATChaseEntry);
	}
	*/
}

void					DATChaseFormat::serialize(void)
{
	DataWriter *pDataWriter = &m_writer;
	
	for(DATChaseEntry *pDATChaseEntry : getEntries())
	{
		pDATChaseEntry->serialize();
	}
}