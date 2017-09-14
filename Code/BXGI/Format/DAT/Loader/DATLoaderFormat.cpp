#include "DATLoaderFormat.h"
#include "DATLoaderEntry.h"
#include "Static/Path.h"
#include "Format/IMG/Regular/IMGManager.h"
#include "Format/IMG/Regular/IMGFormat.h"
#include "Format/IDE/IDEManager.h"
#include "Format/IDE/IDEFormat.h"
#include "Format/IPL/IPLManager.h"
#include "Format/IPL/IPLFormat.h"
#include "Static/String.h"
#include "Stream/DataReader.h"
#include "Static/StdVector.h"
#include "DATLoaderManager.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

void					DATLoaderFormat::unload(void)
{
	removeAllEntries();
}

// serialization
void					DATLoaderFormat::_unserialize(void)
{
	DataReader *pDataReader = DataReader::get();
	pDataReader->readAndStoreLines();
	while (pDataReader->iterateLines())
	{
		unserializeLine();
	}
}

void					DATLoaderFormat::unserializeLine(void)
{
	DataReader *pDataReader = DataReader::get();
	string strLine = *pDataReader->getActiveLine();

	// remove comment from end of line
	string strComment = "";
	size_t uiCommentPosition = strLine.find('#');
	if (uiCommentPosition != string::npos)
	{
		strComment = strLine.substr(uiCommentPosition);
		strLine = strLine.substr(0, uiCommentPosition);
	}

	if (strLine == "")
	{
		// blank line
	}

	else if (String::ltrim(strLine).c_str()[0] == '#')
	{
		// line is a comment
	}

	else
	{
		// parse line
		deque<string> deqTokens = StdVector::convertVectorToDeque(String::split(strLine, " "));

		DATLoaderEntry *pDATLoaderEntry = new DATLoaderEntry;
		pDATLoaderEntry->setEntryType(DATLoaderManager::getDATEntryTypeFromString(deqTokens[0]));
		deqTokens.pop_front();
		pDATLoaderEntry->setEntryValues(deqTokens);
		addEntry(pDATLoaderEntry);
	}
}

// item unserialization
vector<IMGFormat*>		DATLoaderFormat::parseIMGFiles(string strGameDirectoryPath)
{
	return parseFiles<IMGManager, IMGFormat>(strGameDirectoryPath, DAT_LOADER_IMG, DAT_LOADER_CDIMAGE);
}

vector<IDEFormat*>		DATLoaderFormat::parseIDEFiles(string strGameDirectoryPath)
{
	return parseFiles<IDEManager, IDEFormat>(strGameDirectoryPath, DAT_LOADER_IDE);
}

vector<IPLFormat*>		DATLoaderFormat::parseIPLFiles(string strGameDirectoryPath)
{
	return parseFiles<IPLManager, IPLFormat>(strGameDirectoryPath, DAT_LOADER_IPL);
}

template<class ManagerClass, class FormatClass>
vector<FormatClass*>	DATLoaderFormat::parseFiles(string strGameDirectoryPath, EDATLoaderEntryType eType1, EDATLoaderEntryType eType2)
{
	strGameDirectoryPath = Path::addSlashToEnd(strGameDirectoryPath);

	vector<FormatClass*> vecFormats;

	for (auto pDATEntry : getEntries())
	{
		if (pDATEntry->getEntryType() == eType1 || (eType2 != DAT_LOADER_UNKNOWN && pDATEntry->getEntryType() == eType2))
		{
			string strFormatPath = strGameDirectoryPath + Path::removeSlashFromFront(pDATEntry->getEntryValues()[0]);
			FormatClass *pFormat = ManagerClass::get()->parseViaFile(strFormatPath);
			if (pFormat->doesHaveError())
			{
				pFormat->unload();
				delete pFormat;
			}
			else
			{
				vecFormats.push_back(pFormat);
			}
		}
	}

	return vecFormats;
}

// fetch IDE paths
vector<string>			DATLoaderFormat::getRelativeIDEPaths(void)
{
	vector<string> vecRelativeIDEPaths;
	for (auto pDATEntry : getEntries())
	{
		if (pDATEntry->getEntryType() == DAT_LOADER_IDE)
		{
			vecRelativeIDEPaths.push_back(pDATEntry->getEntryValues()[0]);
		}
	}
	return vecRelativeIDEPaths;
}

// fetch IPL paths
vector<string>			DATLoaderFormat::getRelativeIPLPaths(void)
{
	vector<string> vecRelativeIDEPaths;
	for (auto pDATEntry : getEntries())
	{
		if (pDATEntry->getEntryType() == DAT_LOADER_IPL)
		{
			vecRelativeIDEPaths.push_back(pDATEntry->getEntryValues()[0]);
		}
	}
	return vecRelativeIDEPaths;
}