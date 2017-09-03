#include "CIDEEntry_2DFX_Ped.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

CIDEEntry_2DFX_Ped::CIDEEntry_2DFX_Ped(CIDEFormat *pIDEFormat) :
	CIDEEntry_2DFX(pIDEFormat, _2DFX_PED),
	m_uiBehaviour(0),
	m_vecUnknown2{ 0.0f, 0.0f, 0.0f },
	m_vecPedRotation{ 0.0f, 0.0f, 0.0f }
{
}

void			CIDEEntry_2DFX_Ped::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();
	
	CIDEEntry_2DFX::unserialize();
	
	m_uiBehaviour = pDataReader->readTokenUint32();
	m_vecUnknown2 = pDataReader->readVector3D();
	m_vecPedRotation = pDataReader->readVector3D();
}

void			CIDEEntry_2DFX_Ped::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	CIDEEntry_2DFX::serialize();

	pDataWriter->writeToken(m_uiBehaviour);
	pDataWriter->writeToken(m_vecUnknown2);
	pDataWriter->writeToken(m_vecPedRotation);
}