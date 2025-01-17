#include "IDEEntry_2DFX_Light.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Static/String.h"
#include "Format/IDE/IDEFormat.h"

using namespace bxcf;
using namespace bxgi;

IDEEntry_2DFX_Light::IDEEntry_2DFX_Light(IDEFormat *pIDEFormat) :
	IDEEntry_2DFX(pIDEFormat, _2DFX_LIGHT),
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

void			IDEEntry_2DFX_Light::unserialize(void)
{
	DataReader *pDataReader = &m_pFormat->m_reader;

	if(doesSupportFormatGame(GAME_FLAG_GTA_IV))
	{
		setFormatType(1);
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
	else
	{
		setFormatType(0);

		IDEEntry_2DFX::unserialize();
		
		m_strCoronaTexture = String::trim(String::trim(pDataReader->readTokenString(), "\""));
		m_strShadowTexture = String::trim(String::trim(pDataReader->readTokenString(), "\""));
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

void			IDEEntry_2DFX_Light::serialize(void)
{
	DataWriter *pDataWriter = &m_pFormat->m_writer;

	switch (getFormatType())
	{
	case 0:
		IDEEntry_2DFX::serialize();

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