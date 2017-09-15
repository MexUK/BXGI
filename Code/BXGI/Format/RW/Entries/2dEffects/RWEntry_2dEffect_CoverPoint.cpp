#include "RWEntry_2dEffect_CoverPoint.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/RW/RWFormat.h"

using namespace bxcf;
using namespace bxgi;

RWEntry_2dEffect_CoverPoint::RWEntry_2dEffect_CoverPoint(void) :
	_2dEffect(_2DFX_COVER_POINT),
	m_vecDirection({ 0.0f, 0.0f }),
	m_uiCoverType(0)
{
}

void							RWEntry_2dEffect_CoverPoint::unserialize(void)
{
	DataReader *pDataReader = &m_pRWFormat->m_reader;
	
	m_vecDirection = pDataReader->readVector2D();
	m_uiCoverType = pDataReader->readUint32();
}

void							RWEntry_2dEffect_CoverPoint::serialize(void)
{
	DataWriter *pDataWriter = &m_pRWFormat->m_writer;
	
	pDataWriter->writeVector2D(m_vecDirection);
	pDataWriter->writeUint32(m_uiCoverType);
}