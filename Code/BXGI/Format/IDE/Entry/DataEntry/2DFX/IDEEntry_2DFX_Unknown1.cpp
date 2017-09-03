#include "IDEEntry_2DFX_Unknown1.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

IDEEntry_2DFX_Unknown1::IDEEntry_2DFX_Unknown1(IDEFormat *pIDEFormat) :
	IDEEntry_2DFX(pIDEFormat, _2DFX_UNKNOWN_1),
	m_uiUnknown2(0),
	m_vecUnknown3{ 0.0f, 0.0f, 0.0f },
	m_uiUnknown4(0)
{
}

void			IDEEntry_2DFX_Unknown1::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();
	
	IDEEntry_2DFX::unserialize();
	
	m_uiUnknown2 = pDataReader->readTokenUint32();
	m_vecUnknown3 = pDataReader->readVector3D();
	m_uiUnknown4 = pDataReader->readTokenUint32();
}

void			IDEEntry_2DFX_Unknown1::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	IDEEntry_2DFX::serialize();

	pDataWriter->writeToken(m_uiUnknown2);
	pDataWriter->writeToken(m_vecUnknown3);
	pDataWriter->writeToken(m_uiUnknown4);
}