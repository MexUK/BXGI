#include "IPLEntry_ENEX.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

IPLEntry_ENEX::IPLEntry_ENEX(IPLFormat *pIPLFormat) :
	IPLEntry_Data(pIPLFormat, IPL_SECTION_ENEX),
	m_vecEntrancePosition{ 0.0f, 0.0f, 0.0f },
	m_fRotation(0.0f),
	m_vecSize{ 0.0f, 0.0f },
	m_uiConstant8(0),
	m_vecExitPosition{ 0.0f, 0.0f, 0.0f },
	m_fExitRotation(0.0f),
	m_uiTargetInterior(0),
	m_uiMarkerType(0),
	m_uiSkyColourChanger(0),
	m_uiUnknown1(0),
	m_uiTimeOn(0),
	m_uiTimeOff(0)
{
}

void			IPLEntry_ENEX::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();

	switch (pDataReader->getLineTokens().size())
	{
	case 18:
		setFormatType(0);
		setFormatGames(GAME_FLAG_GTA_SA);

		m_vecEntrancePosition = pDataReader->readTokenVector3D();
		m_fRotation = pDataReader->readTokenFloat32();
		m_vecSize = pDataReader->readTokenVector2D();
		m_uiConstant8 = pDataReader->readTokenUint32();
		m_vecExitPosition = pDataReader->readTokenVector3D();
		m_fExitRotation = pDataReader->readTokenFloat32();
		m_uiTargetInterior = pDataReader->readTokenUint32();
		m_uiMarkerType = pDataReader->readTokenUint32();
		m_strInteriorName = pDataReader->readTokenString();
		m_uiSkyColourChanger = pDataReader->readTokenUint32();
		m_uiUnknown1 = pDataReader->readTokenUint32();
		m_uiTimeOn = pDataReader->readTokenUint32();
		m_uiTimeOff = pDataReader->readTokenUint32();
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}

void			IPLEntry_ENEX::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	switch (getFormatType())
	{
	case 0:
		pDataWriter->writeToken(m_vecEntrancePosition);
		pDataWriter->writeToken(m_fRotation);
		pDataWriter->writeToken(m_vecSize);
		pDataWriter->writeToken(m_uiConstant8);
		pDataWriter->writeToken(m_vecExitPosition);
		pDataWriter->writeToken(m_fExitRotation);
		pDataWriter->writeToken(m_uiTargetInterior);
		pDataWriter->writeToken(m_uiMarkerType);
		pDataWriter->writeToken(m_strInteriorName);
		pDataWriter->writeToken(m_uiSkyColourChanger);
		pDataWriter->writeToken(m_uiUnknown1);
		pDataWriter->writeToken(m_uiTimeOn);
		pDataWriter->writeToken(m_uiTimeOff);
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}