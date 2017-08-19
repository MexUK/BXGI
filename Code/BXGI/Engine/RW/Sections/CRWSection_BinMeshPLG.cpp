#include "CRWSection_BinMeshPLG.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"
#include "CRWSection_Geometry.h"

using namespace bxcf;
using namespace bxgi;

CRWSection_BinMeshPLG::CRWSection_BinMeshPLG(void) :
	m_uiFlags(0),
	m_uiMeshCount(0),
	m_uiIndexCount(0)
{
	setSectionId(RW_SECTION_BIN_MESH_PLG);
}

void							CRWSection_BinMeshPLG::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();

	m_uiFlags = pDataReader->readUint32();
	m_uiMeshCount = pDataReader->readUint32();
	m_uiIndexCount = pDataReader->readUint32();

	m_vecMeshes.resize(m_uiMeshCount);
	for (uint32 i = 0; i < m_uiMeshCount; i++)
	{
		CRWEntry_BinMeshPLG_Mesh *pRWEntry_BinMeshPLG_Mesh = new CRWEntry_BinMeshPLG_Mesh;
		m_vecMeshes[i] = pRWEntry_BinMeshPLG_Mesh;

		pRWEntry_BinMeshPLG_Mesh->setIndexCount(pDataReader->readUint32());
		pRWEntry_BinMeshPLG_Mesh->setMaterialIndex(pDataReader->readUint32());
		
		CRWSection_Geometry * pParentSection_Geometry = (CRWSection_Geometry *) getNextParentNodeWithSectionType(RW_SECTION_GEOMETRY);
		if (pParentSection_Geometry && (pParentSection_Geometry->getFlags() & 0x01000000))
		{
			// todo - pre instanced files that use platform OpenGL. See: http://www.gtamodding.com/wiki/Native_Data_PLG_(RW_Section)
			//if (platform == OpenGL)
			//{

			// todo - readBlahAsStdVector
			for (uint32 i2 = 0, j2 = pRWEntry_BinMeshPLG_Mesh->getIndexCount(); i2 < j2; i2++)
			{
				pRWEntry_BinMeshPLG_Mesh->getVertexIndices().push_back(pDataReader->readUint16());
			}

			//}
		}
		else
		{
			// todo - readBlahAsStdVector
			for (uint32 i2 = 0, j2 = pRWEntry_BinMeshPLG_Mesh->getIndexCount(); i2 < j2; i2++)
			{
				pRWEntry_BinMeshPLG_Mesh->getVertexIndices().push_back(pDataReader->readUint32());
			}
		}
	}
}

void							CRWSection_BinMeshPLG::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();

	pDataWriter->writeUint32(m_uiFlags);
	pDataWriter->writeUint32(m_uiMeshCount);
	pDataWriter->writeUint32(m_uiIndexCount);

	for (CRWEntry_BinMeshPLG_Mesh *pRWEntry_BinMeshPLG_Mesh : m_vecMeshes)
	{
		pDataWriter->writeUint32(pRWEntry_BinMeshPLG_Mesh->getIndexCount());
		pDataWriter->writeUint32(pRWEntry_BinMeshPLG_Mesh->getMaterialIndex());

		for (uint32& uiVertexIndex : pRWEntry_BinMeshPLG_Mesh->getVertexIndices())
		{
			pDataWriter->writeUint32(uiVertexIndex);
		}
	}
}