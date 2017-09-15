#include "IPLEntry_CARS.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/IPL/IPLFormat.h"

using namespace bxcf;
using namespace bxgi;

IPLEntry_CARS::IPLEntry_CARS(IPLFormat *pIPLFormat) :
	IPLEntry_Data(pIPLFormat, IPL_SECTION_CARS),
	m_vecPosition{ 0.0f, 0.0f, 0.0f },
	zRotation(0.0f),
	m_iVehicleModelId(0),
	m_uiForceSpawn(0),
	m_uiAlarmTriggerProbability(0),
	m_uiDoorLockProbability(0),
	m_uiUnknown1(0),
	m_uiUnknown2(0)
{
	m_iVehicleColour.x = 0;
	m_iVehicleColour.y = 0;
}

void			IPLEntry_CARS::unserialize(void)
{
	if (getFormat()->isBinary())
	{
		unserialize_Binary();
	}
	else
	{
		unserialize_Text();
	}
}

void			IPLEntry_CARS::unserialize_Text(void)
{
	DataReader *pDataReader = &m_pFormat->m_reader;

	switch (pDataReader->getLineTokenCount())
	{
	case 12:
		setFormatType(0);
		setFormatGames(GAME_FLAG_GTA_SA);

		m_vecPosition = pDataReader->readTokenVector3D();
		zRotation = pDataReader->readTokenFloat32();
		m_iVehicleModelId = pDataReader->readTokenInt32();
		m_iVehicleColour.x = pDataReader->readTokenInt32();
		m_iVehicleColour.y = pDataReader->readTokenInt32();
		m_uiForceSpawn = pDataReader->readTokenUint32();
		m_uiAlarmTriggerProbability = pDataReader->readTokenUint32();
		m_uiDoorLockProbability = pDataReader->readTokenUint32();
		m_uiUnknown1 = pDataReader->readTokenUint32();
		m_uiUnknown2 = pDataReader->readTokenUint32();
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}

void			IPLEntry_CARS::unserialize_Binary(void)
{
	DataReader *pDataReader = &m_pFormat->m_reader;

	setFormatType(0);
	setFormatGames(GAME_FLAG_GTA_SA);

	m_vecPosition = pDataReader->readVector3D();
	zRotation = pDataReader->readFloat32();
	m_iVehicleModelId = pDataReader->readInt32();
	m_iVehicleColour.x = pDataReader->readInt32();
	m_iVehicleColour.y = pDataReader->readInt32();
	m_uiForceSpawn = pDataReader->readUint32();
	m_uiAlarmTriggerProbability = pDataReader->readUint32();
	m_uiDoorLockProbability = pDataReader->readUint32();
	m_uiUnknown1 = pDataReader->readUint32();
	m_uiUnknown2 = pDataReader->readUint32();
}

void			IPLEntry_CARS::serialize(void)
{
	if (getFormat()->isBinary())
	{
		serialize_Binary();
	}
	else
	{
		serialize_Text();
	}
}

void			IPLEntry_CARS::serialize_Text(void)
{
	DataWriter *pDataWriter = &m_pFormat->m_writer;

	switch (getFormatType())
	{
	case 0:
		pDataWriter->writeToken(m_vecPosition);
		pDataWriter->writeToken(zRotation);
		pDataWriter->writeToken(m_iVehicleModelId);
		pDataWriter->writeToken(m_iVehicleColour.x);
		pDataWriter->writeToken(m_iVehicleColour.y);
		pDataWriter->writeToken(m_uiForceSpawn);
		pDataWriter->writeToken(m_uiAlarmTriggerProbability);
		pDataWriter->writeToken(m_uiDoorLockProbability);
		pDataWriter->writeToken(m_uiUnknown1);
		pDataWriter->writeToken(m_uiUnknown2);
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}

void			IPLEntry_CARS::serialize_Binary(void)
{
	DataWriter *pDataWriter = &m_pFormat->m_writer;

	switch (getFormatType())
	{
	case 0:
		pDataWriter->writeVector3D(m_vecPosition);
		pDataWriter->writeFloat32(zRotation);
		pDataWriter->writeInt32(m_iVehicleModelId);
		pDataWriter->writeInt32(m_iVehicleColour.x);
		pDataWriter->writeInt32(m_iVehicleColour.y);
		pDataWriter->writeUint32(m_uiForceSpawn);
		pDataWriter->writeUint32(m_uiAlarmTriggerProbability);
		pDataWriter->writeUint32(m_uiDoorLockProbability);
		pDataWriter->writeUint32(m_uiUnknown1);
		pDataWriter->writeUint32(m_uiUnknown2);
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}