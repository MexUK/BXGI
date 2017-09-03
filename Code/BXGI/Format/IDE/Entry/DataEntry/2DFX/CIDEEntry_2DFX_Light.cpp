#include "CIDEEntry_2DFX_Light.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Static/String2.h"

using namespace bxcf;
using namespace bxgi;

CIDEEntry_2DFX_Light::CIDEEntry_2DFX_Light(CIDEFormat *pIDEFormat) :
	CIDEEntry_2DFX(pIDEFormat, _2DFX_LIGHT),
	m_fViewDistance(0.0f),
	m_fOuterRange(0.0f),
	m_fCoronaSize(0.0f),
	m_fInnerRange(0.0f),
	m_uiIDEFlag(0),
	m_uiFlash(0),
	m_uiWet(0),
	m_uiFlare(0),
	m_uiDust(0)
{
}

void			CIDEEntry_2DFX_Light::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();

	if(doesSupportFormatGame(GAME_FLAG_GTA_IV))
	{
		setFormatType(1);
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
	else
	{
		setFormatType(0);

		CIDEEntry_2DFX::unserialize();
		
		m_strCoronaTexture = String2::trim(String2::trim(pDataReader->readTokenString(), "\""));
		m_strShadowTexture = String2::trim(String2::trim(pDataReader->readTokenString(), "\""));
		m_fViewDistance = pDataReader->readTokenFloat32();
		m_fOuterRange = pDataReader->readTokenFloat32();
		m_fCoronaSize = pDataReader->readTokenFloat32();
		m_fInnerRange = pDataReader->readTokenFloat32();
		m_uiIDEFlag = pDataReader->readTokenUint32();
		m_uiFlash = pDataReader->readTokenUint32();
		m_uiWet = pDataReader->readTokenUint32();
		m_uiFlare = pDataReader->readTokenUint32();
		m_uiDust = pDataReader->readTokenUint32();
	}
}

void			CIDEEntry_2DFX_Light::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	switch (getFormatType())
	{
	case 0:
		CIDEEntry_2DFX::serialize();

		pDataWriter->writeToken("\"" + m_strCoronaTexture + "\"");
		pDataWriter->writeToken("\"" + m_strShadowTexture + "\"");
		pDataWriter->writeToken(m_fViewDistance);
		pDataWriter->writeToken(m_fOuterRange);
		pDataWriter->writeToken(m_fCoronaSize);
		pDataWriter->writeToken(m_fInnerRange);
		pDataWriter->writeToken(m_uiIDEFlag);
		pDataWriter->writeToken(m_uiFlash);
		pDataWriter->writeToken(m_uiWet);
		pDataWriter->writeToken(m_uiFlare);
		pDataWriter->writeToken(m_uiDust);
		break;
	case 1:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}