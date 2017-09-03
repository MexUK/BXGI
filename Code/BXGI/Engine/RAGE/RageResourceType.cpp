#include "RageResourceType.h"

using namespace std;
using namespace bxgi;

string				RageResourceType::getResourceName(void)
{
	if (this == nullptr)
	{
		return "Unknown";
	}
	else
	{
		return m_strName;
	}
}

ERageResourceType	RageResourceType::getResourceId(void)
{
	if (this == nullptr)
	{
		return RAGE_UNKNOWN;
	}
	else
	{
		return m_uiResourceTypeId;
	}
}