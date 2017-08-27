#include "CRWGeometryBoundingInfo.h"

using namespace bxgi;

CRWGeometryBoundingInfo::CRWGeometryBoundingInfo(void) :
	m_vecCenter{ 0.0f, 0.0f, 0.0f },
	m_fCenterRadius(0.0f),
	m_bHasPosition(false),
	m_bHasNormals(false)
{
}