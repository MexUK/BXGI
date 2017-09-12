#include "IDEEntry_VNOD.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/IDE/IDEFormat.h"

using namespace bxcf;
using namespace bxgi;

IDEEntry_VNOD::IDEEntry_VNOD(IDEFormat *pIDEFormat) :
	IDEEntry_Data(pIDEFormat, IDE_SECTION_VNOD)
{
}

void			IDEEntry_VNOD::unserialize(void)
{
	DataReader *pDataReader = &m_pFormat->m_reader;

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}

void			IDEEntry_VNOD::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}