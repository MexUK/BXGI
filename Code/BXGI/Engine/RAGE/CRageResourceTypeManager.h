#pragma once

#include "nsbxgi.h"
#include "Type/Types.h"
#include "Object/Manager.h"
#include "CRageResourceType.h"
#include "Pool/VectorPool.h"
#include <string>

class bxgi::CRageResourceTypeManager : public bxcf::Manager, public bxcf::VectorPool<bxgi::CRageResourceType*>
{
public:
	void											init(void);
	void											uninit(void);

	bxgi::CRageResourceType*						getResourceTypeByIdentifierOrFileExtension(uint32 uiResourceIdentifer, std::string strFileExtension);
	bxgi::CRageResourceType*						getResourceTypeByIdentifier(uint32 uiResourceIdentifer);
	bxgi::CRageResourceType*						getResourceTypeByFileExtension(std::string strFileExtension);

private:
	void											initRageResourceTypes(void);
};