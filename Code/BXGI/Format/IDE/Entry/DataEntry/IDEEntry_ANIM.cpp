#include "IDEEntry_ANIM.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

IDEEntry_ANIM::IDEEntry_ANIM(IDEFormat *pIDEFormat) :
	IDEEntry_Data(pIDEFormat, IDE_SECTION_ANIM),
	m_uiObjectId(0),
	m_fDrawDistance(0.0f),
	m_uiFlags(0),
	m_uiUnknown1(0),
	m_vecBoundingBoxLowerLeftCorner{ 0.0f, 0.0f, 0.0f },
	m_vecBoundingBoxUpperRightCorner{ 0.0f, 0.0f, 0.0f },
	m_vecBoundingSphereCenterPosition{ 0.0f, 0.0f, 0.0f },
	m_fBoundingSphereRadius(0.0f)
{
}

void			IDEEntry_ANIM::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();

	switch (pDataReader->getLineTokens().size())
	{
	case 6:
		// GTA SA
		setFormatType(0);
		setFormatGames(GAME_FLAG_GTA_SA);
		m_uiObjectId = pDataReader->readTokenUint32();
		m_strModelName = pDataReader->readTokenString();
		m_strTXDName = pDataReader->readTokenString();
		m_strAnimationName = pDataReader->readTokenString();
		m_fDrawDistance = pDataReader->readTokenFloat32();
		m_uiFlags = pDataReader->readTokenUint32();
		break;
	case 16:
		// GTA IV
		setFormatType(1);
		setFormatGames(GAME_FLAG_GTA_IV);
		m_strModelName = pDataReader->readTokenString();
		m_strTXDName = pDataReader->readTokenString();
		m_strAnimationName = pDataReader->readTokenString();
		m_fDrawDistance = pDataReader->readTokenFloat32();
		m_uiFlags = pDataReader->readTokenUint32();
		m_uiUnknown1 = pDataReader->readTokenUint32();
		m_vecBoundingBoxLowerLeftCorner = pDataReader->readVector3D();
		m_vecBoundingBoxUpperRightCorner = pDataReader->readVector3D();
		m_vecBoundingSphereCenterPosition = pDataReader->readVector3D();
		m_fBoundingSphereRadius = pDataReader->readTokenFloat32();
		m_strLODModel = pDataReader->readTokenString();
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}

void			IDEEntry_ANIM::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	switch (getFormatType())
	{
	case 0:
		pDataWriter->writeToken(m_strModelName);
		pDataWriter->writeToken(m_strTXDName);
		pDataWriter->writeToken(m_strAnimationName);
		pDataWriter->writeToken(m_fDrawDistance);
		pDataWriter->writeToken(m_uiFlags);
		pDataWriter->writeToken(m_uiUnknown1);
		pDataWriter->writeToken(m_vecBoundingBoxLowerLeftCorner);
		pDataWriter->writeToken(m_vecBoundingBoxUpperRightCorner);
		pDataWriter->writeToken(m_vecBoundingSphereCenterPosition);
		pDataWriter->writeToken(m_fBoundingSphereRadius);
		pDataWriter->writeToken(m_strLODModel);
		break;
	case 1:
		pDataWriter->writeToken(m_uiObjectId);
		pDataWriter->writeToken(m_strModelName);
		pDataWriter->writeToken(m_strTXDName);
		pDataWriter->writeToken(m_strAnimationName);
		pDataWriter->writeToken(m_fDrawDistance);
		pDataWriter->writeToken(m_uiFlags);
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}