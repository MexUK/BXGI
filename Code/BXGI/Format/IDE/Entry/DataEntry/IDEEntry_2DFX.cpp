#include "IDEEntry_2DFX.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/IDE/IDEFormat.h"

using namespace bxgi;

IDEEntry_2DFX::IDEEntry_2DFX(IDEFormat *pIDEFormat, E2DFXType E2DFXTypeValue) :
	IDEEntry_Data(pIDEFormat, IDE_SECTION_2DFX),
	m_E2DFXType(E2DFXTypeValue),
	m_uiObjectId(0),
	m_vecPosition{ 0.0f, 0.0f, 0.0f },
	m_uiUnknown1(0),
	m_ui2dfxType(0),
	m_vecRotation{ 0.0f, 0.0f, 0.0f, 0.0f }
{
	m_vecColour.x = 0;
	m_vecColour.y = 0;
	m_vecColour.z = 0;
}

IDEEntry_2DFX::~IDEEntry_2DFX(void)
{
}

void			IDEEntry_2DFX::unserialize(void)
{
	DataReader *pDataReader = &m_pFormat->m_reader;
	
	if(doesSupportFormatGame(GAME_FLAG_GTA_IV))
	{
		setFormatGames(GAME_FLAG_GTA_IV);
		m_strModelName = pDataReader->readTokenString();
		m_vecPosition = pDataReader->readTokenVector3D();
		m_uiUnknown1 = pDataReader->readTokenUint32();
		m_vecRotation = pDataReader->readTokenVector4D();
	}
	else
	{
		setFormatGames(GAME_FLAG_GTA_III | GAME_FLAG_GTA_VC);
		m_uiObjectId = pDataReader->readTokenUint32();
		m_vecPosition = pDataReader->readTokenVector3D();
		m_vecColour.x = pDataReader->readTokenUint32();
		m_vecColour.y = pDataReader->readTokenUint32();
		m_vecColour.z = pDataReader->readTokenUint32();
		m_uiUnknown1 = pDataReader->readTokenUint32();
		m_ui2dfxType = pDataReader->readTokenUint32();
	}
}

void			IDEEntry_2DFX::serialize(void)
{
	DataWriter *pDataWriter = &m_pFormat->m_writer;

	if (doesSupportFormatGame(GAME_FLAG_GTA_IV))
	{
		pDataWriter->writeStringRef(m_strModelName);
		pDataWriter->writeTokenVector3D(m_vecPosition);
		pDataWriter->writeUint32(m_uiUnknown1);
		pDataWriter->writeTokenVector4D(m_vecRotation);
	}
	else
	{
		pDataWriter->writeUint32(m_uiObjectId);
		pDataWriter->writeTokenVector3D(m_vecPosition);
		pDataWriter->writeTokenVector3ui32(m_vecColour);
		pDataWriter->writeUint32(m_uiUnknown1);
		pDataWriter->writeUint32(m_ui2dfxType);
	}
}