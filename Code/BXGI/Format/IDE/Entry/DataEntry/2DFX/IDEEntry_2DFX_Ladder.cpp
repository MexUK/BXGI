#include "IDEEntry_2DFX_Ladder.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/IDE/IDEFormat.h"

using namespace bxcf;
using namespace bxgi;

IDEEntry_2DFX_Ladder::IDEEntry_2DFX_Ladder(IDEFormat *pIDEFormat) :
	IDEEntry_2DFX(pIDEFormat, _2DFX_LADDER),
	m_vecLadderStartPosition{ 0.0f, 0.0f, 0.0f },
	m_vecLadderEndPosition{ 0.0f, 0.0f, 0.0f },
	m_vecClimbDirectionPosition{ 0.0f, 0.0f, 0.0f },
	m_uiFlags(0)
{
}

void			IDEEntry_2DFX_Ladder::unserialize(void)
{
	DataReader *pDataReader = &m_pFormat->m_reader;
	
	IDEEntry_2DFX::unserialize();
	
	m_vecLadderStartPosition = pDataReader->readVector3D();
	m_vecLadderEndPosition = pDataReader->readVector3D();
	m_vecClimbDirectionPosition = pDataReader->readVector3D();
	m_uiFlags = pDataReader->readTokenUint32();
}


void			IDEEntry_2DFX_Ladder::serialize(void)
{
	DataWriter *pDataWriter = &m_pFormat->m_writer;

	IDEEntry_2DFX::serialize();

	pDataWriter->writeToken(m_vecLadderStartPosition);
	pDataWriter->writeToken(m_vecLadderEndPosition);
	pDataWriter->writeToken(m_vecClimbDirectionPosition);
	pDataWriter->writeToken(m_uiFlags);
}