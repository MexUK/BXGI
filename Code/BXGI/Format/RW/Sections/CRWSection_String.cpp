#include "CRWSection_String.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"
#include "Static/CString2.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

CRWSection_String::CRWSection_String(void)
{
	setSectionId(RW_SECTION_STRING);
}

void							CRWSection_String::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();

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

void							CRWSection_String::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();

	uint32 uiStringLength = (uint32)(ceil(((float32)(m_strData.length() + 1)) / 4.0) * 4);
	string strData = CString2::zeroPad(m_strData, uiStringLength);

	pDataWriter->writeStringRef(strData);
}