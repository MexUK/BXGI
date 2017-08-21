#ifndef CIntermediateGeometry_H
#define CIntermediateGeometry_H

#include "bxgi.h"
#include "Type/Types.h"
#include "CIntermediateBoundingObject.h"
#include "CIntermediateVertexData.h"
#include "CIntermediateIndexData.h"
#include <vector>

class bxgi::CIntermediateGeometry
{
public:
	CIntermediateGeometry(void) :
		m_pBoundingObject(nullptr),
		m_bHasVertexColours(false),
		m_bHasUVTextureCoordinates(false),
		m_bHasPositions(false),
		m_bHasNormals(false)
	{
		bxcf::Vec3f vecCenter;
		vecCenter.x = 0.0f;
		vecCenter.y = 0.0f;
		vecCenter.z = 0.0f;
		m_pBoundingObject = new bxgi::CIntermediateBoundingObject;
		m_pBoundingObject->setBoundingObjectType(bxgi::BOUNDING_OBJECT_TYPE_SPHERE);
		m_pBoundingObject->setCenter(vecCenter);
		m_pBoundingObject->setRadius(0.0f);
		m_pBoundingObject->setHasPositions(false);
		m_pBoundingObject->setHasNormals(false);
	}

	void									unload(void) {}

	void									setBoundingObject(bxgi::CIntermediateBoundingObject *pBoundingObject) { m_pBoundingObject = pBoundingObject; }
	bxgi::CIntermediateBoundingObject*			getBoundingObject(void) { return m_pBoundingObject; }

	void									setHasVertexColours(bool bHasVertexColours) { m_bHasVertexColours = bHasVertexColours; }
	bool									doesHaveVertexColours(void) { return m_bHasVertexColours; }

	void									setHasUVTextureCoordinates(bool bHasUVTextureCoordinates) { m_bHasUVTextureCoordinates = bHasUVTextureCoordinates; }
	bool									doesHaveUVTextureCoordinates(void) { return m_bHasUVTextureCoordinates; }

	void									setHasPositions(bool bHasPositions) { m_bHasPositions = bHasPositions; }
	bool									doesHavePositions(void) { return m_bHasPositions; }

	void									setHasNormals(bool bHasNormals) { m_bHasNormals = bHasNormals; }
	bool									doesHaveNormals(void) { return m_bHasNormals; }

	std::vector<bxgi::CIntermediateVertexData>&	getVertices(void) { return m_vecVertices; }

	std::vector<bxgi::CIntermediateIndexData>&	getTriangles(void) { return m_vecTriangles; }

private:
	bxgi::CIntermediateBoundingObject*			m_pBoundingObject;
	uint8									m_bHasVertexColours : 1;
	uint8									m_bHasUVTextureCoordinates : 1;
	uint8									m_bHasPositions : 1;
	uint8									m_bHasNormals : 1;
	std::vector<bxgi::CIntermediateVertexData>	m_vecVertices;
	std::vector<bxgi::CIntermediateIndexData>		m_vecTriangles;
};

#endif