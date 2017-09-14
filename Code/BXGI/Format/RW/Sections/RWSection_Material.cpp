#include "RWSection_Material.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/RW/RWFormat.h"

using namespace bxcf;
using namespace bxgi;

RWSection_Material::RWSection_Material(void) :
	m_uiUnknown1(0),
	m_uiColour(0),
	m_uiUnknown2(0),
	m_uiUsesTextures(0),
	m_fAmbient(0.0f),
	m_fSpecular(0.0f),
	m_fDiffuse(0.0f)
{
	setSectionId(RW_SECTION_MATERIAL);
}

// serialization
void							RWSection_Material::_unserialize(void)
{
	DataReader *pDataReader = &m_pRWFormat->m_reader;

	m_uiUnknown1 = pDataReader->readUint32();
	m_uiColour = pDataReader->readUint32();
	m_uiUnknown2 = pDataReader->readUint32();
	m_uiUsesTextures = pDataReader->readUint32();
	m_fAmbient = pDataReader->readFloat32();
	m_fSpecular = pDataReader->readFloat32();
	m_fDiffuse = pDataReader->readFloat32();
}

void							RWSection_Material::_serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	pDataWriter->writeUint32(m_uiUnknown1);
	pDataWriter->writeUint32(m_uiColour);
	pDataWriter->writeUint32(m_uiUnknown2);
	pDataWriter->writeUint32(m_uiUsesTextures);
	pDataWriter->writeFloat32(m_fAmbient);
	pDataWriter->writeFloat32(m_fSpecular);
	pDataWriter->writeFloat32(m_fDiffuse);
}