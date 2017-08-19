#include "CIDEEntry_2DFX.h"
#include "Exception/eExceptionCode.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxgi;

void			CIDEEntry_2DFX::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();
	
	if(doesSupportFormatGame(GAME_FLAG_GTA_IV))
	{
		setFormatGames(GAME_FLAG_GTA_IV);
		m_strModelName = pDataReader->readTokenString();
		m_vecPosition = pDataReader->readVector3D();
		m_uiUnknown1 = pDataReader->readTokenUint32();
		m_vecRotation = pDataReader->readVector4D();
	}
	else
	{
		setFormatGames(GAME_FLAG_GTA_III | GAME_FLAG_GTA_VC);
		m_uiObjectId = pDataReader->readTokenUint32();
		m_vecPosition = pDataReader->readVector3D();
		m_vecColour.x = pDataReader->readTokenUint32();
		m_vecColour.y = pDataReader->readTokenUint32();
		m_vecColour.z = pDataReader->readTokenUint32();
		m_uiUnknown1 = pDataReader->readTokenUint32();
		m_ui2dfxType = pDataReader->readTokenUint32();
	}
}

void			CIDEEntry_2DFX::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();

	if (doesSupportFormatGame(GAME_FLAG_GTA_IV))
	{
		pDataWriter->writeStringRef(m_strModelName);
		pDataWriter->writeVector3D(m_vecPosition);
		pDataWriter->writeUint32(m_uiUnknown1);
		pDataWriter->writeVector4D(m_vecRotation);
	}
	else
	{
		pDataWriter->writeUint32(m_uiObjectId);
		pDataWriter->writeVector3D(m_vecPosition);
		pDataWriter->writeVector3ui32(m_vecColour);
		pDataWriter->writeUint32(m_uiUnknown1);
		pDataWriter->writeUint32(m_ui2dfxType);
	}
}