#include "CIDEEntry_2DFX_Unknown1.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

CIDEEntry_2DFX_Unknown1::CIDEEntry_2DFX_Unknown1(CIDEFormat *pIDEFormat) :
	CIDEEntry_2DFX(pIDEFormat, _2DFX_UNKNOWN_1),
	m_uiUnknown2(0),
	m_vecUnknown3{ 0.0f, 0.0f, 0.0f },
	m_uiUnknown4(0)
{
}

void			CIDEEntry_2DFX_Unknown1::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();
	
	CIDEEntry_2DFX::unserialize();
	
	m_uiUnknown2 = pDataReader->readTokenUint32();
	m_vecUnknown3 = pDataReader->readVector3D();
	m_uiUnknown4 = pDataReader->readTokenUint32();
}

void			CIDEEntry_2DFX_Unknown1::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	CIDEEntry_2DFX::serialize();

	pDataWriter->writeToken(m_uiUnknown2);
	pDataWriter->writeToken(m_vecUnknown3);
	pDataWriter->writeToken(m_uiUnknown4);
}