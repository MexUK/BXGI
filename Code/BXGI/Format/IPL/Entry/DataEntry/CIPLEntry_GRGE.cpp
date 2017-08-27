#include "CIPLEntry_GRGE.h"
#include "Exception/EExceptionCode.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

CIPLEntry_GRGE::CIPLEntry_GRGE(CIPLFormat *pIPLFormat) :
	CIPLEntry_Data(pIPLFormat, IPL_SECTION_GRGE),
	m_vecPosition{ 0.0f, 0.0f, 0.0f },
	m_vecLine{ 0.0f, 0.0f },
	m_vecCubePosition{ 0.0f, 0.0f, 0.0f },
	m_uiGarageFlags(0),
	m_uiGarageType(0)
{
}

void			CIPLEntry_GRGE::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();

	uint32 uiLineTokenCount = pDataReader->getLineTokens().size();
	switch (uiLineTokenCount)
	{
	case 10:
	case 11:
		setFormatType(uiLineTokenCount == 10 ? 0 : 1);
		setFormatGames(GAME_FLAG_GTA_SA);

		m_vecPosition = pDataReader->readTokenVector3D();
		m_vecLine = pDataReader->readTokenVector2D();
		m_vecCubePosition = pDataReader->readTokenVector3D();
		m_uiGarageFlags = pDataReader->readTokenUint32();
		m_uiGarageType = pDataReader->readTokenUint32();
		if (getFormatType() == 1)
		{
			m_strGarageName = pDataReader->readString();
		}
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}

void			CIPLEntry_GRGE::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();

	switch (getFormatType())
	{
	case 0:
	case 1:
		pDataWriter->writeToken(m_vecPosition);
		pDataWriter->writeToken(m_vecLine);
		pDataWriter->writeToken(m_vecCubePosition);
		pDataWriter->writeToken(m_uiGarageFlags);
		pDataWriter->writeToken(m_uiGarageType);
		if (getFormatType() == 1)
		{
			pDataWriter->writeToken(m_strGarageName);
		}
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}