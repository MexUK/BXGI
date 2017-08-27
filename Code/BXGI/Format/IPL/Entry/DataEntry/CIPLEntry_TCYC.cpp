#include "CIPLEntry_TCYC.h"
#include "Exception/EExceptionCode.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

CIPLEntry_TCYC::CIPLEntry_TCYC(CIPLFormat *pIPLFormat) :
	CIPLEntry_Data(pIPLFormat, IPL_SECTION_TCYC)
{
}

void			CIPLEntry_TCYC::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();

	switch (pDataReader->getLineTokens().size())
	{
	case 11:
		setFormatType(0);
		setFormatGames(GAME_FLAG_GTA_SA);

		m_vecPositionLower = pDataReader->readTokenVector3D();
		m_vecPositionUpper = pDataReader->readTokenVector3D();
		m_uiUnknown1 = pDataReader->readTokenUint32();
		m_uiWeatherId = pDataReader->readTokenUint32();
		m_iBrightness = pDataReader->readTokenInt32();
		m_uiTime = pDataReader->readTokenUint32();
		m_uiDrawDistance = pDataReader->readTokenUint32();
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}

void			CIPLEntry_TCYC::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();

	switch (getFormatType())
	{
	case 0:
		pDataWriter->writeVector3D(m_vecPositionLower);
		pDataWriter->writeVector3D(m_vecPositionUpper);
		pDataWriter->writeUint32(m_uiUnknown1);
		pDataWriter->writeUint32(m_uiWeatherId);
		pDataWriter->writeInt32(m_iBrightness);
		pDataWriter->writeUint32(m_uiTime);
		pDataWriter->writeUint32(m_uiDrawDistance);
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}