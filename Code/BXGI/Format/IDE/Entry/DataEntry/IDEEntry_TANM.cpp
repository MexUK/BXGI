#include "IDEEntry_TANM.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/IDE/IDEFormat.h"

using namespace bxcf;
using namespace bxgi;

IDEEntry_TANM::IDEEntry_TANM(IDEFormat *pIDEFormat) :
	IDEEntry_Data(pIDEFormat, IDE_SECTION_TANM)
{
}

void			IDEEntry_TANM::unserialize(void)
{
	DataReader *pDataReader = &m_pFormat->m_reader;

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}

void			IDEEntry_TANM::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}