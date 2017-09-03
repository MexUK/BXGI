#include "CDATLoaderFormat.h"
#include "CDATLoaderEntry.h"
#include "Static/Path.h"
#include "Format/IMG/Regular/CIMGManager.h"
#include "Format/IMG/Regular/CIMGFormat.h"
#include "Format/IDE/CIDEManager.h"
#include "Format/IDE/CIDEFormat.h"
#include "Format/IPL/CIPLManager.h"
#include "Format/IPL/CIPLFormat.h"
#include "Static/String2.h"
#include "Stream/DataReader.h"
#include "Static/StdVector.h"
#include "CDATLoaderManager.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

void					CDATLoaderFormat::unload(void)
{
	removeAllEntries();
}

void					CDATLoaderFormat::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();
	pDataReader->readAndStoreLines();
	while (pDataReader->iterateLines())
	{
		unserializeLine();
	}
}

void					CDATLoaderFormat::unserializeLine(void)
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

	else if (String2::ltrim(strLine).c_str()[0] == '#')
	{
		// line is a comment
	}

	else
	{
		// parse line
		deque<string> deqTokens = StdVector::convertVectorToDeque(String2::split(strLine, " "));

		CDATLoaderEntry *pDATLoaderEntry = new CDATLoaderEntry;
		pDATLoaderEntry->setEntryType(CDATLoaderManager::getDATEntryTypeFromString(deqTokens[0]));
		deqTokens.pop_front();
		pDATLoaderEntry->setEntryValues(deqTokens);
		addEntry(pDATLoaderEntry);
	}
}

vector<string>			CDATLoaderFormat::getRelativeIDEPaths(void)
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

vector<string>			CDATLoaderFormat::getRelativeIPLPaths(void)
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

vector<CIMGFormat*>		CDATLoaderFormat::parseIMGFiles(string strGameDirectoryPath)
{
	return parseFiles<CIMGManager, CIMGFormat>(strGameDirectoryPath, DAT_LOADER_IMG, DAT_LOADER_CDIMAGE);
}

vector<CIDEFormat*>		CDATLoaderFormat::parseIDEFiles(string strGameDirectoryPath)
{
	return parseFiles<CIDEManager, CIDEFormat>(strGameDirectoryPath, DAT_LOADER_IDE);
}

vector<CIPLFormat*>		CDATLoaderFormat::parseIPLFiles(string strGameDirectoryPath)
{
	return parseFiles<CIPLManager, CIPLFormat>(strGameDirectoryPath, DAT_LOADER_IPL);
}

template<class ManagerClass, class FormatClass>
vector<FormatClass*>	CDATLoaderFormat::parseFiles(string strGameDirectoryPath, EDATLoaderEntryType eType1, EDATLoaderEntryType eType2)
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