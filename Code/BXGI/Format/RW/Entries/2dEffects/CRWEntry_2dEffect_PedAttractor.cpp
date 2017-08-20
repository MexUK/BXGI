#include "CRWEntry_2dEffect_PedAttractor.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"
#include "Static/CString2.h"

using namespace bxcf;
using namespace bxgi;

CRWEntry_2dEffect_PedAttractor::CRWEntry_2dEffect_PedAttractor(void) :
	C2dEffect(_2DFX_PED_ATTRACTOR),
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

void							CRWEntry_2dEffect_PedAttractor::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();
	
	m_iType = pDataReader->readInt32();
	m_vecRotation[0] = pDataReader->readVector3D();
	m_vecRotation[1] = pDataReader->readVector3D();
	m_vecRotation[2] = pDataReader->readVector3D();
	m_strExternalScriptName = CString2::rtrimFromLeft(pDataReader->readString(8));
	m_iPedExistingProbability = pDataReader->readInt32();
	m_ucUnknown1 = pDataReader->readUint8();
	m_ucNotUsed1 = pDataReader->readUint8();
	m_ucUnknown2 = pDataReader->readUint8();
	m_ucNotUsed2 = pDataReader->readUint8();
}

void							CRWEntry_2dEffect_PedAttractor::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();
	
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