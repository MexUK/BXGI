#include "RWSection_String.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Static/String.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

RWSection_String::RWSection_String(void)
{
	setSectionId(RW_SECTION_STRING);
}

void							RWSection_String::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();

	// todo
	//uint64 uiPreviousSeek = pDataReader->getSeek();
	//pDataReader->setPeek(true);
	m_strData = pDataReader->readStringUntilZero();
	uint32 uiBytePadCount = ((m_strData.length() + 1) % 4);
	if (uiBytePadCount != 0)
	{
		pDataReader->readString(4 - uiBytePadCount);
	}
	//pDataReader->setPeek(false);
	//pDataReader->setSeek(uiPreviousSeek);
}

void							RWSection_String::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	uint32 uiStringLength = (uint32)(ceil(((float32)(m_strData.length() + 1)) / 4.0) * 4);
	string strData = String::zeroPad(m_strData, uiStringLength);

	pDataWriter->writeStringRef(strData);
}