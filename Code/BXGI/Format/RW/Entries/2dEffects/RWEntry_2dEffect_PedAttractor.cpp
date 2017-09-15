#include "RWEntry_2dEffect_PedAttractor.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Static/String.h"
#include "Format/RW/RWFormat.h"

using namespace bxcf;
using namespace bxgi;

RWEntry_2dEffect_PedAttractor::RWEntry_2dEffect_PedAttractor(void) :
	_2dEffect(_2DFX_PED_ATTRACTOR),
	m_iType(0),
	m_iPedExistingProbability(0),
	m_ucUnknown1(0),
	m_ucNotUsed1(0),
	m_ucUnknown2(0),
	m_ucNotUsed2(0)
{
	m_vecRotation[0] = { 0.0f, 0.0f, 0.0f };
	m_vecRotation[1] = { 0.0f, 0.0f, 0.0f };
	m_vecRotation[2] = { 0.0f, 0.0f, 0.0f };
};

void							RWEntry_2dEffect_PedAttractor::unserialize(void)
{
	DataReader *pDataReader = &m_pRWFormat->m_reader;
	
	m_iType = pDataReader->readInt32();
	m_vecRotation[0] = pDataReader->readVector3D();
	m_vecRotation[1] = pDataReader->readVector3D();
	m_vecRotation[2] = pDataReader->readVector3D();
	m_strExternalScriptName = String::rtrimFromLeft(pDataReader->readString(8));
	m_iPedExistingProbability = pDataReader->readInt32();
	m_ucUnknown1 = pDataReader->readUint8();
	m_ucNotUsed1 = pDataReader->readUint8();
	m_ucUnknown2 = pDataReader->readUint8();
	m_ucNotUsed2 = pDataReader->readUint8();
}

void							RWEntry_2dEffect_PedAttractor::serialize(void)
{
	DataWriter *pDataWriter = &m_pRWFormat->m_writer;
	
	pDataWriter->writeInt32(m_iType);
	pDataWriter->writeVector3D(m_vecRotation[0]);
	pDataWriter->writeVector3D(m_vecRotation[1]);
	pDataWriter->writeVector3D(m_vecRotation[2]);
	pDataWriter->writeStringRef(m_strExternalScriptName, 8);
	pDataWriter->writeInt32(m_iPedExistingProbability);
	pDataWriter->writeUint8(m_ucUnknown1);
	pDataWriter->writeUint8(m_ucNotUsed1);
	pDataWriter->writeUint8(m_ucUnknown2);
	pDataWriter->writeUint8(m_ucNotUsed2);
}