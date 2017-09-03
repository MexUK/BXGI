#include "RWEntry_2dEffect_ParticleEffect.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Static/String.h"

using namespace bxcf;
using namespace bxgi;

RWEntry_2dEffect_ParticleEffect::RWEntry_2dEffect_ParticleEffect(void) :
	_2dEffect(_2DFX_PARTICLE)
{
};

void							RWEntry_2dEffect_ParticleEffect::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();
	
	m_strParticleEffectName = String::rtrimFromLeft(pDataReader->readString(24));
}

void							RWEntry_2dEffect_ParticleEffect::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();
	
	pDataWriter->writeStringRef(m_strParticleEffectName, 24);
}