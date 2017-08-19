#include "CRWEntry_2dEffect_StreetSign.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"
#include "Static/CString2.h"

using namespace bxcf;
using namespace bxgi;

CRWEntry_2dEffect_StreetSign::CRWEntry_2dEffect_StreetSign(void) :
	C2dEffect(_2DFX_STREET_SIGN),
	m_vecSize({ 0.0f, 0.0f }),
	m_vecRotation({ 0.0f, 0.0f, 0.0f }),
	m_uiFlags(0)
{
};

void							CRWEntry_2dEffect_StreetSign::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();
	
	m_vecSize = pDataReader->readVector2D();
	m_vecRotation = pDataReader->readVector3D();
	m_uiFlags = (uint16) pDataReader->readUint32();
	for (uint32 i = 0; i < 4; i++)
	{
		m_strText[i] = CString2::rtrimFromLeft(pDataReader->readString(16));
	}
}

void							CRWEntry_2dEffect_StreetSign::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();

	pDataWriter->writeVector2D(m_vecSize);
	pDataWriter->writeVector3D(m_vecRotation);
	pDataWriter->writeUint32(m_uiFlags);
	for (uint32 i = 0; i < 4; i++)
	{
		pDataWriter->writeStringRef(m_strText[i]);
	}
}