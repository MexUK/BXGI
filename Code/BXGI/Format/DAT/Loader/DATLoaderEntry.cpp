#include "DATLoaderEntry.h"
#include "Stream/DataWriter.h"
#include "Static/String.h"
#include "Static/StdVector.h"
#include "DATLoaderFormat.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

DATLoaderEntry::DATLoaderEntry(DATLoaderFormat *pFormat) :
	m_pFormat(pFormat)
{
}

// serialization
void						DATLoaderEntry::serialize(void)
{
	DataWriter *pDataWriter = &m_pFormat->m_writer;

	if (m_uiEntryType2 == DAT_LOADER_UNKNOWN)
	{
		pDataWriter->writeTokenString(m_deqValues[0]);
	}
	else
	{
		pDataWriter->writeTokenString(getEntryTypeText());
		for (string& strToken : m_deqValues)
		{
			pDataWriter->writeTokenString(strToken);
		}
	}
}

// entry type text
string						DATLoaderEntry::getEntryTypeText(void)
{
	switch (m_uiEntryType2)
	{
	case DAT_LOADER_IMG:		return "IMG";
	case DAT_LOADER_CDIMAGE:	return "CDIMAGE";
	case DAT_LOADER_IMGLIST:	return "IMGLIST";
	case DAT_LOADER_WATER:		return "WATER";
	case DAT_LOADER_IDE:		return "IDE";
	case DAT_LOADER_HIERFILE:	return "HIERFILE";
	case DAT_LOADER_COLFILE:	return "COLFILE";
	case DAT_LOADER_IPL:		return "IPL";
	case DAT_LOADER_MAPZONE:	return "MAPZONE";
	case DAT_LOADER_TEXDICTION:	return "TEXDICTION";
	case DAT_LOADER_MODELFILE:	return "MODELFILE";
	case DAT_LOADER_SPLASH:		return "SPLASH";
	case DAT_LOADER_EXIT:		return "EXIT";
	case DAT_LOADER_UNKNOWN:
	default:
								return "UNKNOWN";

	}
}