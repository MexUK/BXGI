#include "CRWEntry_2dEffect_CoverPoint.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

CRWEntry_2dEffect_CoverPoint::CRWEntry_2dEffect_CoverPoint(void) :
	C2dEffect(_2DFX_COVER_POINT),
	m_vecDirection({ 0.0f, 0.0f }),
	m_uiCoverType(0)
{
}

void							CRWEntry_2dEffect_CoverPoint::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();
	
	m_vecDirection = pDataReader->readVector2D();
	m_uiCoverType = pDataReader->readUint32();
}

void							CRWEntry_2dEffect_CoverPoint::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();
	
	pDataWriter->writeVector2D(m_vecDirection);
	pDataWriter->writeUint32(m_uiCoverType);
}