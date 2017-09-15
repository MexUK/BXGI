#include "IPLEntry_GRGE.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/IPL/IPLFormat.h"

using namespace bxcf;
using namespace bxgi;

IPLEntry_GRGE::IPLEntry_GRGE(IPLFormat *pIPLFormat) :
	IPLEntry_Data(pIPLFormat, IPL_SECTION_GRGE),
	m_vecPosition{ 0.0f, 0.0f, 0.0f },
	m_vecLine{ 0.0f, 0.0f },
	m_vecCubePosition{ 0.0f, 0.0f, 0.0f },
	m_uiGarageFlags(0),
	m_uiGarageType(0)
{
}

void			IPLEntry_GRGE::unserialize(void)
{
	DataReader *pDataReader = &m_pFormat->m_reader;

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

void			IPLEntry_GRGE::serialize(void)
{
	DataWriter *pDataWriter = &m_pFormat->m_writer;

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