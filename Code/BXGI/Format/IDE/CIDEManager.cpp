#include "CIDEManager.h"
#include "CIDEFormat.h"
#include "eIDESection.h"
#include "Static/CString2.h"
#include "Static/CStdVector.h"
#include "Static/CDebug.h"

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
		vecEntryNamesWithoutExtension = CStdVector::combineVectors(vecEntryNamesWithoutExtension, CStdVector::combineVectors(vecModelNames, vecTextureNames));
		pIDEFile->unload();
		delete pIDEFile;
	}
	return CStdVector::removeDuplicates(vecEntryNamesWithoutExtension);
}