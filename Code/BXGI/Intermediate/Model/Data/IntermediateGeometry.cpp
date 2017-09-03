#include "IntermediateGeometry.h"

using namespace bxgi;

IntermediateGeometry::IntermediateGeometry(void) :
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
	m_pBoundingObject = new IntermediateBoundingObject;
	m_pBoundingObject->setBoundingObjectType(BOUNDING_OBJECT_TYPE_SPHERE);
	m_pBoundingObject->setCenter(vecCenter);
	m_pBoundingObject->setRadius(0.0f);
	m_pBoundingObject->setHasPositions(false);
	m_pBoundingObject->setHasNormals(false);
}