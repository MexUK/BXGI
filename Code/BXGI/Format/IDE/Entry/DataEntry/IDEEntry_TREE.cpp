#include "IDEEntry_TREE.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/IDE/IDEFormat.h"

using namespace bxcf;
using namespace bxgi;

IDEEntry_TREE::IDEEntry_TREE(IDEFormat *pIDEFormat) :
	IDEEntry_Data(pIDEFormat, IDE_SECTION_TREE)
{
}

void			IDEEntry_TREE::unserialize(void)
{
	DataReader *pDataReader = &m_pFormat->m_reader;

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}

void			IDEEntry_TREE::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}