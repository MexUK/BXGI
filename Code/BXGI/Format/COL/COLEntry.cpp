#include "COLEntry.h"
#include "COLManager.h"
#include "COLFormat.h"
#include "Static/String.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Static/Debug.h"
#include "Exception/EExceptionCode.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

COLEntry::COLEntry(COLFormat *pCOLFile) : 
	m_pCOLFile(pCOLFile),
	m_uiCOLVersion(COL_UNKNOWN),
	m_uiFileSize(0),
	m_usModelId(0),

	m_uiCollisionLineCount(0),
	m_uiCollisionConeCount(0),
	m_uiCollisionSphereCount(0),
	m_uiCollisionBoxCount(0),
	m_uiCollisionMeshFaceCount(0),
	m_uiCollisionMeshVertexCount(0),
	m_uiCollisionMeshFaceGroupCount(0),
	m_uiFlags(0),
	m_uiCollisionLinesOffset(0),
	m_uiCollisionConesOffset(0),
	m_uiCollisionSpheresOffset(0),
	m_uiCollisionBoxesOffset(0),
	m_uiCollisionMeshVerticesOffset(0),
	m_uiCollisionMeshFacesOffset(0),
	m_uiTrianglePlanesOffset(0),
	m_uiCollisionMeshFaceGroupsOffset(0),

	m_uiShadowMeshVertexCount(0),
	m_uiShadowMeshFaceCount(0),
	m_uiShadowMeshVerticesOffset(0),
	m_uiShadowMeshFacesOffset(0),

	m_uiUnknown1(0),

	m_uiHeaderStartOffset(0)
{
}

// serialization
bool			COLEntry::unserialize(void)
{
	DataReader *pDataReader = &m_pCOLFile->m_reader;

	uint64 uiHeaderStartPosition = pDataReader->getSeek();
	setHeaderStartOffset((uint32)uiHeaderStartPosition);

	// COL 1 2 3 & 4 header
	string strVersionStamp = pDataReader->readString(4);
	
	string strFourZeros;
	for (uint32 i = 0; i < 4; i++)
	{
		strFourZeros.append(1, '\0');
	}

	if (strVersionStamp == strFourZeros)
	{
		// EOF
		return false;
	}
	setCOLVersion(COLManager::getCOLVersionFromFourCC(strVersionStamp));
	if (getCOLVersion() == COL_UNKNOWN)
	{
		throw EXCEPTION_UNKNOWN_FORMAT;
	}
	uint32 uiEntrySize = pDataReader->readUint32(); // entry size from after this value
	setModelName(String::rtrimFromLeft(pDataReader->readString(22)));
	setModelId(pDataReader->readUint16());
	parseBoundingObjects();

	ECOLVersion ECOLVersionValue = getCOLVersion();
	if (ECOLVersionValue == COL_1)
	{
		// COL 1 header & body

		// read collision sphere count
		setCollisionSphereCount(pDataReader->readUint32());

		// read collision spheres array
		setCollisionSpheresOffset((uint32)(pDataReader->getSeek()));
		parseCollisionSpheres();

		// read unknown1 count
		setUnknown1(pDataReader->readUint32()); // number of unknown data (0)

		// read collision box count
		setCollisionBoxCount(pDataReader->readUint32());

		// read collision boxes array
		setCollisionBoxesOffset((uint32)(pDataReader->getSeek()));
		parseCollisionBoxes();

		// read collision mesh vertex count
		setCollisionMeshVertexCount(pDataReader->readUint32());

		// read collision mesh vertices array
		setCollisionMeshVerticesOffset((uint32)(pDataReader->getSeek()));
		parseCollisionMeshVertices();

		// read collision mesh face count
		setCollisionMeshFaceCount(pDataReader->readUint32());

		// read collision mesh faces array
		setCollisionMeshFacesOffset((uint32)(pDataReader->getSeek()));
		parseCollisionMeshFaces();
	}
	else
	{
		// COL 2 3 & 4 header
		setCollisionSphereCount(pDataReader->readUint16());
		setCollisionBoxCount(pDataReader->readUint16());
		setCollisionMeshFaceCount(pDataReader->readUint16());
		uint8 uiLineOrConeCount = pDataReader->readUint8();
		uint8 uiPadding1 = pDataReader->readUint8(); // 1 byte padding
		setFlags(pDataReader->readUint32());

		if (doesUseCones())
		{
			setCollisionConeCount(uiLineOrConeCount);
		}
		else
		{
			setCollisionLineCount(uiLineOrConeCount);
		}

		setCollisionSpheresOffset(pDataReader->readUint32());
		setCollisionBoxesOffset(pDataReader->readUint32());
		if (doesUseCones())
		{
			setCollisionConesOffset(pDataReader->readUint32());
		}
		else
		{
			setCollisionLinesOffset(pDataReader->readUint32());
		}
		setCollisionMeshVerticesOffset(pDataReader->readUint32());
		setCollisionMeshFacesOffset(pDataReader->readUint32());
		setTrianglePlanesOffset(pDataReader->readUint32());

		if ((getFlags() & 8) != 0)
		{
			// entry does have face groups
			uint64 uiCurrentSeek = pDataReader->getSeek();

			// read face group count
			uint32 uiFaceGroupCountSeek = (getHeaderStartOffset() + 4 + getCollisionMeshFacesOffset()) - 4;
			pDataReader->setSeek(uiFaceGroupCountSeek);
			setCollisionMeshFaceGroupCount(pDataReader->readUint32());

			// read face groups offset
			setCollisionMeshFaceGroupsOffset((getCollisionMeshFacesOffset() - 4) - (28 * getCollisionMeshFaceGroupCount())); // 28 = sizeof(TFaceGroup)

			// restore parser seek position
			pDataReader->setSeek(uiCurrentSeek);
		}
		else
		{
			// entry does not have face groups
			setCollisionMeshFaceGroupCount(0);
			setCollisionMeshFaceGroupsOffset(0);
		}

		if (ECOLVersionValue == COL_3 || ECOLVersionValue == COL_4)
		{
			// COL 3 & 4 header
			setShadowMeshFaceCount(pDataReader->readUint32());
			setShadowMeshVerticesOffset(pDataReader->readUint32());
			setShadowMeshFacesOffset(pDataReader->readUint32());

			if (ECOLVersionValue == COL_4)
			{
				// COL 4 header
				setUnknown1(pDataReader->readUint32());
			}
		}

		// calculate vertex counts

		// COL 2 3 & 4
		uint32 uiCollisionMeshVertexCount = calculateCollisionMeshVertexCount();
		setCollisionMeshVertexCount(uiCollisionMeshVertexCount);

		if (ECOLVersionValue == COL_3 || ECOLVersionValue == COL_4)
		{
			// COL 3 & 4
			uint32 uiShadowMeshVertexCount = calculateShadowMeshVertexCount();
			setShadowMeshVertexCount(uiShadowMeshVertexCount);
		}

		// COL 2 3 & 4 body
		pDataReader->setSeek(getHeaderStartOffset() + 4 + getCollisionSpheresOffset());
		parseCollisionSpheres();

		pDataReader->setSeek(getHeaderStartOffset() + 4 + getCollisionBoxesOffset());
		parseCollisionBoxes();

		if (doesUseCones())
		{
			pDataReader->setSeek(getHeaderStartOffset() + 4 + getCollisionConesOffset());
			parseCollisionCones();
		}
		else
		{
			pDataReader->setSeek(getHeaderStartOffset() + 4 + getCollisionLinesOffset());
			parseCollisionLines();
		}

		pDataReader->setSeek(getHeaderStartOffset() + 4 + getCollisionMeshVerticesOffset());
		parseCollisionMeshVertices();

		pDataReader->setSeek(getHeaderStartOffset() + 4 + getCollisionMeshFaceGroupsOffset());
		parseCollisionMeshFaceGroups();

		pDataReader->setSeek(getHeaderStartOffset() + 4 + getCollisionMeshFacesOffset());
		parseCollisionMeshFaces();

		if (ECOLVersionValue == COL_3 || ECOLVersionValue == COL_4)
		{
			// COL 3 & 4 body
			pDataReader->setSeek(getHeaderStartOffset() + 4 + getShadowMeshVerticesOffset());
			parseShadowMeshVertices();

			pDataReader->setSeek(getHeaderStartOffset() + 4 + getShadowMeshFacesOffset());
			parseShadowMeshFaces();
		}
	}

	if (!pDataReader->canSeekTo(getHeaderStartOffset() + uiEntrySize + 8))
	{
		// EOF
		return false;
	}

	pDataReader->setSeek(getHeaderStartOffset() + uiEntrySize + 8);
	return true;
}

void			COLEntry::serialize(void)
{
	DataWriter *pDataWriter = &m_pCOLFile->m_writer;
	ECOLVersion ECOLVersionValue = getCOLVersion();

	// COL 1 2 3 & 4 header
	pDataWriter->writeString(COLManager::getFourCCFromCOLVersion(ECOLVersionValue));
	pDataWriter->writeUint32(calculateEntrySizeForPacking());
	pDataWriter->writeStringRef(m_strModelName, 22);
	pDataWriter->writeUint16(m_usModelId);
	storeBoundingObjects();

	if (ECOLVersionValue != COL_1)
	{
		// COL 2 3 & 4 header
		serializeHeader_Versions2_3_4();
	}

	// entry body
	if (ECOLVersionValue == COL_1)
	{
		// COL 1 body
		serializeBody_Version1();
	}
	else
	{
		// COL 2 3 & 4 body
		serializeBody_Versions2_3_4();
	}
}

void			COLEntry::serializeHeader_Versions2_3_4(void)
{
	DataWriter *pDataWriter = &m_pCOLFile->m_writer;
	ECOLVersion ECOLVersionValue = getCOLVersion();

	// calculate collision spheres array offset
	uint32 uiCollisionSpheresOffset = COLManager::getEntryHeaderSizeForPacking(ECOLVersionValue) + 4;

	// calculate collision boxes array offset
	uint32 uiCollisionBoxesOffset = uiCollisionSpheresOffset + m_uiCollisionSphereCount;

	// calculate collision cones array offset
	uint32 uiCollisionConesOrLinesOffset = uiCollisionBoxesOffset + m_uiCollisionBoxCount;

	// calculate collision mesh vertices array offset
	uint32 uiCollisionMeshVerticesOffset = uiCollisionConesOrLinesOffset + m_uiCollisionConeCount;

	// allow for potential padding
	uint32 uiNextPropertyOffset = 0;
	if (((m_uiCollisionMeshVertexCount * 6) % 4) != 0)
	{
		uiNextPropertyOffset += 2; // 2 bytes padding
	}

	// calculate collision mesh face groups array offset
	uint32 uiCollisionMeshFaceGroupsOffset = uiCollisionMeshVerticesOffset + m_uiCollisionMeshVertexCount + uiNextPropertyOffset;

	// calculate collision mesh faces array offset
	uint32 uiCollisionMeshFacesOffset;
	if (m_uiFlags & 8)
	{
		// does have face groups
		uint32 uiCollisionMeshFaceGroupCountOffset = uiCollisionMeshFaceGroupsOffset + m_uiCollisionMeshFaceGroupCount;
		uiCollisionMeshFacesOffset = uiCollisionMeshFaceGroupCountOffset + 4;
	}
	else
	{
		// doesn't have face groups
		uiCollisionMeshFacesOffset = uiCollisionMeshFaceGroupsOffset;
	}

	// calculate triangle planes array offset
	uint32 uiTrianglePlanesOffset = 0;

	uint32 uiShadowMeshVerticesOffset = 0;
	uint32 uiShadowMeshFacesOffset = 0;
	if (ECOLVersionValue == COL_3 || ECOLVersionValue == COL_4)
	{
		// COL 3 & 4 header

		// calculate shadow mesh vertices array offset
		uiShadowMeshVerticesOffset = uiCollisionMeshFacesOffset + m_uiCollisionMeshFaceCount;

		// allow for potential padding
		uint32 uiNextPropertyOffset2 = 0;
		if (((m_uiShadowMeshVertexCount * 6) % 4) != 0)
		{
			uiNextPropertyOffset2 += 2; // 2 bytes padding
		}

		// calculate shadow mesh faces array offset
		uiShadowMeshFacesOffset = uiShadowMeshVerticesOffset + m_uiShadowMeshVertexCount + uiNextPropertyOffset2;
	}

	setCollisionSpheresOffset(uiCollisionSpheresOffset);
	setCollisionBoxesOffset(uiCollisionBoxesOffset);
	if (doesUseCones())
	{
		setCollisionConesOffset(uiCollisionConesOrLinesOffset);
	}
	else
	{
		setCollisionLinesOffset(uiCollisionConesOrLinesOffset);
	}
	setCollisionMeshVerticesOffset(uiCollisionMeshVerticesOffset);
	setCollisionMeshFacesOffset(uiCollisionMeshFacesOffset);
	setTrianglePlanesOffset(uiTrianglePlanesOffset);
	setShadowMeshVerticesOffset(uiShadowMeshVerticesOffset);
	setShadowMeshFacesOffset(uiShadowMeshFacesOffset);

	// COL 2 3 & 4 header
	pDataWriter->writeUint16((uint16)m_uiCollisionSphereCount);
	pDataWriter->writeUint16((uint16)m_uiCollisionBoxCount);
	pDataWriter->writeUint16((uint16)m_uiCollisionMeshFaceCount);
	if (doesUseCones())
	{
		pDataWriter->writeUint8((uint8)m_uiCollisionConeCount);
	}
	else
	{
		pDataWriter->writeUint8((uint8)m_uiCollisionLineCount);
	}
	pDataWriter->writeUint8(0); // 1 byte padding
	pDataWriter->writeUint32(getFlagsForPacking());
	pDataWriter->writeUint32(uiCollisionSpheresOffset);
	pDataWriter->writeUint32(uiCollisionBoxesOffset);
	pDataWriter->writeUint32(uiCollisionConesOrLinesOffset);
	pDataWriter->writeUint32(uiCollisionMeshVerticesOffset);
	pDataWriter->writeUint32(uiCollisionMeshFacesOffset);
	pDataWriter->writeUint32(uiTrianglePlanesOffset);

	if (ECOLVersionValue == COL_3 || ECOLVersionValue == COL_4)
	{
		// COL 3 & 4 header
		pDataWriter->writeUint32(m_uiShadowMeshFaceCount);
		pDataWriter->writeUint32(uiShadowMeshVerticesOffset);
		pDataWriter->writeUint32(uiShadowMeshFacesOffset);

		if (ECOLVersionValue == COL_4)
		{
			// COL 4 header
			pDataWriter->writeUint32(m_uiUnknown1);
		}
	}
}

void			COLEntry::serializeBody_Version1(void)
{
	DataWriter *pDataWriter = &m_pCOLFile->m_writer;

	pDataWriter->writeUint32(m_uiCollisionSphereCount);
	storeCollisionSpheres();

	pDataWriter->writeUint32(0); // number of unknown data (0)

	pDataWriter->writeUint32(m_uiCollisionBoxCount);
	storeCollisionBoxes();

	pDataWriter->writeUint32(m_uiCollisionMeshVertexCount);
	storeCollisionMeshVertices();

	pDataWriter->writeUint32(m_uiCollisionMeshFaceCount);
	storeCollisionMeshFaces();
}

void			COLEntry::serializeBody_Versions2_3_4(void)
{
	DataWriter *pDataWriter = &m_pCOLFile->m_writer;
	ECOLVersion ECOLVersionValue = getCOLVersion();

	storeCollisionSpheres();
	storeCollisionBoxes();
	if (doesUseCones())
	{
		storeCollisionCones();
	}
	else
	{
		storeCollisionLines();
	}
	storeCollisionMeshVertices();

	if (((m_uiCollisionMeshVertexCount * 6) % 4) != 0)
	{
		pDataWriter->writeString(2); // 2 bytes padding
	}

	if (m_uiFlags & 8) // has face groups
	{
		storeCollisionMeshFaceGroups();
		pDataWriter->writeUint32(m_uiCollisionMeshFaceGroupCount);
	}

	storeCollisionMeshFaces();

	if (ECOLVersionValue == COL_3 || ECOLVersionValue == COL_4)
	{
		// COL 3 & 4 body
		storeShadowMeshVertices();

		if (((m_uiShadowMeshVertexCount * 6) % 4) != 0)
		{
			pDataWriter->writeString(2); // 2 bytes padding
		}

		storeShadowMeshFaces();
	}
}

uint32			COLEntry::calculateEntrySizeForPacking()
{
	ECOLVersion ECOLVersionValue = getCOLVersion();

	if (ECOLVersionValue == COL_1)
	{
		return COLManager::getEntryHeaderSizeForPacking(COL_1)
			+ (5 * 4) // 5 uint32s
			+ m_uiCollisionSphereCount
			+ m_uiCollisionBoxCount
			+ m_uiCollisionMeshVertexCount
			+ m_uiCollisionMeshFaceCount
			;
	}
	else
	{
		uint32 uiEntrySizeForPacking = COLManager::getEntryHeaderSizeForPacking(ECOLVersionValue)
			+ m_uiCollisionSphereCount
			+ m_uiCollisionBoxCount
			+ m_uiCollisionMeshVertexCount
			+ m_uiCollisionMeshFaceCount
			;
		if (((m_uiCollisionMeshVertexCount * 6) % 4) != 0)
		{
			uiEntrySizeForPacking += 2;
		}
		if ((m_uiFlags & 8) != 0)
		{
			uiEntrySizeForPacking += m_uiCollisionMeshFaceGroupCount;
			uiEntrySizeForPacking += 4;
		}
		if (ECOLVersionValue == COL_3 || ECOLVersionValue == COL_4)
		{
			uiEntrySizeForPacking += m_uiShadowMeshVertexCount;
			uiEntrySizeForPacking += m_uiShadowMeshFaceCount;
			if (((m_uiShadowMeshVertexCount * 6) % 4) != 0)
			{
				uiEntrySizeForPacking += 2;
			}
		}
		return uiEntrySizeForPacking;
	}
}

// unserialize components
void					COLEntry::parseBoundingObjects(void)
{
	DataReader *pDataReader = &m_pCOLFile->m_reader;
	ECOLVersion ECOLVersionValue = getCOLVersion();
	TBounds& boundingObjects = getBoundingObjects();

	if (ECOLVersionValue == COL_1)
	{
		boundingObjects.m_fRadius = pDataReader->readFloat32();
		boundingObjects.m_vecCenter.x = pDataReader->readFloat32();
		boundingObjects.m_vecCenter.y = pDataReader->readFloat32();
		boundingObjects.m_vecCenter.z = pDataReader->readFloat32();
		boundingObjects.m_vecMin.x = pDataReader->readFloat32();
		boundingObjects.m_vecMin.y = pDataReader->readFloat32();
		boundingObjects.m_vecMin.z = pDataReader->readFloat32();
		boundingObjects.m_vecMax.x = pDataReader->readFloat32();
		boundingObjects.m_vecMax.y = pDataReader->readFloat32();
		boundingObjects.m_vecMax.z = pDataReader->readFloat32();
	}
	else
	{
		boundingObjects.m_vecMin.x = pDataReader->readFloat32();
		boundingObjects.m_vecMin.y = pDataReader->readFloat32();
		boundingObjects.m_vecMin.z = pDataReader->readFloat32();
		boundingObjects.m_vecMax.x = pDataReader->readFloat32();
		boundingObjects.m_vecMax.y = pDataReader->readFloat32();
		boundingObjects.m_vecMax.z = pDataReader->readFloat32();
		boundingObjects.m_vecCenter.x = pDataReader->readFloat32();
		boundingObjects.m_vecCenter.y = pDataReader->readFloat32();
		boundingObjects.m_vecCenter.z = pDataReader->readFloat32();
		boundingObjects.m_fRadius = pDataReader->readFloat32();
	}
}
void					COLEntry::parseCollisionLines(void)
{
	if (getCOLVersion() == COL_1)
	{
		return;
	}

	DataReader *pDataReader = &m_pCOLFile->m_reader;
	vector<TLine>& vecLines = getCollisionLines();

	for (uint32 i = 0, j = getCollisionLineCount(); i < j; i++)
	{
		TLine line;
		line.m_vecPosition1 = pDataReader->readVector3D();
		line.m_vecPosition2 = pDataReader->readVector3D();
		vecLines.push_back(line);
	}
}
void					COLEntry::parseCollisionCones(void)
{
	if (getCOLVersion() == COL_1)
	{
		return;
	}

	DataReader *pDataReader = &m_pCOLFile->m_reader;
	vector<TCone>& vecCones = getCollisionCones();

	for (uint32 i = 0, j = getCollisionConeCount(); i < j; i++)
	{
		TCone cone;
		cone.m_vecPosition = pDataReader->readVector3D();
		cone.m_fLength = pDataReader->readFloat32();
		cone.m_fRadius = pDataReader->readFloat32();
		vecCones.push_back(cone);
	}
}
void					COLEntry::parseCollisionSpheres(void)
{
	DataReader *pDataReader = &m_pCOLFile->m_reader;
	ECOLVersion ECOLVersionValue = getCOLVersion();
	vector<TSphere>& vecSpheres = getCollisionSpheres();

	for (uint32 i = 0, j = getCollisionSphereCount(); i < j; i++)
	{
		TSphere sphere;
		if (ECOLVersionValue == COL_1)
		{
			sphere.m_fRadius = pDataReader->readFloat32();
			sphere.m_vecCenter.x = pDataReader->readFloat32();
			sphere.m_vecCenter.y = pDataReader->readFloat32();
			sphere.m_vecCenter.z = pDataReader->readFloat32();
			sphere.m_surface.m_ucMaterial = pDataReader->readUint8();
			sphere.m_surface.m_ucFlag = pDataReader->readUint8();
			sphere.m_surface.m_ucBrightness = pDataReader->readUint8();
			sphere.m_surface.m_ucLight = pDataReader->readUint8();
		}
		else
		{
			sphere.m_vecCenter.x = pDataReader->readFloat32();
			sphere.m_vecCenter.y = pDataReader->readFloat32();
			sphere.m_vecCenter.z = pDataReader->readFloat32();
			sphere.m_fRadius = pDataReader->readFloat32();
			sphere.m_surface.m_ucMaterial = pDataReader->readUint8();
			sphere.m_surface.m_ucFlag = pDataReader->readUint8();
			sphere.m_surface.m_ucBrightness = pDataReader->readUint8();
			sphere.m_surface.m_ucLight = pDataReader->readUint8();
		}
		vecSpheres.push_back(sphere);
	}
}
void					COLEntry::parseCollisionBoxes(void)
{
	DataReader *pDataReader = &m_pCOLFile->m_reader;
	vector<TBox>& vecBoxes = getCollisionBoxes();

	for (uint32 i = 0, j = getCollisionBoxCount(); i < j; i++)
	{
		TBox box;
		box.m_min.x = pDataReader->readFloat32();
		box.m_min.y = pDataReader->readFloat32();
		box.m_min.z = pDataReader->readFloat32();
		box.m_max.x = pDataReader->readFloat32();
		box.m_max.y = pDataReader->readFloat32();
		box.m_max.z = pDataReader->readFloat32();
		box.m_surface.m_ucMaterial = pDataReader->readUint8();
		box.m_surface.m_ucFlag = pDataReader->readUint8();
		box.m_surface.m_ucBrightness = pDataReader->readUint8();
		box.m_surface.m_ucLight = pDataReader->readUint8();
		vecBoxes.push_back(box);
	}
}
void					COLEntry::parseCollisionMeshVertices(void)
{
	DataReader *pDataReader = &m_pCOLFile->m_reader;
	ECOLVersion ECOLVersionValue = getCOLVersion();
	vector<TVertex>& vecVertices = getCollisionMeshVertices();

	for (uint32 i = 0, j = getCollisionMeshVertexCount(); i < j; i++)
	{
		TVertex vertex;
		if (ECOLVersionValue == COL_1)
		{
			vertex.x = pDataReader->readFloat32();
			vertex.y = pDataReader->readFloat32();
			vertex.z = pDataReader->readFloat32();
		}
		else
		{
			vertex.x = ((float32) pDataReader->readInt16()) / 128.0f;
			vertex.y = ((float32) pDataReader->readInt16()) / 128.0f;
			vertex.z = ((float32) pDataReader->readInt16()) / 128.0f;
		}
		vecVertices.push_back(vertex);
	}
}
void					COLEntry::parseCollisionMeshFaces(void)
{
	DataReader *pDataReader = &m_pCOLFile->m_reader;
	ECOLVersion ECOLVersionValue = getCOLVersion();
	vector<TFace>& vecFaces = getCollisionMeshFaces();

	for (uint32 i = 0, j = getCollisionMeshFaceCount(); i < j; i++)
	{
		TFace face;
		if (ECOLVersionValue == COL_1)
		{
			face.m_uiA = pDataReader->readUint32();
			face.m_uiB = pDataReader->readUint32();
			face.m_uiC = pDataReader->readUint32();
			face.m_surface.m_ucMaterial = pDataReader->readUint8();
			face.m_surface.m_ucFlag = pDataReader->readUint8();
			face.m_surface.m_ucBrightness = pDataReader->readUint8();
			face.m_surface.m_ucLight = pDataReader->readUint8();
		}
		else
		{
			face.m_uiA = pDataReader->readUint16();
			face.m_uiB = pDataReader->readUint16();
			face.m_uiC = pDataReader->readUint16();
			face.m_surface.m_ucMaterial = pDataReader->readUint8();
			face.m_surface.m_ucFlag = 0;
			face.m_surface.m_ucBrightness = 0;
			face.m_surface.m_ucLight = pDataReader->readUint8();
		}
		vecFaces.push_back(face);
	}
}
void					COLEntry::parseCollisionMeshFaceGroups(void)
{
	DataReader *pDataReader = &m_pCOLFile->m_reader;
	vector<TFaceGroup>& vecFaceGroups = getCollisionMeshFaceGroups();

	for (uint32 i = 0, j = getCollisionMeshFaceGroupCount(); i < j; i++)
	{
		TFaceGroup faceGroup;
		faceGroup.m_min.x = pDataReader->readFloat32();
		faceGroup.m_min.y = pDataReader->readFloat32();
		faceGroup.m_min.z = pDataReader->readFloat32();
		faceGroup.m_max.x = pDataReader->readFloat32();
		faceGroup.m_max.y = pDataReader->readFloat32();
		faceGroup.m_max.z = pDataReader->readFloat32();
		faceGroup.m_startFace = pDataReader->readUint16();
		faceGroup.m_endFace = pDataReader->readUint16();
		vecFaceGroups.push_back(faceGroup);
	}
}

void					COLEntry::parseShadowMeshVertices(void)
{
	DataReader *pDataReader = &m_pCOLFile->m_reader;
	vector<TVertex>& vecVertices = getShadowMeshVertices();

	for (uint32 i = 0, j = getShadowMeshVertexCount(); i < j; i++)
	{
		TVertex vertex;
		vertex.x = ((float32) pDataReader->readInt16()) / 128.0f;
		vertex.y = ((float32) pDataReader->readInt16()) / 128.0f;
		vertex.z = ((float32) pDataReader->readInt16()) / 128.0f;
		vecVertices.push_back(vertex);
	}
}
void					COLEntry::parseShadowMeshFaces(void)
{
	DataReader *pDataReader = &m_pCOLFile->m_reader;
	vector<TFace>& vecFaces = getShadowMeshFaces();

	for (uint32 i = 0, j = getShadowMeshFaceCount(); i < j; i++)
	{
		TFace face;
		face.m_uiA = pDataReader->readUint16();
		face.m_uiB = pDataReader->readUint16();
		face.m_uiC = pDataReader->readUint16();
		face.m_surface.m_ucMaterial = pDataReader->readUint8();
		face.m_surface.m_ucFlag = 0;
		face.m_surface.m_ucBrightness = 0;
		face.m_surface.m_ucLight = pDataReader->readUint8();
		vecFaces.push_back(face);
	}
}

// serialize components
void				COLEntry::storeBoundingObjects(void)
{
	DataWriter *pDataWriter = &m_pCOLFile->m_writer;
	ECOLVersion ECOLVersionValue = getCOLVersion();

	TBounds boundingObjects = getBoundingObjects();
	if (ECOLVersionValue == COL_1)
	{
		pDataWriter->writeFloat32(boundingObjects.m_fRadius);
		pDataWriter->writeVector3D(boundingObjects.m_vecCenter);
		pDataWriter->writeVector3D(boundingObjects.m_vecMin);
		pDataWriter->writeVector3D(boundingObjects.m_vecMax);
	}
	else
	{
		pDataWriter->writeVector3D(boundingObjects.m_vecMin);
		pDataWriter->writeVector3D(boundingObjects.m_vecMax);
		pDataWriter->writeVector3D(boundingObjects.m_vecCenter);
		pDataWriter->writeFloat32(boundingObjects.m_fRadius);
	}
}
void				COLEntry::storeCollisionSpheres(void)
{
	DataWriter *pDataWriter = &m_pCOLFile->m_writer;
	ECOLVersion ECOLVersionValue = getCOLVersion();

	for (TSphere& sphere : getCollisionSpheres())
	{
		if (ECOLVersionValue == COL_1)
		{
			pDataWriter->writeFloat32(sphere.m_fRadius);
			pDataWriter->writeVector3D(sphere.m_vecCenter);
			pDataWriter->writeUint8(sphere.m_surface.m_ucMaterial);
			pDataWriter->writeUint8(sphere.m_surface.m_ucFlag);
			pDataWriter->writeUint8(sphere.m_surface.m_ucBrightness);
			pDataWriter->writeUint8(sphere.m_surface.m_ucLight);
		}
		else
		{
			pDataWriter->writeVector3D(sphere.m_vecCenter);
			pDataWriter->writeFloat32(sphere.m_fRadius);
			pDataWriter->writeUint8(sphere.m_surface.m_ucMaterial);
			pDataWriter->writeUint8(sphere.m_surface.m_ucFlag);
			pDataWriter->writeUint8(sphere.m_surface.m_ucBrightness);
			pDataWriter->writeUint8(sphere.m_surface.m_ucLight);
		}
	}
}
void				COLEntry::storeCollisionLines(void)
{
	DataWriter *pDataWriter = &m_pCOLFile->m_writer;

	for (TLine& line : getCollisionLines())
	{
		pDataWriter->writeVector3D(line.m_vecPosition1);
		pDataWriter->writeVector3D(line.m_vecPosition2);
	}
}
void				COLEntry::storeCollisionCones(void)
{
	DataWriter *pDataWriter = &m_pCOLFile->m_writer;

	for (TCone& cone : getCollisionCones())
	{
		pDataWriter->writeVector3D(cone.m_vecPosition);
		pDataWriter->writeFloat32(cone.m_fLength);
		pDataWriter->writeFloat32(cone.m_fRadius);
	}
}
void				COLEntry::storeCollisionBoxes(void)
{
	DataWriter *pDataWriter = &m_pCOLFile->m_writer;

	for (TBox& box : getCollisionBoxes())
	{
		pDataWriter->writeVector3D(box.m_min);
		pDataWriter->writeVector3D(box.m_max);
		pDataWriter->writeUint8(box.m_surface.m_ucMaterial);
		pDataWriter->writeUint8(box.m_surface.m_ucFlag);
		pDataWriter->writeUint8(box.m_surface.m_ucBrightness);
		pDataWriter->writeUint8(box.m_surface.m_ucLight);
	}
}
void				COLEntry::storeCollisionMeshVertices(void)
{
	DataWriter *pDataWriter = &m_pCOLFile->m_writer;
	ECOLVersion ECOLVersionValue = getCOLVersion();

	for (TVertex& vertex : getCollisionMeshVertices())
	{
		if (ECOLVersionValue == COL_1)
		{
			pDataWriter->writeFloat32(vertex.x);
			pDataWriter->writeFloat32(vertex.y);
			pDataWriter->writeFloat32(vertex.z);
		}
		else
		{
			pDataWriter->writeUint16((uint16)(vertex.x));
			pDataWriter->writeUint16((uint16)(vertex.y));
			pDataWriter->writeUint16((uint16)(vertex.z));
		}
	}
}
void				COLEntry::storeCollisionMeshFaces(void)
{
	DataWriter *pDataWriter = &m_pCOLFile->m_writer;
	ECOLVersion ECOLVersionValue = getCOLVersion();

	for (TFace& face : getCollisionMeshFaces())
	{
		if (ECOLVersionValue == COL_1)
		{
			pDataWriter->writeUint32(face.m_uiA);
			pDataWriter->writeUint32(face.m_uiB);
			pDataWriter->writeUint32(face.m_uiC);
			pDataWriter->writeUint8(face.m_surface.m_ucMaterial);
			pDataWriter->writeUint8(face.m_surface.m_ucFlag);
			pDataWriter->writeUint8(face.m_surface.m_ucBrightness);
			pDataWriter->writeUint8(face.m_surface.m_ucLight);
		}
		else
		{
			pDataWriter->writeUint16((uint16)(face.m_uiA));
			pDataWriter->writeUint16((uint16)(face.m_uiB));
			pDataWriter->writeUint16((uint16)(face.m_uiC));
			pDataWriter->writeUint8(face.m_surface.m_ucMaterial);
			pDataWriter->writeUint8(face.m_surface.m_ucLight);
		}
	}
}
void				COLEntry::storeCollisionMeshFaceGroups(void)
{
	DataWriter *pDataWriter = &m_pCOLFile->m_writer;

	for (TFaceGroup& faceGroup : getCollisionMeshFaceGroups())
	{
		pDataWriter->writeVector3D(faceGroup.m_min);
		pDataWriter->writeVector3D(faceGroup.m_max);
		pDataWriter->writeUint16(faceGroup.m_startFace);
		pDataWriter->writeUint16(faceGroup.m_endFace);
	}
}
void				COLEntry::storeShadowMeshVertices(void)
{
	DataWriter *pDataWriter = &m_pCOLFile->m_writer;

	for (TVertex& vertex : getShadowMeshVertices())
	{
		pDataWriter->writeUint16((uint16)vertex.x);
		pDataWriter->writeUint16((uint16)vertex.y);
		pDataWriter->writeUint16((uint16)vertex.z);
	}
}
void				COLEntry::storeShadowMeshFaces(void)
{
	DataWriter *pDataWriter = &m_pCOLFile->m_writer;

	for (TFace& face : getShadowMeshFaces())
	{
		pDataWriter->writeUint16((uint16)(face.m_uiA));
		pDataWriter->writeUint16((uint16)(face.m_uiB));
		pDataWriter->writeUint16((uint16)(face.m_uiC));
		pDataWriter->writeUint8(face.m_surface.m_ucMaterial);
		pDataWriter->writeUint8(face.m_surface.m_ucLight);
	}
}

uint32	COLEntry::getFlagsForPacking(void)
{
	ECOLVersion ECOLVersionValue = getCOLVersion();
	uint32 uiFlags = 0;
	if (getCollisionSphereCount() > 0 || getCollisionBoxCount() > 0 || getCollisionMeshVertexCount() > 0 || getCollisionMeshFaceCount() > 0)
	{
		uiFlags |= 2;
	}
	if (getCollisionMeshFaceGroupCount() > 0)
	{
		uiFlags |= 8;
	}
	if (ECOLVersionValue >= 3 && getShadowMeshFaceCount() > 0)
	{
		uiFlags |= 16;
	}
	return uiFlags;
}

uint32	COLEntry::calculateCollisionMeshVertexCount(void)
{
	DataReader *pDataReader = &m_pCOLFile->m_reader;
	ECOLVersion ECOLVersionValue = getCOLVersion();

	pDataReader->setSeek(getHeaderStartOffset() + 4 + getCollisionMeshFacesOffset());
	string strCollisionMeshFacesData = pDataReader->readString(getCollisionMeshFaceCount() * (ECOLVersionValue == 1 ? 16 : 8)); // 16 or 8 = sizeof(TFace)
	uint32 uiSeek = 0;
	uint32 uiHighestVertexIndex = 0;
	if (getCollisionMeshFaceCount() == 0)
	{
		return 0;
	}
	if (ECOLVersionValue == COL_1)
	{
		// COL 1
		for (uint32 i = 0, j = getCollisionMeshFaceCount(); i < j; i++)
		{
			for (uint8 i2 = 0; i2 < 3; i2++)
			{
				uint32 uiVertexIndex = String::unpackUint32(strCollisionMeshFacesData.substr(uiSeek, 4), false);
				if (uiVertexIndex > uiHighestVertexIndex)
				{
					uiHighestVertexIndex = uiVertexIndex;
				}
				uiSeek += 4; // 4 is sizeof(uint32)
			}
			uiSeek += 4; // 4 is sizeof(TSurface)
		}
	}
	else
	{
		// COL 2, 3 & 4
		for (uint32 i = 0, j = getCollisionMeshFaceCount(); i < j; i++)
		{
			for (uint8 i2 = 0; i2 < 3; i2++)
			{
				uint32 uiVertexIndex = String::unpackUint16(strCollisionMeshFacesData.substr(uiSeek, 2), false);
				if (uiVertexIndex > uiHighestVertexIndex)
				{
					uiHighestVertexIndex = uiVertexIndex;
				}
				uiSeek += 2; // 2 is sizeof(uint16)
			}
			uiSeek += 2; // 1 byte for material, 1 byte for light
		}
	}
	return uiHighestVertexIndex + 1;
}

uint32	COLEntry::calculateShadowMeshVertexCount(void)
{
	DataReader *pDataReader = &m_pCOLFile->m_reader;
	ECOLVersion ECOLVersionValue = getCOLVersion();

	//Debug::log("getHeaderStartOffset(): " + String::toString(getHeaderStartOffset()));
	//Debug::log("getShadowMeshFacesOffset(): " + String::toString(getShadowMeshFacesOffset()));
	//Debug::log("getShadowMeshFaceCount(): " + String::toString(getShadowMeshFaceCount()));
	//Debug::log("getCOLVersion(): " + String::toString(getCOLVersion()));
	pDataReader->setSeek(getHeaderStartOffset() + 4 + getShadowMeshFacesOffset());
	string strShadowMeshFacesData = pDataReader->readString(getShadowMeshFaceCount() * (ECOLVersionValue == 1 ? 16 : 8)); // 12 or 6 = sizeof(TVertex)
	//Debug::log("part 2 -- strShadowMeshFacesData.length(): " + String::toString(strShadowMeshFacesData.length()));
	uint32 uiSeek = 0;
	uint32 uiHighestVertexIndex = 0;
	if (getShadowMeshFaceCount() == 0)
	{
		return 0;
	}
	if (ECOLVersionValue == COL_1)
	{
		// COL 1
		for (uint32 i = 0, j = getShadowMeshFaceCount(); i < j; i++)
		{
			for (uint8 i2 = 0; i2 < 3; i2++)
			{
				uint32 uiVertexIndex = String::unpackUint32(strShadowMeshFacesData.substr(uiSeek, 4), false);
				if (uiVertexIndex > uiHighestVertexIndex)
				{
					uiHighestVertexIndex = uiVertexIndex;
				}
				uiSeek += 4; // 4 is sizeof(uint32)
			}
			uiSeek += 4; // 4 is sizeof(TSurface)
		}
	}
	else
	{
		// COL 2, 3 & 4
		for (uint32 i = 0, j = getShadowMeshFaceCount(); i < j; i++)
		{
			for (uint8 i2 = 0; i2 < 3; i2++)
			{
				uint32 uiVertexIndex = String::unpackUint16(strShadowMeshFacesData.substr(uiSeek, 2), false);
				if (uiVertexIndex > uiHighestVertexIndex)
				{
					uiHighestVertexIndex = uiVertexIndex;
				}
				uiSeek += 2; // 2 is sizeof(uint16)
			}
			uiSeek += 2; // 1 byte for material, 1 byte for light
		}
	}
	return uiHighestVertexIndex + 1;
}

// mesh vertex offset
void			COLEntry::applyCollisionMeshVerticesOffset(Vec3f vecAxisOffsets)
{
	for (Vec3f vecVertex : getCollisionMeshVertices())
	{
		vecVertex.x += vecAxisOffsets.x;
		vecVertex.y += vecAxisOffsets.y;
		vecVertex.z += vecAxisOffsets.z;
	}
}

// version support
bool			COLEntry::areLinesSupported(void)
{
	return true;
}

bool			COLEntry::areConesSupported(void)
{
	return m_uiCOLVersion >= 3;
}

bool			COLEntry::areFaceGroupsSupported(void)
{
	return m_uiCOLVersion >= 2;
}

bool			COLEntry::areShadowMeshesSupported(void)
{
	return m_uiCOLVersion >= 3;
}

bool			COLEntry::isLightIntensitySupported(void)
{
	return m_uiCOLVersion >= 2;
}

bool			COLEntry::isDataCompressionSupported(void)
{
	return m_uiCOLVersion >= 2;
}

bool			COLEntry::isClumpCollisionSupported(void)
{
	return m_uiCOLVersion == 3;
}

bool			COLEntry::doesUseCones(void)
{
	return (getFlags() & 1) == 1;
}

bool			COLEntry::doesUseFaceGroups(void)
{
	return (getFlags() & 8) == 8;
}