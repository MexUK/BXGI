#include "IDEEntry_2DFX_Ped.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/IDE/IDEFormat.h"

using namespace bxcf;
using namespace bxgi;

IDEEntry_2DFX_Ped::IDEEntry_2DFX_Ped(IDEFormat *pIDEFormat) :
	IDEEntry_2DFX(pIDEFormat, _2DFX_PED),
	m_uiBehaviour(0),
	m_vecUnknown2{ 0.0f, 0.0f, 0.0f },
	m_vecPedRotation{ 0.0f, 0.0f, 0.0f }
{
}

void			IDEEntry_2DFX_Ped::unserialize(void)
{
	DataReader *pDataReader = &m_pFormat->m_reader;
	
	IDEEntry_2DFX::unserialize();
	
	m_uiBehaviour = pDataReader->readTokenUint32();
	m_vecUnknown2 = pDataReader->readTokenVector3D();
	m_vecPedRotation = pDataReader->readTokenVector3D();
}

void			IDEEntry_2DFX_Ped::serialize(void)
{
	DataWriter *pDataWriter = &m_pFormat->m_writer;

	IDEEntry_2DFX::serialize();

	pDataWriter->writeToken(m_uiBehaviour);
	pDataWriter->writeToken(m_vecUnknown2);
	pDataWriter->writeToken(m_vecPedRotation);
}