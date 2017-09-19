#include "IDEManager.h"
#include "Format/IDE/IDEFormat.h"
#include "Format/IDE/EIDESection.h"
#include "Static/String.h"
#include "Static/StdVector.h"
#include "Static/Debug.h"
#include "Event/Events.h"
#include "../BXCF/Event/EEvent.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

IDEManager::IDEManager(void)
{
}

// initialization
void			IDEManager::init(void)
{
}
void			IDEManager::uninit(void)
{
}

// entry fetching
void			IDEManager::getModelAndTextureSetNamesFromFiles(
	vector<string>& vecIDEFilePaths,
	set<string>& stModelNames,
	set<string>& stTextureSetNames,
	vector<EIDESection>& vecModelSections,
	vector<EIDESection>& vecTextureSections
)
{
	for (string& strIDEFilePath : vecIDEFilePaths)
	{
		IDEFormat ideFormat(strIDEFilePath);
		if (!ideFormat.unserialize())
		{
			continue;
		}

		for (string& strModelName : ideFormat.getModelNamesInSections(vecModelSections))
		{
			stModelNames.insert(String::toUpperCase(strModelName));
		}
		for (string& strTextureSetName : ideFormat.getTextureSetNamesInSections(vecTextureSections))
		{
			stTextureSetNames.insert(String::toUpperCase(strTextureSetName));
		}

		Events::trigger(TASK_PROGRESS);
	}
}

// entry fetching old
vector<string>	IDEManager::getIDEEntryNamesWithoutExtension(vector<string> vecIDEPaths, bool bModelNames, bool bTXDNames)
{
	vector<string> vecEntryNamesWithoutExtension;
	IDEFormat *pIDEFile = nullptr;
	for (auto strIDEPath : vecIDEPaths)
	{
		pIDEFile = IDEManager::get()->unserializeFile(strIDEPath);
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