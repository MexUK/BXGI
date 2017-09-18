#include "IDEEntry_2DFX_Particle.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/IDE/IDEFormat.h"

using namespace bxcf;
using namespace bxgi;

IDEEntry_2DFX_Particle::IDEEntry_2DFX_Particle(IDEFormat *pIDEFormat) :
	IDEEntry_2DFX(pIDEFormat, _2DFX_PARTICLE),
	m_uiParticleType(0),
	m_fSize(0.0f),
	m_uiDestructionStageCount(0),
	m_uiDrawDistance(0),
	m_uiUnknown2(0),
	m_uiUnknown3(0)
{
	m_vecColour2.x = 0;
	m_vecColour2.y = 0;
	m_vecColour2.z = 0;
}

void			IDEEntry_2DFX_Particle::unserialize(void)
{
	DataReader *pDataReader = &m_pFormat->m_reader;
	
	IDEEntry_2DFX::unserialize();
	
	if(doesSupportFormatGame(GAME_FLAG_GTA_IV))
	{
		setFormatType(1);
		m_strParticleName = pDataReader->readTokenString();
		m_uiDestructionStageCount = pDataReader->readTokenUint32();
		m_uiUnknown2 = pDataReader->readTokenUint32();
		m_fSize = pDataReader->readTokenFloat32();
		m_uiDrawDistance = pDataReader->readTokenUint32();
		m_uiUnknown3 = pDataReader->readTokenUint32();
		m_vecColour2.x = pDataReader->readTokenUint32();
		m_vecColour2.y = pDataReader->readTokenUint32();
		m_vecColour2.z = pDataReader->readTokenUint32();
	}
	else
	{
		setFormatType(0);
		m_uiParticleType = pDataReader->readTokenUint32();
		m_vecStrength = pDataReader->readTokenVector3D();
		m_fSize = pDataReader->readTokenFloat32();
	}
}

void			IDEEntry_2DFX_Particle::serialize(void)
{
	DataWriter *pDataWriter = &m_pFormat->m_writer;
	
	IDEEntry_2DFX::serialize();

	switch (getFormatType())
	{
	case 0:
		pDataWriter->writeToken(m_strParticleName);
		pDataWriter->writeToken(m_uiDestructionStageCount);
		pDataWriter->writeToken(m_uiUnknown2);
		pDataWriter->writeToken(m_fSize);
		pDataWriter->writeToken(m_uiDrawDistance);
		pDataWriter->writeToken(m_uiUnknown3);
		pDataWriter->writeToken(m_vecColour2.x);
		pDataWriter->writeToken(m_vecColour2.y);
		pDataWriter->writeToken(m_vecColour2.z);
		break;
	case 1:
		pDataWriter->writeToken(m_uiParticleType);
		pDataWriter->writeToken(m_vecStrength);
		pDataWriter->writeToken(m_fSize);
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}