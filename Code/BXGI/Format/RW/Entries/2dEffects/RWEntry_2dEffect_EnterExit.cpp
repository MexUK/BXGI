#include "RWEntry_2dEffect_EnterExit.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Static/String2.h"

using namespace bxcf;
using namespace bxgi;

RWEntry_2dEffect_EnterExit::RWEntry_2dEffect_EnterExit(void) :
	_2dEffect(_2DFX_ENTER_EXIT),
	m_fEnterMarkerRotationAngle(0.0f),
	m_vecMarkerApporximationRadius({ 0.0f, 0.0f }),
	m_vecExitMarkerPosition({ 0.0f, 0.0f, 0.0f }),
	m_fExitMarkerAngleRotation(0.0f),
	m_iInteriorCount(0),
	m_iFlags(0),
	m_iSkyColour(0)
{
}

void							RWEntry_2dEffect_EnterExit::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();
	
	m_fEnterMarkerRotationAngle = pDataReader->readFloat32();
	m_vecMarkerApporximationRadius = pDataReader->readVector2D();
	m_vecExitMarkerPosition = pDataReader->readVector3D();
	m_fExitMarkerAngleRotation = pDataReader->readFloat32();
	m_iInteriorCount = pDataReader->readInt8();
	m_iFlags = pDataReader->readInt8();
	m_strInteriorName = String2::rtrimFromLeft(pDataReader->readString(8));
	m_iSkyColour = pDataReader->readInt8();
}

void							RWEntry_2dEffect_EnterExit::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();
	
	pDataWriter->writeFloat32(m_fEnterMarkerRotationAngle);
	pDataWriter->writeVector2D(m_vecMarkerApporximationRadius);
	pDataWriter->writeVector3D(m_vecExitMarkerPosition);
	pDataWriter->writeFloat32(m_fExitMarkerAngleRotation);
	pDataWriter->writeInt32(m_iInteriorCount);
	pDataWriter->writeInt32(m_iFlags);
	pDataWriter->writeStringRef(m_strInteriorName, 8);
	pDataWriter->writeInt32(m_iSkyColour);
}