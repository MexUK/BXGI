#include "CIDEEntry_2DFX_Unknown1.h"
#include "Exception/eExceptionCode.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

void			CIDEEntry_2DFX_Unknown1::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();
	
	CIDEEntry_2DFX::unserialize();
	
	m_uiUnknown2 = pDataReader->readTokenUint32();
	m_vecUnknown3 = pDataReader->readVector3D();
	m_uiUnknown4 = pDataReader->readTokenUint32();
}

void			CIDEEntry_2DFX_Unknown1::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();

	CIDEEntry_2DFX::serialize();

	pDataWriter->writeToken(m_uiUnknown2);
	pDataWriter->writeToken(m_vecUnknown3);
	pDataWriter->writeToken(m_uiUnknown4);
}