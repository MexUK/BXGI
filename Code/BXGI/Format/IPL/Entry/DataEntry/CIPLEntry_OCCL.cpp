#include "CIPLEntry_OCCL.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

CIPLEntry_OCCL::CIPLEntry_OCCL(CIPLFormat *pIPLFormat) :
	CIPLEntry_Data(pIPLFormat, IPL_SECTION_OCCL),
	m_vecMidPosition{ 0.0f, 0.0f },
	m_fBottomHeightZ(0.0f),
	m_vecWidth{ 0.0f, 0.0f },
	m_fHeight(0.0f),
	m_fRotation(0.0f),
	m_fUnknown1(0.0f),
	m_fUnknown2(0.0f),
	m_uiUnknown3(0)
{
}

void			CIPLEntry_OCCL::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();

	switch (pDataReader->getLineTokens().size())
	{
	case 7:
		setFormatType(0);
		setFormatGames(GAME_FLAG_GTA_VC);

		m_vecMidPosition = pDataReader->readTokenVector2D();
		m_fBottomHeightZ = pDataReader->readTokenFloat32();
		m_vecWidth = pDataReader->readTokenVector2D();
		m_fHeight = pDataReader->readTokenFloat32();
		m_fRotation = pDataReader->readTokenFloat32();
		break;
	case 10:
		setFormatType(1);
		setFormatGames(GAME_FLAG_GTA_SA);

		m_vecMidPosition = pDataReader->readTokenVector2D();
		m_fBottomHeightZ = pDataReader->readTokenFloat32();
		m_vecWidth = pDataReader->readTokenVector2D();
		m_fHeight = pDataReader->readTokenFloat32();
		m_fRotation = pDataReader->readTokenFloat32();
		m_fUnknown1 = pDataReader->readTokenFloat32();
		m_fUnknown2 = pDataReader->readTokenFloat32();
		m_uiUnknown3 = pDataReader->readTokenUint32();
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}

void			CIPLEntry_OCCL::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	switch (getFormatType())
	{
	case 0:
		pDataWriter->writeToken(m_vecMidPosition);
		pDataWriter->writeToken(m_fBottomHeightZ);
		pDataWriter->writeToken(m_vecWidth);
		pDataWriter->writeToken(m_fHeight);
		pDataWriter->writeToken(m_fRotation);
		break;
	case 1:
		pDataWriter->writeToken(m_vecMidPosition);
		pDataWriter->writeToken(m_fBottomHeightZ);
		pDataWriter->writeToken(m_vecWidth);
		pDataWriter->writeToken(m_fHeight);
		pDataWriter->writeToken(m_fRotation);
		pDataWriter->writeToken(m_fUnknown1);
		pDataWriter->writeToken(m_fUnknown2);
		pDataWriter->writeToken(m_uiUnknown3);
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}