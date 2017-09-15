#include "IPLEntry_TCYC.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/IPL/IPLFormat.h"

using namespace bxcf;
using namespace bxgi;

IPLEntry_TCYC::IPLEntry_TCYC(IPLFormat *pIPLFormat) :
	IPLEntry_Data(pIPLFormat, IPL_SECTION_TCYC)
{
}

void			IPLEntry_TCYC::unserialize(void)
{
	DataReader *pDataReader = &m_pFormat->m_reader;

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

void			IPLEntry_TCYC::serialize(void)
{
	DataWriter *pDataWriter = &m_pFormat->m_writer;

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