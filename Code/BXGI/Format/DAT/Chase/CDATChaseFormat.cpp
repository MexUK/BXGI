#include "CDATChaseFormat.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

CDATChaseFormat::CDATChaseFormat(void) :
	Format(true, LITTLE_ENDIAN)
{
}

void					CDATChaseFormat::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();
	
	/*
	todo
	uint32 uiEntryCount = getTotalStreamLength() / 28;
	vector<CDATChaseEntry*> vecEntries = pDataReader->readStructMultiple<CDATChaseEntry>(uiEntryCount);
	
	m_vecEntries.resize(uiEntryCount);
	for(uint32 i = 0; i < uiEntryCount; i++)
	{
		CDATChaseEntry_Raw *pDATChaseEntry_Raw = vecEntries[i];
		CDATChaseEntry *pDATChaseEntry = new CDATChaseEntry;
		
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

void					CDATChaseFormat::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();
	
	for(CDATChaseEntry *pDATChaseEntry : getEntries())
	{
		pDATChaseEntry->serialize();
	}
}