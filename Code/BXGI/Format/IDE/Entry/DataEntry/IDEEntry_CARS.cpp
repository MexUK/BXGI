#include "IDEEntry_CARS.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Static/String.h"
#include "Format/IDE/IDEFormat.h"

using namespace bxcf;
using namespace bxgi;

IDEEntry_CARS::IDEEntry_CARS(IDEFormat *pIDEFormat) :
	IDEEntry_Data(pIDEFormat, IDE_SECTION_CARS),
	m_uiObjectId(0),
	m_uiSpawnFrequency(0),
	m_uiUnknown1(0),
	m_uiUnknown2(0),
	m_uiWheelModelId(0),
	m_fWheelScaleFront(0.0f),
	m_fWheelScaleRear(0.0f),
	m_fWheelScale(0.0f),
	m_uiUnknown3(0),
	m_uiMaxVehicleCountAtOneTime(0),
	m_vecWheelRadius{ 0.0f, 0.0f },
	m_fDirtLevel(0.0f),
	m_uiLODMultiplier(0),
	m_fSwankness(0.0f),
	m_uiFlags(0)
{
}

void			IDEEntry_CARS::unserialize(void)
{
	DataReader *pDataReader = &m_pFormat->m_reader;
	uint32 uiTokenCount = pDataReader->getLineTokens().size();

	switch (uiTokenCount)
	{
	case 10: // GTA III & GTA VC
	case 11:
	case 12:
	case 13:
		setFormatType(0);
		setFormatGames(GAME_FLAG_GTA_III | GAME_FLAG_GTA_VC);
		m_uiObjectId = pDataReader->readTokenUint32();
		m_strModelName = pDataReader->readTokenString();
		m_strTXDName = pDataReader->readTokenString();
		m_strVehicleType = pDataReader->readTokenString();
		m_strHandlingId = pDataReader->readTokenString();
		m_strGameName = pDataReader->readTokenString();
		m_strAnimationFile = pDataReader->readTokenString();
		m_strVehicleClass = pDataReader->readTokenString();
		m_uiSpawnFrequency = pDataReader->readTokenUint32();
		m_uiUnknown1 = pDataReader->readTokenUint32();
		
		if (uiTokenCount >= 11)
		{
			m_uiUnknown2 = pDataReader->readTokenUint32();
			if (uiTokenCount >= 12)
			{
				m_uiWheelModelId = pDataReader->readTokenUint32();
				if (uiTokenCount >= 13)
				{
					setFormatType(1);
					setFormatGames(GAME_FLAG_GTA_VC);
					m_fWheelScale = pDataReader->readTokenFloat32();
				}
			}
		}
		break;
	case 15: // GTA SA or GTA IV
	{
		pDataReader->setPeek(true);
		bool bIsSA = String::isPositiveInteger(pDataReader->readTokenString());
		pDataReader->setPeek(false);
		if (bIsSA)
		{
			// GTA SA
			setFormatType(2);
			setFormatGames(GAME_FLAG_GTA_SA);
			m_uiObjectId = pDataReader->readTokenUint32();
			m_strModelName = pDataReader->readTokenString();
			m_strTXDName = pDataReader->readTokenString();
			m_strVehicleType = pDataReader->readTokenString();
			m_strHandlingId = pDataReader->readTokenString();
			m_strGameName = pDataReader->readTokenString();
			m_strAnimationFile = pDataReader->readTokenString();
			m_strVehicleClass = pDataReader->readTokenString();
			m_uiSpawnFrequency = pDataReader->readTokenUint32();
			m_uiUnknown1 = pDataReader->readTokenUint32();
			m_uiUnknown2 = pDataReader->readTokenUint32();
			m_uiWheelModelId = pDataReader->readTokenUint32();
			m_fWheelScaleFront = pDataReader->readTokenFloat32();
			m_fWheelScaleRear = pDataReader->readTokenFloat32();
			m_uiUnknown3 = pDataReader->readTokenUint32();
		}
		else
		{
			// GTA IV
			setFormatType(3);
			setFormatGames(GAME_FLAG_GTA_IV);
			m_strModelName = pDataReader->readTokenString();
			m_strTXDName = pDataReader->readTokenString();
			m_strVehicleType = pDataReader->readTokenString();
			m_strHandlingId = pDataReader->readTokenString();
			m_strGameName = pDataReader->readTokenString();
			m_strAnimationFile = pDataReader->readTokenString();
			m_strAnimationFile2 = pDataReader->readTokenString();
			m_uiSpawnFrequency = pDataReader->readTokenUint32();
			m_uiMaxVehicleCountAtOneTime = pDataReader->readTokenUint32();
			m_vecWheelRadius = pDataReader->readTokenVector2D();
			m_fDirtLevel = pDataReader->readTokenFloat32();
			m_uiLODMultiplier = pDataReader->readTokenUint32();
			m_fSwankness = pDataReader->readTokenFloat32();
			m_uiFlags = pDataReader->readTokenUint32();
		}
		break;
	}
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}

void			IDEEntry_CARS::serialize(void)
{
	DataWriter *pDataWriter = &m_pFormat->m_writer;

	switch (getFormatType())
	{
	case 0:
		pDataWriter->writeToken(m_uiObjectId);
		pDataWriter->writeToken(m_strModelName);
		pDataWriter->writeToken(m_strTXDName);
		pDataWriter->writeToken(m_strVehicleType);
		pDataWriter->writeToken(m_strHandlingId);
		pDataWriter->writeToken(m_strGameName);
		pDataWriter->writeToken(m_strVehicleClass);
		pDataWriter->writeToken(m_uiSpawnFrequency);
		pDataWriter->writeToken(m_uiUnknown1);
		pDataWriter->writeToken(m_uiUnknown2);
		pDataWriter->writeToken(m_uiWheelModelId);
		pDataWriter->writeToken(m_fWheelScale);
		break;
	case 1:
		pDataWriter->writeToken(m_uiObjectId);
		pDataWriter->writeToken(m_strModelName);
		pDataWriter->writeToken(m_strTXDName);
		pDataWriter->writeToken(m_strVehicleType);
		pDataWriter->writeToken(m_strHandlingId);
		pDataWriter->writeToken(m_strGameName);
		pDataWriter->writeToken(m_strAnimationFile);
		pDataWriter->writeToken(m_strVehicleClass);
		pDataWriter->writeToken(m_uiSpawnFrequency);
		pDataWriter->writeToken(m_uiUnknown1);
		pDataWriter->writeToken(m_uiUnknown2);
		pDataWriter->writeToken(m_uiWheelModelId);
		pDataWriter->writeToken(m_fWheelScale);
		break;
	case 2:
		pDataWriter->writeToken(m_uiObjectId);
		pDataWriter->writeToken(m_strModelName);
		pDataWriter->writeToken(m_strTXDName);
		pDataWriter->writeToken(m_strVehicleType);
		pDataWriter->writeToken(m_strHandlingId);
		pDataWriter->writeToken(m_strGameName);
		pDataWriter->writeToken(m_strAnimationFile);
		pDataWriter->writeToken(m_strVehicleClass);
		pDataWriter->writeToken(m_uiSpawnFrequency);
		pDataWriter->writeToken(m_uiUnknown1);
		pDataWriter->writeToken(m_uiUnknown2);
		pDataWriter->writeToken(m_uiWheelModelId);
		pDataWriter->writeToken(m_fWheelScaleFront);
		pDataWriter->writeToken(m_fWheelScaleRear);
		pDataWriter->writeToken(m_uiUnknown3);
		break;
	case 3:
		pDataWriter->writeToken(m_strModelName);
		pDataWriter->writeToken(m_strTXDName);
		pDataWriter->writeToken(m_strVehicleType);
		pDataWriter->writeToken(m_strHandlingId);
		pDataWriter->writeToken(m_strGameName);
		pDataWriter->writeToken(m_strAnimationFile);
		pDataWriter->writeToken(m_strAnimationFile2);
		pDataWriter->writeToken(m_uiSpawnFrequency);
		pDataWriter->writeToken(m_uiMaxVehicleCountAtOneTime);
		pDataWriter->writeToken(m_vecWheelRadius);
		pDataWriter->writeToken(m_fDirtLevel);
		pDataWriter->writeToken(m_uiLODMultiplier);
		pDataWriter->writeToken(m_fSwankness);
		pDataWriter->writeToken(m_uiFlags);
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}