#include "IPLEntry_AUZO.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/IPL/IPLFormat.h"

using namespace bxcf;
using namespace bxgi;

IPLEntry_AUZO::IPLEntry_AUZO(IPLFormat *pIPLFormat) :
	IPLEntry_Data(pIPLFormat, IPL_SECTION_AUZO),
	m_uiSoundId(0),
	m_uiSwitch(0),
	m_vecLowerLeftPosition{ 0.0f, 0.0f, 0.0f },
	m_vecUpperRightPosition{ 0.0f, 0.0f, 0.0f },
	m_vecCenterPosition{ 0.0f, 0.0f, 0.0f },
	m_fRadius(0.0f)
{
}

void			IPLEntry_AUZO::unserialize(void)
{
	DataReader *pDataReader = &m_pFormat->m_reader;

	switch (pDataReader->getLineTokens().size())
	{
	case 9:
		setFormatType(0);
		setFormatGames(GAME_FLAG_GTA_SA);

		m_strZoneName = pDataReader->readTokenString();
		m_uiSoundId = pDataReader->readTokenUint32();
		m_uiSwitch = pDataReader->readTokenUint32();
		m_vecLowerLeftPosition = pDataReader->readTokenVector3D();
		m_vecUpperRightPosition = pDataReader->readTokenVector3D();
		break;
	case 7:
		setFormatType(1);
		setFormatGames(GAME_FLAG_GTA_SA);

		m_strZoneName = pDataReader->readTokenString();
		m_uiSoundId = pDataReader->readTokenUint32();
		m_uiSwitch = pDataReader->readTokenUint32();
		m_vecCenterPosition = pDataReader->readTokenVector3D();
		m_fRadius = pDataReader->readTokenFloat32();
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}

void			IPLEntry_AUZO::serialize(void)
{
	DataWriter *pDataWriter = &m_pFormat->m_writer;

	switch (getFormatType())
	{
	case 0:
		pDataWriter->writeToken(m_strZoneName);
		pDataWriter->writeToken(m_uiSoundId);
		pDataWriter->writeToken(m_uiSwitch);
		pDataWriter->writeToken(m_vecLowerLeftPosition);
		pDataWriter->writeToken(m_vecUpperRightPosition);
		break;
	case 1:
		pDataWriter->writeToken(m_strZoneName);
		pDataWriter->writeToken(m_uiSoundId);
		pDataWriter->writeToken(m_uiSwitch);
		pDataWriter->writeToken(m_vecLowerLeftPosition);
		pDataWriter->writeToken(m_fRadius);
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}