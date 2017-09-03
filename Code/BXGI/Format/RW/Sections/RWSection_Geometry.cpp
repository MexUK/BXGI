#include "RWSection_Geometry.h"
#include "RWSection_String.h"
#include "Engine/RW/RWManager.h"
#include "Static/String.h"
#include "Intermediate/Model/Data/IntermediateGeometry.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Static/Debug.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

RWSection_Geometry::RWSection_Geometry(void) :
	m_uiFlags(0),
	m_ucUVCoordinateCount(0),
	m_ucGeometryNativeFlags(0),
	m_uiTriangleCount(0),
	m_uiVertexCount(0),
	m_uiFrameCount(0),
	m_uiAmbientColour(0),
	m_uiDiffuseColour(0),
	m_uiSpecularColour(0)
{
	setSectionId(RW_SECTION_GEOMETRY);
}

void							RWSection_Geometry::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();

	//uint32 uiTemp1 = pDataReader->getSeek();

	/*
	todo - also update some methods and remove old properties etc

	m_usFlags = pDataReader->readUint16();
	m_ucUVCoordinateCount = pDataReader->readUint8();
	if (m_usFlags & 4)
	{
		m_ucUVCoordinateCount = 1;
	}
	m_ucGeometryNativeFlags = pDataReader->readUint8();
	*/
	m_uiFlags = pDataReader->readUint32();
	m_uiTriangleCount = pDataReader->readUint32();
	m_uiVertexCount = pDataReader->readUint32();
	m_uiFrameCount = pDataReader->readUint32();

	if (RWVersion::unpackVersionStamp(m_uiSectionRWVersion) < 0x34000)
	{
		m_uiAmbientColour = pDataReader->readUint32();
		m_uiDiffuseColour = pDataReader->readUint32();
		m_uiSpecularColour = pDataReader->readUint32();
	}
	else
	{
		m_uiAmbientColour = 1;
		m_uiDiffuseColour = 1;
		m_uiSpecularColour = 1;
	}

	if ((m_uiFlags & 0x01000000) == 0)
	{
		if (m_uiFlags & 8)
		{
			m_vecVertexColours = pDataReader->readVector4ui8ArrayAsStdVector(m_uiVertexCount);
		}

		if (m_uiFlags & 4 || m_uiFlags & 0x80)
		{
			uint32 uiTextureSetCount = ((m_uiFlags & 0x00FF0000) >> 16) & 0xFF;
			if (uiTextureSetCount == 0)
			{
				if (m_uiFlags & 4)
				{
					uiTextureSetCount = 1;
				}
				else if (m_uiFlags & 0x80)
				{
					uiTextureSetCount = 2;
				}
			}
			for (uint32 i = 0; i < uiTextureSetCount; i++)
			{
				// todo - store multiple texture coordinates so data loss doesn't occur
				m_vecTextureCoordinates = pDataReader->readVector2DArrayAsStdVector(m_uiVertexCount);
			}
		}

		m_vecVertexIndices = pDataReader->readVector4ui16ArrayAsStdVector(m_uiTriangleCount);
	}

	for (uint32 uiMorphTargetIndex = 0; uiMorphTargetIndex < m_uiFrameCount; uiMorphTargetIndex++)
	{
		m_boundingInfo.setCenter(pDataReader->readVector3D());
		m_boundingInfo.setCenterRadius(pDataReader->readFloat32());
		m_boundingInfo.setHasPosition(pDataReader->readUint32() != 0);
		m_boundingInfo.setHasNormals(pDataReader->readUint32() != 0);

		if (/*(m_uiFlags & 2) || */m_boundingInfo.doesHavePosition() /* || true */) // todo
		{
			m_vecVertexPositions = pDataReader->readVector3DArrayAsStdVector(m_uiVertexCount);
		}

		if (/*(m_uiFlags & 16) || */m_boundingInfo.doesHaveNormals()) // todo
		{
			m_vecVertexNormals = pDataReader->readVector3DArrayAsStdVector(m_uiVertexCount);
		}
	}

	//uint32 uiTemp2 = pDataReader->getSeek();

	//uint32 uiByteCountRead = uiTemp2 - uiTemp1;
	//Debug::log("Geometry uiByteCountRead: " + String::toString(uiByteCountRead));
}

void							RWSection_Geometry::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	pDataWriter->writeUint32(m_uiFlags);
	pDataWriter->writeUint32(m_uiTriangleCount);
	pDataWriter->writeUint32(m_uiVertexCount);
	pDataWriter->writeUint32(m_uiFrameCount);

	uint32 uiRWVersionCC = RWManager::get()->getSerializationRWVersion();
	if (RWVersion::unpackVersionStamp(uiRWVersionCC) < 0x34000)
	{
		pDataWriter->writeUint32(m_uiAmbientColour);
		pDataWriter->writeUint32(m_uiDiffuseColour);
		pDataWriter->writeUint32(m_uiSpecularColour);
	}

	if ((m_uiFlags & 0x01000000) == 0)
	{
		if (m_uiFlags & 8)
		{
			pDataWriter->writeStdVector4ui8(m_vecVertexColours);
		}

		if (m_uiFlags & 4 || m_uiFlags & 0x80)
		{
			uint32 uiTextureSetCount = ((m_uiFlags & 0x00FF0000) >> 16) & 0xFF;
			if (uiTextureSetCount == 0)
			{
				if (m_uiFlags & 4)
				{
					uiTextureSetCount = 1;
				}
				else if (m_uiFlags & 0x80)
				{
					uiTextureSetCount = 2;
				}
			}
			for (uint32 i = 0; i < uiTextureSetCount; i++)
			{
				// todo - store multiple texture coordinates so data loss doesn't occur
				pDataWriter->writeStdVector2D(m_vecTextureCoordinates);
			}
		}
		// todo Debug::log("SEEK AFTER TEXTURE COORDINATES: " + String::toString((uint32)DataWriter::get()->getSeek()));
		pDataWriter->writeStdVector4ui16(m_vecVertexIndices);
	}

	for (uint32 uiMorphTargetIndex = 0; uiMorphTargetIndex < m_uiFrameCount; uiMorphTargetIndex++)
	{
		pDataWriter->writeVector3D(m_boundingInfo.getCenter());
		pDataWriter->writeFloat32(m_boundingInfo.getCenterRadius());
		pDataWriter->writeUint32(m_boundingInfo.doesHavePosition() ? 1 : 0);
		pDataWriter->writeUint32(m_boundingInfo.doesHaveNormals() ? 1 : 0);

		if (/*(m_uiFlags & 2) || */m_boundingInfo.doesHavePosition()) // todo
		{
			pDataWriter->writeStdVector3D(m_vecVertexPositions);
		}
		if (/*(m_uiFlags & 16) || */m_boundingInfo.doesHaveNormals()) // todo
		{
			pDataWriter->writeStdVector3D(m_vecVertexNormals);
		}
	}
}

string							RWSection_Geometry::getTextureDiffuseName(void)
{
	return ((RWSection_String*)getSectionsByType(RW_SECTION_TEXTURE)[0]->getSectionsByType(RW_SECTION_STRING)[0])->getData();
}

void							RWSection_Geometry::setVertexColours(vector<Vec4u8>& vecDayVertexColours)
{
	if (vecDayVertexColours.size() > 0)
	{
		m_uiFlags |= 8; // vertex colours flag
	}
	m_vecVertexColours.clear();
	m_vecVertexColours.resize(m_uiVertexCount);

	uint32 i = 0, j = vecDayVertexColours.size();
	if (j > m_uiVertexCount)
	{
		j = m_uiVertexCount;
	}
	for (; i < j; i++)
	{
		m_vecVertexColours[i] = vecDayVertexColours[i];
	}

	Vec4u8 vecColour;
	vecColour.x = 0xFF;
	vecColour.y = 0xFF;
	vecColour.z = 0xFF;
	vecColour.w = 0xFF;
	for (i = j, j = m_uiVertexCount; i < j; i++)
	{
		m_vecVertexColours[i] = vecColour;
	}
}

void							RWSection_Geometry::removePrelightning(void)
{
	m_uiFlags &= ~(1 << 3); // turn bit 4 off (flag 8)
	m_vecVertexColours.clear();
}

IntermediateGeometry*			RWSection_Geometry::convertToIntermediateGeometry(void)
{
	IntermediateGeometry *pGeneralGeometry = new IntermediateGeometry;

	Vec3f vecCenter;
	vecCenter.x = m_boundingInfo.getCenter().x;
	vecCenter.y = m_boundingInfo.getCenter().y;
	vecCenter.z = m_boundingInfo.getCenter().z;


	pGeneralGeometry->getBoundingObject()->setBoundingObjectType(BOUNDING_OBJECT_TYPE_SPHERE);
	pGeneralGeometry->getBoundingObject()->setCenter(vecCenter);
	pGeneralGeometry->getBoundingObject()->setRadius(m_boundingInfo.getCenterRadius());

	vector<Vec3f> vecVertexPositions = getVertexPositions();
	vector<Vec3f> vecVertexNormals = getVertexNormals();
	vector<Vec4u8> vecVertexColours = getVertexColours();
	vector<Vec2f> vecTextureUVCoordinates = getTextureCoordinates();

	if (m_uiFlags & 2)
	{
		pGeneralGeometry->setHasPositions(true);
	}
	if (m_uiFlags & 16)
	{
		pGeneralGeometry->setHasNormals(true);
	}
	if (m_uiFlags & 8)
	{
		pGeneralGeometry->setHasVertexColours(true);
	}
	if ((m_uiFlags & 4) || (m_uiFlags & 128))
	{
		pGeneralGeometry->setHasUVTextureCoordinates(true);
	}

	uint32 i = 0;
	for (Vec3f& vecPosition : vecVertexPositions)
	{
		IntermediateVertexData vertexData;
		if (m_uiFlags & 2)
		{
			vertexData.getPosition().x = vecPosition.x;
			vertexData.getPosition().y = vecPosition.y;
			vertexData.getPosition().z = vecPosition.z;
		}
		if (m_uiFlags & 16)
		{
			vertexData.getNormal().x = vecVertexNormals[i].x;
			vertexData.getNormal().y = vecVertexNormals[i].y;
			vertexData.getNormal().z = vecVertexNormals[i].z;
		}
		if (m_uiFlags & 8)
		{
			vertexData.setColour(vecVertexColours[i].convertToUint32());
		}
		if ((m_uiFlags & 4) || (m_uiFlags & 128))
		{
			vertexData.getTextureCoordinates().x = vecTextureUVCoordinates[i].x;
			vertexData.getTextureCoordinates().y = vecTextureUVCoordinates[i].y;
		}
		pGeneralGeometry->getVertices().push_back(vertexData);
		i++;
	}

	return pGeneralGeometry;
}