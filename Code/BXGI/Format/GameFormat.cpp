#include "GameFormat.h"
#include "Engine/RW/RWManager.h"
#include "Engine/RAGE/RageManager.h"
#include "Format/COL/COLManager.h"
#include "Static/String.h"
#include "Game/EGame.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;
using namespace bxcf::fileType;

// file extension
bool			GameFormat::isKnownGameExtension(string& strFileExtension)
{
	return isModelExtension(strFileExtension)
		|| isTextureExtension(strFileExtension)
		|| isCollisionExtension(strFileExtension);
}

bool			GameFormat::isModelExtension(string& strFileExtension)
{
	return RWManager::isRWModelExtension(strFileExtension)
		|| RageManager::isRageModelExtension(strFileExtension);
}

bool			GameFormat::isTextureExtension(string& strFileExtension)
{
	return RWManager::isRWTextureExtension(strFileExtension)
		|| RageManager::isRageTextureExtension(strFileExtension);
}

bool			GameFormat::isCollisionExtension(string& strFileExtension)
{
	return COLManager::isCollisionExtension(strFileExtension)
		|| RageManager::isRageCollisionExtension(strFileExtension);
}

bool			GameFormat::isAnimationExtension(string& strFileExtension)
{
	string strFileExtensionUpper = String::toUpperCase(strFileExtension);
	return strFileExtensionUpper == "IFP";
}

// RW file type
EFileType		GameFormat::getRWFileType(string& strFileExtension)
{
	if (RWManager::isRWModelExtension(strFileExtension))
	{
		return MODEL;
	}
	else if (RWManager::isRWTextureExtension(strFileExtension))
	{
		return TEXTURE;
	}
	else if (COLManager::isCollisionExtension(strFileExtension))
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

EFileType		GameFormat::getRageFileType(string& strFileExtension)
{
	if (RageManager::isRageTextureExtension(strFileExtension))
	{
		return TEXTURE;
	}
	else
	{
		return UNKNOWN;
	}
}