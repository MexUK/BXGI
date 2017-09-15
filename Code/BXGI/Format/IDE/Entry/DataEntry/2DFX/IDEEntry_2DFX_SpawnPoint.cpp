#include "IDEEntry_2DFX_SpawnPoint.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/IDE/IDEFormat.h"

using namespace bxcf;
using namespace bxgi;

void			IDEEntry_2DFX_SpawnPoint::unserialize(void)
{
	DataReader *pDataReader = &m_pFormat->m_reader;
	
	IDEEntry_2DFX::unserialize();
	
	m_vecFlags.x = pDataReader->readTokenUint32();
	m_vecFlags.y = pDataReader->readTokenUint32();
	m_vecFlags.z = pDataReader->readTokenUint32();
	m_vecFlags.w = pDataReader->readTokenUint32();
}

void			IDEEntry_2DFX_SpawnPoint::serialize(void)
{
	DataWriter *pDataWriter = &m_pFormat->m_writer;

	IDEEntry_2DFX::serialize();

	pDataWriter->writeToken(m_vecFlags.x);
	pDataWriter->writeToken(m_vecFlags.y);
	pDataWriter->writeToken(m_vecFlags.z);
	pDataWriter->writeToken(m_vecFlags.w);
}