#include "CDATChaseEntry.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

CDATChaseEntry::CDATChaseEntry(void) :
	m_pFormat(nullptr),
	m_iSteeringAngle(0),
	m_iGasPedalPower(0),
	m_iBrakePedalPower(0),
	m_iHandbrakeUsed(0)
{
	m_vecVelocity.x = 0;
	m_vecVelocity.y = 0;
	m_vecVelocity.z = 0;
	m_vecRight.x = 0;
	m_vecRight.y = 0;
	m_vecRight.z = 0;
	m_vecTop.x = 0;
	m_vecTop.y = 0;
	m_vecTop.z = 0;
	m_vecPosition.x = 0.0f;
	m_vecPosition.y = 0.0f;
	m_vecPosition.z = 0.0f;
}

void					CDATChaseEntry::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();
	
	/*
	todo
	pDataWriter->write(pDATChaseEntry_Raw->m_vecVelocity);
	pDataWriter->write(pDATChaseEntry_Raw->m_vecRight);
	pDataWriter->write(pDATChaseEntry_Raw->m_vecTop);
	pDataWriter->write(pDATChaseEntry_Raw->m_iSteeringAngle);
	pDataWriter->write(pDATChaseEntry_Raw->m_iGasPedalPower);
	pDataWriter->write(pDATChaseEntry_Raw->m_iBrakePedalPower);
	pDataWriter->write(pDATChaseEntry_Raw->m_iHandbrakeUsed);
	pDataWriter->write(pDATChaseEntry_Raw->m_vecPosition);
	*/
}