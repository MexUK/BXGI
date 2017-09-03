#include "RWEntry_2dEffect_StreetSign.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Static/String2.h"

using namespace bxcf;
using namespace bxgi;

RWEntry_2dEffect_StreetSign::RWEntry_2dEffect_StreetSign(void) :
	_2dEffect(_2DFX_STREET_SIGN),
	m_vecSize({ 0.0f, 0.0f }),
	m_vecRotation({ 0.0f, 0.0f, 0.0f }),
	m_uiFlags(0)
{
};

void							RWEntry_2dEffect_StreetSign::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();
	
	m_vecSize = pDataReader->readVector2D();
	m_vecRotation = pDataReader->readVector3D();
	m_uiFlags = (uint16) pDataReader->readUint32();
	for (uint32 i = 0; i < 4; i++)
	{
		m_strText[i] = String2::rtrimFromLeft(pDataReader->readString(16));
	}
}

void							RWEntry_2dEffect_StreetSign::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	pDataWriter->writeVector2D(m_vecSize);
	pDataWriter->writeVector3D(m_vecRotation);
	pDataWriter->writeUint32(m_uiFlags);
	for (uint32 i = 0; i < 4; i++)
	{
		pDataWriter->writeStringRef(m_strText[i]);
	}
}