#include "IntermediateBoundingObject.h"

using namespace bxgi;

IntermediateBoundingObject::IntermediateBoundingObject(void) :
	m_uiBoundingObjectType(BOUNDING_OBJECT_TYPE_SPHERE),
	m_fRadius(0.0f),
	m_bHasPositions(false),
	m_bHasNormals(false)
{
	m_vecMin.x = 0.0f;
	m_vecMin.y = 0.0f;
	m_vecMin.z = 0.0f;
	m_vecMin.w = 0.0f;
	m_vecMax.x = 0.0f;
	m_vecMax.y = 0.0f;
	m_vecMax.z = 0.0f;
	m_vecMax.w = 0.0f;

	m_vecCenter.x = 0.0f;
	m_vecCenter.y = 0.0f;
	m_vecCenter.z = 0.0f;
}