#include "CIDEEntry_2DFX_SpawnPoint.h"
#include "Exception/EExceptionCode.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

void			CIDEEntry_2DFX_SpawnPoint::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();
	
	CIDEEntry_2DFX::unserialize();
	
	m_vecFlags.x = pDataReader->readTokenUint32();
	m_vecFlags.y = pDataReader->readTokenUint32();
	m_vecFlags.z = pDataReader->readTokenUint32();
	m_vecFlags.w = pDataReader->readTokenUint32();
}

void			CIDEEntry_2DFX_SpawnPoint::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();

	CIDEEntry_2DFX::serialize();

	pDataWriter->writeToken(m_vecFlags.x);
	pDataWriter->writeToken(m_vecFlags.y);
	pDataWriter->writeToken(m_vecFlags.z);
	pDataWriter->writeToken(m_vecFlags.w);
}