#include "CRWEntry_2dEffect_ParticleEffect.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"
#include "Static/CString2.h"

using namespace bxcf;
using namespace bxgi;

CRWEntry_2dEffect_ParticleEffect::CRWEntry_2dEffect_ParticleEffect(void) :
	C2dEffect(_2DFX_PARTICLE)
{
};

void							CRWEntry_2dEffect_ParticleEffect::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();
	
	m_strParticleEffectName = CString2::rtrimFromLeft(pDataReader->readString(24));
}

void							CRWEntry_2dEffect_ParticleEffect::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();
	
	pDataWriter->writeStringRef(m_strParticleEffectName, 24);
}