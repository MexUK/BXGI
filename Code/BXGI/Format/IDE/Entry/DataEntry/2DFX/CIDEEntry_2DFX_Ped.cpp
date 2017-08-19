#include "CIDEEntry_2DFX_Ped.h"
#include "Exception/eExceptionCode.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

void			CIDEEntry_2DFX_Ped::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();
	
	CIDEEntry_2DFX::unserialize();
	
	m_uiBehaviour = pDataReader->readTokenUint32();
	m_vecUnknown2 = pDataReader->readVector3D();
	m_vecPedRotation = pDataReader->readVector3D();
}

void			CIDEEntry_2DFX_Ped::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();

	CIDEEntry_2DFX::serialize();

	pDataWriter->writeToken(m_uiBehaviour);
	pDataWriter->writeToken(m_vecUnknown2);
	pDataWriter->writeToken(m_vecPedRotation);
}