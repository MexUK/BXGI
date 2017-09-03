#include "CRWEntry_2dEffect_ParticleEffect.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Static/String2.h"

using namespace bxcf;
using namespace bxgi;

CRWEntry_2dEffect_ParticleEffect::CRWEntry_2dEffect_ParticleEffect(void) :
	C2dEffect(_2DFX_PARTICLE)
{
};

void							CRWEntry_2dEffect_ParticleEffect::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();
	
	m_strParticleEffectName = String2::rtrimFromLeft(pDataReader->readString(24));
}

void							CRWEntry_2dEffect_ParticleEffect::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();
	
	pDataWriter->writeStringRef(m_strParticleEffectName, 24);
}