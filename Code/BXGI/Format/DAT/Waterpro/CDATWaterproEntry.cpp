#include "CDATWaterproEntry.h"

using namespace bxgi;

CDATWaterproEntry::CDATWaterproEntry(void) :
	m_fWaterLevel(0.0f)
{
	m_vecWaterZoneStart.x = 0.0f;
	m_vecWaterZoneStart.y = 0.0f;
	m_vecWaterZoneEnd.x = 0.0f;
	m_vecWaterZoneEnd.y = 0.0f;
}