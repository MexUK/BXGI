#include "IDEEntry_2DFX_Explosion.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

IDEEntry_2DFX_Explosion::IDEEntry_2DFX_Explosion(IDEFormat *pIDEFormat) :
	IDEEntry_2DFX(pIDEFormat, _2DFX_EXPLOSION),
	m_uiUnknown1(0),
	m_uiUnknown2(0),
	m_uiUnknown3(0),
	m_uiUnknown4(0)
{
}

void			IDEEntry_2DFX_Explosion::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();
	
	IDEEntry_2DFX::unserialize();
	
	m_uiUnknown1 = pDataReader->readTokenUint32();
	m_uiUnknown2 = pDataReader->readTokenUint32();
	m_uiUnknown3 = pDataReader->readTokenUint32();
	m_uiUnknown4 = pDataReader->readTokenUint32();
}

void			IDEEntry_2DFX_Explosion::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	IDEEntry_2DFX::serialize();

	pDataWriter->writeToken(m_uiUnknown1);
	pDataWriter->writeToken(m_uiUnknown2);
	pDataWriter->writeToken(m_uiUnknown3);
	pDataWriter->writeToken(m_uiUnknown4);
}