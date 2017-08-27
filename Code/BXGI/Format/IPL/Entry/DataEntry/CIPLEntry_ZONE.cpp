#include "CIPLEntry_ZONE.h"
#include "Exception/EExceptionCode.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

CIPLEntry_ZONE::CIPLEntry_ZONE(CIPLFormat *pIPLFormat) :
	CIPLEntry_Data(pIPLFormat, IPL_SECTION_ZONE),
	m_uiZoneType(0),
	m_vecBottomLeftPosition{ 0.0f, 0.0f, 0.0f },
	m_vecTopRightPosition{ 0.0f, 0.0f, 0.0f },
	m_uiLevel(0)
{
}

void			CIPLEntry_ZONE::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();

	uint32 uiLineTokenCount = pDataReader->getLineTokens().size();
	switch (uiLineTokenCount)
	{
	case 9:
	case 10:
		setFormatType(uiLineTokenCount == 9 ? 0 : 1);
		setFormatGames(GAME_FLAG_GTA_III | GAME_FLAG_GTA_VC);

		m_strZoneName = pDataReader->readTokenString();
		m_uiZoneType = pDataReader->readTokenUint32();
		m_vecBottomLeftPosition = pDataReader->readTokenVector3D();
		m_vecTopRightPosition = pDataReader->readTokenVector3D();
		m_uiLevel = pDataReader->readTokenUint32();
		if (getFormatType() == 1)
		{
			setFormatGames(GAME_FLAG_GTA_SA | GAME_FLAG_GTA_IV);
			m_strZoneText = pDataReader->readTokenString();
		}
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}

void			CIPLEntry_ZONE::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();

	switch (getFormatType())
	{
	case 0:
	case 1:
		pDataWriter->writeToken(m_strZoneName);
		pDataWriter->writeToken(m_uiZoneType);
		pDataWriter->writeToken(m_vecBottomLeftPosition);
		pDataWriter->writeToken(m_vecTopRightPosition);
		pDataWriter->writeToken(m_uiLevel);
		if (getFormatType() == 1)
		{
			pDataWriter->writeToken(m_strZoneText);
		}
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}