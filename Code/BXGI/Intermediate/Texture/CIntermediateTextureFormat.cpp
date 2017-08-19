#include "CIntermediateTextureFormat.h"

using namespace bxgi;

void			CIntermediateTextureFormat::unload(void)
{
	for (auto pGeneralTexture : getEntries())
	{
		for (auto pGeneralMipmap : pGeneralTexture->getEntries())
		{
			delete pGeneralMipmap;
		}
		delete pGeneralTexture;
	}
}