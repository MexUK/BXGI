#include "IPLEntry_PICK.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

IPLEntry_PICK::IPLEntry_PICK(IPLFormat *pIPLFormat) :
	IPLEntry_Data(pIPLFormat, IPL_SECTION_PICK)
{
}

void			IPLEntry_PICK::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();

	switch (pDataReader->getLineTokens().size())
	{
	case 4:
		setFormatType(0);
		setFormatGames(GAME_FLAG_GTA_SA);

		m_uiWeaponId = pDataReader->readTokenUint32();
		m_vecPosition = pDataReader->readTokenVector3D();
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}

void			IPLEntry_PICK::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	switch (getFormatType())
	{
	case 0:
		pDataWriter->writeToken(m_uiWeaponId);
		pDataWriter->writeToken(m_vecPosition);
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}