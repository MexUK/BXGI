#include "CGameFormat.h"
#include "Engine/RW/CRWManager.h"
#include "Engine/RAGE/CRageManager.h"
#include "Format/COL/CCOLManager.h"
#include "Static/String2.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;
using namespace bxcf::fileType;

// file extension
bool			CGameFormat::isKnownGameExtension(string& strFileExtension)
{
	return isModelExtension(strFileExtension)
		|| isTextureExtension(strFileExtension)
		|| isCollisionExtension(strFileExtension);
}

bool			CGameFormat::isModelExtension(string& strFileExtension)
{
	return CRWManager::isRWModelExtension(strFileExtension)
		|| CRageManager::isRageModelExtension(strFileExtension);
}

bool			CGameFormat::isTextureExtension(string& strFileExtension)
{
	return CRWManager::isRWTextureExtension(strFileExtension)
		|| CRageManager::isRageTextureExtension(strFileExtension);
}

bool			CGameFormat::isCollisionExtension(string& strFileExtension)
{
	return CCOLManager::isCollisionExtension(strFileExtension)
		|| CRageManager::isRageCollisionExtension(strFileExtension);
}

bool			CGameFormat::isAnimationExtension(string& strFileExtension)
{
	string strFileExtensionUpper = String2::toUpperCase(strFileExtension);
	return strFileExtensionUpper == "IFP";
}

// RW file type
EFileType		CGameFormat::getRWFileType(string& strFileExtension)
{
	if (CRWManager::isRWModelExtension(strFileExtension))
	{
		return MODEL;
	}
	else if (CRWManager::isRWTextureExtension(strFileExtension))
	{
		return TEXTURE;
	}
	else if (CCOLManager::isCollisionExtension(strFileExtension))
	{
		return COLLISION;
	}
	else if (isAnimationExtension(strFileExtension))
	{
		return ANIMATION;
	}
	else
	{
		return UNKNOWN;
	}
}