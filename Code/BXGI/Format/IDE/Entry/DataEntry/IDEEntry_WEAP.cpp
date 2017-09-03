#include "IDEEntry_WEAP.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

IDEEntry_WEAP::IDEEntry_WEAP(IDEFormat *pIDEFormat) :
	IDEEntry_Data(pIDEFormat, IDE_SECTION_WEAP),
	m_uiObjectId(0),
	m_uiClumpCount(0),
	m_fDrawDistance(0.0f),
	m_uiFlags(0)
{
}

void			IDEEntry_WEAP::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();

	switch (pDataReader->getLineTokens().size())
	{
	case 7:
		// GTA VC, GTA SA
		setFormatType(0);
		setFormatGames(GAME_FLAG_GTA_VC | GAME_FLAG_GTA_SA);
		m_uiObjectId = pDataReader->readTokenUint32();
		m_strModelName = pDataReader->readTokenString();
		m_strTXDName = pDataReader->readTokenString();
		m_strAnimationName = pDataReader->readTokenString();
		m_uiClumpCount = pDataReader->readTokenUint32();
		m_fDrawDistance = pDataReader->readTokenFloat32();
		m_uiFlags = pDataReader->readTokenUint32();
		break;
	case 6:
		// GTA IV
		setFormatType(1);
		setFormatGames(GAME_FLAG_GTA_IV);
		m_strModelName = pDataReader->readTokenString();
		m_strTXDName = pDataReader->readTokenString();
		m_strAnimationName = pDataReader->readTokenString();
		m_uiClumpCount = pDataReader->readTokenUint32();
		m_fDrawDistance = pDataReader->readTokenFloat32();
		m_uiFlags = pDataReader->readTokenUint32();
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}

void			IDEEntry_WEAP::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	switch (getFormatType())
	{
	case 0:
		pDataWriter->writeToken(m_uiObjectId);
		pDataWriter->writeToken(m_strModelName);
		pDataWriter->writeToken(m_strTXDName);
		pDataWriter->writeToken(m_strAnimationName);
		pDataWriter->writeToken(m_uiClumpCount);
		pDataWriter->writeToken(m_fDrawDistance);
		pDataWriter->writeToken(m_uiFlags);
		break;
	case 1:
		pDataWriter->writeToken(m_strModelName);
		pDataWriter->writeToken(m_strTXDName);
		pDataWriter->writeToken(m_strAnimationName);
		pDataWriter->writeToken(m_uiClumpCount);
		pDataWriter->writeToken(m_fDrawDistance);
		pDataWriter->writeToken(m_uiFlags);
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}