#include "CIPLEntry_JUMP.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

CIPLEntry_JUMP::CIPLEntry_JUMP(CIPLFormat *pIPLFormat) :
	CIPLEntry_Data(pIPLFormat, IPL_SECTION_JUMP),
	m_vecStartLowerPosition{ 0.0f, 0.0f, 0.0f },
	m_vecStartUpperPosition{ 0.0f, 0.0f, 0.0f },
	m_vecTargetLowerPosition{ 0.0f, 0.0f, 0.0f },
	m_vecTargetUpperPosition{ 0.0f, 0.0f, 0.0f },
	m_vecCameraPosition{ 0.0f, 0.0f, 0.0f },
	m_uiReward(0)
{
}

void			CIPLEntry_JUMP::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();

	switch (pDataReader->getLineTokens().size())
	{
	case 16:
		setFormatType(0);
		setFormatGames(GAME_FLAG_GTA_SA);

		m_vecStartLowerPosition = pDataReader->readTokenVector3D();
		m_vecStartUpperPosition = pDataReader->readTokenVector3D();
		m_vecTargetLowerPosition = pDataReader->readTokenVector3D();
		m_vecTargetUpperPosition = pDataReader->readTokenVector3D();
		m_vecCameraPosition = pDataReader->readTokenVector3D();
		m_uiReward = pDataReader->readTokenUint32();
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}

void			CIPLEntry_JUMP::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	switch (getFormatType())
	{
	case 0:
		pDataWriter->writeToken(m_vecStartLowerPosition);
		pDataWriter->writeToken(m_vecStartUpperPosition);
		pDataWriter->writeToken(m_vecTargetLowerPosition);
		pDataWriter->writeToken(m_vecTargetUpperPosition);
		pDataWriter->writeToken(m_vecCameraPosition);
		pDataWriter->writeToken(m_uiReward);
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}