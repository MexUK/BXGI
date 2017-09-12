#include "IDEEntry_LINK.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/IDE/IDEFormat.h"

using namespace bxcf;
using namespace bxgi;

IDEEntry_LINK::IDEEntry_LINK(IDEFormat *pIDEFormat) :
	IDEEntry_Data(pIDEFormat, IDE_SECTION_LINK)
{
}

void			IDEEntry_LINK::unserialize(void)
{
	DataReader *pDataReader = &m_pFormat->m_reader;

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}

void			IDEEntry_LINK::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}