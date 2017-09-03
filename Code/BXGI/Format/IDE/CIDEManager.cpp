#include "CIDEManager.h"
#include "CIDEFormat.h"
#include "EIDESection.h"
#include "Static/String2.h"
#include "Static/StdVector.h"
#include "Static/Debug.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

CIDEManager::CIDEManager(void)
{
}

void			CIDEManager::init(void)
{
}
void			CIDEManager::uninit(void)
{
}

vector<string>		CIDEManager::getIDEEntryNamesWithoutExtension(vector<string> vecIDEPaths, bool bModelNames, bool bTXDNames)
{
	vector<string> vecEntryNamesWithoutExtension;
	CIDEFormat *pIDEFile = nullptr;
	for (auto strIDEPath : vecIDEPaths)
	{
		pIDEFile = CIDEManager::get()->parseViaFile(strIDEPath);
		if (pIDEFile->doesHaveError())
		{
			pIDEFile->unload();
			delete pIDEFile;
			continue;
		}
		vector<string>
			vecModelNames,
			vecTextureNames;
		if (bModelNames)
		{
			vecModelNames = pIDEFile->getModelNames();
		}
		if (bTXDNames)
		{
			vecTextureNames = pIDEFile->getTXDNames();
		}
		vecEntryNamesWithoutExtension = StdVector::combineVectors(vecEntryNamesWithoutExtension, StdVector::combineVectors(vecModelNames, vecTextureNames));
		pIDEFile->unload();
		delete pIDEFile;
	}
	return StdVector::removeDuplicates(vecEntryNamesWithoutExtension);
}