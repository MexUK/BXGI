#include "RWGeometryBoundingInfo.h"

using namespace bxgi;

RWGeometryBoundingInfo::RWGeometryBoundingInfo(void) :
	m_vecCenter{ 0.0f, 0.0f, 0.0f },
	m_fCenterRadius(0.0f),
	m_bHasPosition(false),
	m_bHasNormals(false)
{
}