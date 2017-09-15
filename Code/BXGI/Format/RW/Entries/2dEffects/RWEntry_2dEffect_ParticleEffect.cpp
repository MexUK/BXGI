#include "RWEntry_2dEffect_ParticleEffect.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Static/String.h"
#include "Format/RW/RWFormat.h"

using namespace bxcf;
using namespace bxgi;

RWEntry_2dEffect_ParticleEffect::RWEntry_2dEffect_ParticleEffect(void) :
	_2dEffect(_2DFX_PARTICLE)
{
};

void							RWEntry_2dEffect_ParticleEffect::unserialize(void)
{
	DataReader *pDataReader = &m_pRWFormat->m_reader;
	
	m_strParticleEffectName = String::rtrimFromLeft(pDataReader->readString(24));
}

void							RWEntry_2dEffect_ParticleEffect::serialize(void)
{
	DataWriter *pDataWriter = &m_pRWFormat->m_writer;
	
	pDataWriter->writeStringRef(m_strParticleEffectName, 24);
}