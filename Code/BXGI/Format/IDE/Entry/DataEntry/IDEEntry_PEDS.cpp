#include "IDEEntry_PEDS.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/IDE/IDEFormat.h"

using namespace bxcf;
using namespace bxgi;

IDEEntry_PEDS::IDEEntry_PEDS(IDEFormat *pIDEFormat) :
	IDEEntry_Data(pIDEFormat, IDE_SECTION_PEDS),
	m_uiObjectId(0),
	m_uiCarsCanDrive(0)
{
	m_vecPreferredRadioStations.x = 0;
	m_vecPreferredRadioStations.y = 0;
}

void			IDEEntry_PEDS::unserialize(void)
{
	DataReader *pDataReader = &m_pFormat->m_reader;

	uint32 uiLineTokenCount = pDataReader->getLineTokens().size();
	bool bMatchFound = false;
	if (uiLineTokenCount == 7 || uiLineTokenCount == 11 || uiLineTokenCount == 15)
	{
		// GTA III, GTA VC, GTA SA
		bMatchFound = true;
		setFormatType(0);
		setFormatGames(GAME_FLAG_GTA_III | GAME_FLAG_GTA_VC | GAME_FLAG_GTA_SA);
		m_uiObjectId = pDataReader->readTokenUint32();
		m_strModelName = pDataReader->readTokenString();
		m_strTXDName = pDataReader->readTokenString();
		m_strDefaultPedType = pDataReader->readTokenString();
		m_strBehaviour = pDataReader->readTokenString();
		m_strAnimationGroup = pDataReader->readTokenString();
		m_uiCarsCanDrive = pDataReader->readTokenUint32();

		if (uiLineTokenCount == 11 || uiLineTokenCount == 15)
		{
			// GTA VC, GTA SA
			setFormatType(1);
			setFormatGames(GAME_FLAG_GTA_VC | GAME_FLAG_GTA_SA);
			m_strAnimationFile = pDataReader->readTokenString();
			m_vecPreferredRadioStations.x = pDataReader->readTokenUint32();
			m_vecPreferredRadioStations.y = pDataReader->readTokenUint32();

			if (uiLineTokenCount == 15)
			{
				// GTA SA
				setFormatType(2);
				setFormatGames(GAME_FLAG_GTA_SA);
				m_strVoiceArchive = pDataReader->readTokenString();
				m_strVoices[0] = pDataReader->readTokenString();
				m_strVoices[1] = pDataReader->readTokenString();
			}
		}
	}

	if (uiLineTokenCount == 16)
	{
		// GTA IV
		bMatchFound = true;
		setFormatType(3);
		setFormatGames(GAME_FLAG_GTA_IV);
		m_strModelName = pDataReader->readTokenString();
		m_strProps = pDataReader->readTokenString();
		m_strDefaultPedType = pDataReader->readTokenString();
		m_strAnimationGroup = pDataReader->readTokenString();
		m_strGestureGroup = pDataReader->readTokenString();
		m_strGesturePhoneGroup = pDataReader->readTokenString();
		m_strFacialGroup = pDataReader->readTokenString();
		m_strVisemeGroup = pDataReader->readTokenString();
		m_strFlags = pDataReader->readTokenString();
		m_strAnimationFile = pDataReader->readTokenString();
		m_strBlendShapeFile = pDataReader->readTokenString();
		m_vecPreferredRadioStations.x = pDataReader->readTokenUint32();
		m_vecPreferredRadioStations.y = pDataReader->readTokenUint32();
		m_strAudioType = pDataReader->readTokenString();
		m_strFirstVoice = pDataReader->readTokenString();
		m_strLastVoice = pDataReader->readTokenString();
	}

	if (!bMatchFound)
	{
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}

void			IDEEntry_PEDS::serialize(void)
{
	DataWriter *pDataWriter = &m_pFormat->m_writer;

	switch (getFormatType())
	{
	case 0:
	case 1:
	case 2:
		pDataWriter->writeToken(m_uiObjectId);
		pDataWriter->writeToken(m_strModelName);
		pDataWriter->writeToken(m_strTXDName);
		pDataWriter->writeToken(m_strDefaultPedType);
		pDataWriter->writeToken(m_strBehaviour);
		pDataWriter->writeToken(m_strAnimationGroup);
		pDataWriter->writeToken(m_uiCarsCanDrive);
		if (getFormatType() >= 1)
		{
			pDataWriter->writeToken(m_strAnimationFile);
			pDataWriter->writeToken(m_vecPreferredRadioStations.x);
			pDataWriter->writeToken(m_vecPreferredRadioStations.y);
			if (getFormatType() == 2)
			{
				pDataWriter->writeToken(m_strVoiceArchive);
				pDataWriter->writeToken(m_strVoices[0]);
				pDataWriter->writeToken(m_strVoices[1]);
			}
		}
		break;
	case 3:
		pDataWriter->writeToken(m_strModelName);
		pDataWriter->writeToken(m_strProps);
		pDataWriter->writeToken(m_strDefaultPedType);
		pDataWriter->writeToken(m_strAnimationGroup);
		pDataWriter->writeToken(m_strGestureGroup);
		pDataWriter->writeToken(m_strGesturePhoneGroup);
		pDataWriter->writeToken(m_strFacialGroup);
		pDataWriter->writeToken(m_strVisemeGroup);
		pDataWriter->writeToken(m_strFlags);
		pDataWriter->writeToken(m_strAnimationFile);
		pDataWriter->writeToken(m_strBlendShapeFile);
		pDataWriter->writeToken(m_vecPreferredRadioStations.x);
		pDataWriter->writeToken(m_vecPreferredRadioStations.y);
		pDataWriter->writeToken(m_strAudioType);
		pDataWriter->writeToken(m_strFirstVoice);
		pDataWriter->writeToken(m_strLastVoice);
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}