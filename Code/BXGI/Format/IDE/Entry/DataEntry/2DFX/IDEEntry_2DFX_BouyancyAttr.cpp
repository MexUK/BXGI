#include "IDEEntry_2DFX_BouyancyAttr.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/IDE/IDEFormat.h"

using namespace bxcf;
using namespace bxgi;

IDEEntry_2DFX_BouyancyAttr::IDEEntry_2DFX_BouyancyAttr(IDEFormat *pIDEFormat) :
	IDEEntry_2DFX(pIDEFormat, _2DFX_BOUYANCY_ATTR)
{
}

void			IDEEntry_2DFX_BouyancyAttr::unserialize(void)
{
	DataReader *pDataReader = &m_pFormat->m_reader;
	
	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}

void			IDEEntry_2DFX_BouyancyAttr::serialize(void)
{
	DataWriter *pDataWriter = &m_pFormat->m_writer;

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}