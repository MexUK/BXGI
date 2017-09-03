#include "CRWSection_2dEffect.h"
#include "Format/RW/Entries/C2dEffect.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Exception/EExceptionCode.h"
#include "Type/Vector/Vec3f.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

CRWSection_2dEffect::CRWSection_2dEffect(void)
{
	setSectionId(RW_SECTION_2D_EFFECT);
	m_p2dEffectsPool = new C2dEffectsPool;
}
CRWSection_2dEffect::~CRWSection_2dEffect(void)
{
	delete m_p2dEffectsPool;
}

void							CRWSection_2dEffect::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();

	uint32 uiEntryCount = pDataReader->readUint32();

	C2dEffect *p2dEffect = nullptr;
	for (uint32 i = 0; i < uiEntryCount; i++)
	{
		// read 2d effect header data
		Vec3f vecPosition = pDataReader->readVector3D();
		uint32 uiEntryType = pDataReader->readUint32();
		uint32 uiDataSize = pDataReader->readUint32();

		uint32 uiNextSeek = ((uint32)pDataReader->getSeek()) + uiDataSize; // todo - change getSeek() To uint32

		// create 2d effect
		p2dEffect = C2dEffect::createRW2dEffect((E2DFXType)uiEntryType);
		if (p2dEffect == nullptr)
		{
			throw EXCEPTION_UNKNOWN_ENTRY_TYPE;
		}
		get2dEffects()->addEntry(p2dEffect);

		// store 2d effect header data
		p2dEffect->setPosition(vecPosition);
		p2dEffect->set2DFXType((E2DFXType)uiEntryType);
		p2dEffect->setDataSize(uiDataSize);

		// read 2d effect body data
		p2dEffect->unserialize();

		// prepare next 2d effect
		pDataReader->setSeek(uiNextSeek);
	}
}

void							CRWSection_2dEffect::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	// 2d effect RW section header
	pDataWriter->writeUint32(get2dEffects()->getEntryCount());

	// 2d effect entries
	for (C2dEffect* p2dEffect : get2dEffects()->getEntries())
	{
		// 2d effect header
		pDataWriter->writeVector3D(p2dEffect->getPosition());
		pDataWriter->writeUint32(p2dEffect->get2DFXType());
		pDataWriter->writeUint32(p2dEffect->getDataSize());

		// 2d effect body
		p2dEffect->serialize();
	}
}