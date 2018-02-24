#include "IPLManager.h"
#include "Format/IPL/IPLFormat.h"
#include "Static/String.h"
#include "Event/Events.h"
#include "../BXCF/Event/EEvent.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

IPLManager::IPLManager(void)
{
}

// initialization
void			IPLManager::init(void)
{
}
void			IPLManager::uninit(void)
{
}

// entry fetching
void			IPLManager::getModelNamesFromFiles(
	vector<string>& vecIPLFilePaths,
	set<string>& stModelNames
)
{
	for (string& strIPLFilePath : vecIPLFilePaths)
	{
		IPLFormat iplFormat(strIPLFilePath);
		if (!iplFormat.unserialize())
		{
			continue;
		}

		for (string& strModelName : iplFormat.getModelNames())
		{
			stModelNames.insert(String::toUpperCase(strModelName));
		}

		Events::triggerDefault(TASK_PROGRESS);
	}
}