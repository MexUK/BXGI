#include "IntermediateTextureFormat.h"

using namespace bxgi;

void			IntermediateTextureFormat::unload(void)
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