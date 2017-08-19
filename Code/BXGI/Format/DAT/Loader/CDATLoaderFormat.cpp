#include "CDATLoaderFormat.h"
#include "CDATLoaderEntry.h"
#include "Static/CPath.h"
#include "Format/IMG/Regular/CIMGManager.h"
#include "Format/IMG/Regular/CIMGFormat.h"
#include "Format/IDE/CIDEManager.h"
#include "Format/IDE/CIDEFormat.h"
#include "Format/IPL/CIPLManager.h"
#include "Format/IPL/CIPLFormat.h"
#include "Static/CString2.h"
#include "Stream/CDataReader.h"
#include "Static/CStdVector.h"
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
	CDataReader *pDataReader = CDataReader::get();
	pDataReader->readAndStoreLines();
	while (pDataReader->iterateLines())
	{
		unserializeLine();
	}
}

void					CDATLoaderFormat::unserializeLine(void)
{
	CDataReader *pDataReader = CDataReader::get();
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

	else if (CString2::ltrim(strLine).c_str()[0] == '#')
	{
		// line is a comment
	}

	else
	{
		// parse line
		deque<string> deqTokens = CStdVector::convertVectorToDeque(CString2::split(strLine, " "));

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
vector<FormatClass*>	CDATLoaderFormat::parseFiles(string strGameDirectoryPath, eDATLoaderEntryType eType1, eDATLoaderEntryType eType2)
{
	strGameDirectoryPath = CPath::addSlashToEnd(strGameDirectoryPath);

	vector<FormatClass*> vecFormats;

	for (auto pDATEntry : getEntries())
	{
		if (pDATEntry->getEntryType() == eType1 || (eType2 != DAT_LOADER_UNKNOWN && pDATEntry->getEntryType() == eType2))
		{
			string strFormatPath = strGameDirectoryPath + CPath::removeSlashFromFront(pDATEntry->getEntryValues()[0]);
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