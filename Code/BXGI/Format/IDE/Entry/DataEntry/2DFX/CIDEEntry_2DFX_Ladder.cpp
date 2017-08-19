#include "CIDEEntry_2DFX_Ladder.h"
#include "Exception/eExceptionCode.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

void			CIDEEntry_2DFX_Ladder::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();
	
	CIDEEntry_2DFX::unserialize();
	
	m_vecLadderStartPosition = pDataReader->readVector3D();
	m_vecLadderEndPosition = pDataReader->readVector3D();
	m_vecClimbDirectionPosition = pDataReader->readVector3D();
	m_uiFlags = pDataReader->readTokenUint32();
}


void			CIDEEntry_2DFX_Ladder::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();

	CIDEEntry_2DFX::serialize();

	pDataWriter->writeToken(m_vecLadderStartPosition);
	pDataWriter->writeToken(m_vecLadderEndPosition);
	pDataWriter->writeToken(m_vecClimbDirectionPosition);
	pDataWriter->writeToken(m_uiFlags);
}