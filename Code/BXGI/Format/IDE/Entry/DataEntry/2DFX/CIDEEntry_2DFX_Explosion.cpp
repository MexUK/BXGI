#include "CIDEEntry_2DFX_Explosion.h"
#include "Exception/EExceptionCode.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

CIDEEntry_2DFX_Explosion::CIDEEntry_2DFX_Explosion(CIDEFormat *pIDEFormat) :
	CIDEEntry_2DFX(pIDEFormat, _2DFX_EXPLOSION),
	m_uiUnknown1(0),
	m_uiUnknown2(0),
	m_uiUnknown3(0),
	m_uiUnknown4(0)
{
}

void			CIDEEntry_2DFX_Explosion::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();
	
	CIDEEntry_2DFX::unserialize();
	
	m_uiUnknown1 = pDataReader->readTokenUint32();
	m_uiUnknown2 = pDataReader->readTokenUint32();
	m_uiUnknown3 = pDataReader->readTokenUint32();
	m_uiUnknown4 = pDataReader->readTokenUint32();
}

void			CIDEEntry_2DFX_Explosion::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();

	CIDEEntry_2DFX::serialize();

	pDataWriter->writeToken(m_uiUnknown1);
	pDataWriter->writeToken(m_uiUnknown2);
	pDataWriter->writeToken(m_uiUnknown3);
	pDataWriter->writeToken(m_uiUnknown4);
}