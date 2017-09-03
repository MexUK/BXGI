#include "IDEManager.h"
#include "IDEFormat.h"
#include "EIDESection.h"
#include "Static/String2.h"
#include "Static/StdVector.h"
#include "Static/Debug.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

IDEManager::IDEManager(void)
{
}

void			IDEManager::init(void)
{
}
void			IDEManager::uninit(void)
{
}

vector<string>		IDEManager::getIDEEntryNamesWithoutExtension(vector<string> vecIDEPaths, bool bModelNames, bool bTXDNames)
{
	vector<string> vecEntryNamesWithoutExtension;
	IDEFormat *pIDEFile = nullptr;
	for (auto strIDEPath : vecIDEPaths)
	{
		pIDEFile = IDEManager::get()->parseViaFile(strIDEPath);
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