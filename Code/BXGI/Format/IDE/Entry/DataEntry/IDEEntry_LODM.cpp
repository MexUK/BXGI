#include "IDEEntry_LODM.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/IDE/IDEFormat.h"

using namespace bxcf;
using namespace bxgi;

IDEEntry_LODM::IDEEntry_LODM(IDEFormat *pIDEFormat) :
	IDEEntry_Data(pIDEFormat, IDE_SECTION_LODM)
{
}

void			IDEEntry_LODM::unserialize(void)
{
	DataReader *pDataReader = &m_pFormat->m_reader;

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}

void			IDEEntry_LODM::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}