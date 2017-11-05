#include "IMGFormat.h"
#include "nsbxgi.h"
#include "Type/Types.h"
#include "Event/Events.h"
#include "Event/EEvent.h"
#include "Stream/DataWriter.h"
#include "IMGManager.h"
#include "Static/String.h"
#include "Exception/EExceptionCode.h"
#include "Static/Path.h"
#include "Format/COL/COLManager.h"
#include "Localization/LocalizationManager.h"
#include "Engine/RW/RWManager.h"
#include "Engine/RW/RWVersion.h"
#include "Static/Memory.h"
#include "Engine/RAGE/RageManager.h"
#include "Engine/RAGE/RageResourceTypeManager.h"
#include "Static/File.h"
#include "Static/Timing.h"
#include "Static/Debug.h"
#include "Format/EFileType.h"
#include "Static/StdVector.h"
#include "IMGEntry.h"
#include "../BXCF/Event/EEvent.h"
#include "Format/DFF/DFFFormat.h"
#include "Format/IMG/Regular/IMGFormatVersion1.h"
#include "Format/IMG/Regular/IMGFormatVersion2.h"
#include "Format/IMG/Regular/IMGFormatVersion3.h"
#include "Format/IMG/Regular/IMGFormatVersionFastman92.h"
#include <set>
#include <algorithm>

using namespace std;
using namespace bxcf;
using namespace bxcf::fileType;
using namespace bxgi;

IMGFormat::IMGFormat(void) :
	ContainerFormat(true, LITTLE_ENDIAN),
	m_uiIMGVersion(IMG_UNKNOWN),
	m_uiPlatform(PLATFORM_PC),
	m_uiSubVersion(0),
	m_uiEncryptionType(0),
	m_ucGameType(0),
	m_bEncrypted(false)
{
}

IMGFormat::IMGFormat(EIMGVersion uiIMGVersion) :
	ContainerFormat(true, LITTLE_ENDIAN),
	m_uiIMGVersion(uiIMGVersion),
	m_uiPlatform(PLATFORM_PC),
	m_uiSubVersion(0),
	m_uiEncryptionType(0),
	m_ucGameType(0),
	m_bEncrypted(false)
{
}

IMGFormat::IMGFormat(EIMGVersion uiIMGVersion, std::string& strFilePathOrData, bool bStringIsFilePath) :
	ContainerFormat(strFilePathOrData, bStringIsFilePath, true, LITTLE_ENDIAN),
	m_uiIMGVersion(uiIMGVersion),
	m_uiPlatform(PLATFORM_PC),
	m_uiSubVersion(0),
	m_uiEncryptionType(0),
	m_ucGameType(0),
	m_bEncrypted(false)
{
}

IMGFormat::IMGFormat(std::string& strFilePathOrData, bool bStringIsFilePath) :
	ContainerFormat(strFilePathOrData, bStringIsFilePath, true, LITTLE_ENDIAN),
	m_uiIMGVersion(IMG_UNKNOWN),
	m_uiPlatform(PLATFORM_PC),
	m_uiSubVersion(0),
	m_uiEncryptionType(0),
	m_ucGameType(0),
	m_bEncrypted(false)
{
}

IMGFormat::IMGFormat(DataReader& reader) :
	ContainerFormat(reader, true, LITTLE_ENDIAN),
	m_uiIMGVersion(IMG_UNKNOWN),
	m_uiPlatform(PLATFORM_PC),
	m_uiSubVersion(0),
	m_uiEncryptionType(0),
	m_ucGameType(0),
	m_bEncrypted(false)
{
}

// meta data
void				IMGFormat::_readMetaData(void)
{
	if (String::toUpperCase(Path::getFileExtension(m_strFilePath)) == "DIR")
	{
		// version 1
		m_uiIMGVersion = IMG_1;
		m_uiEntryCount = (uint32)(m_reader.getSize() / 32);
		return;
	}

	bool bFileWasOpened = false;
	if (!m_reader.isFileOpen())
	{
		bFileWasOpened = m_reader.open(doesFormatUseBinaryData());
	}

	auto checkToCloseReader = [=](void)
	{
		if (bFileWasOpened)
		{
			m_reader.close();
		}
	};

	if (!m_reader.canRead(12))
	{
		string strFilePathDIRExt = getDIRFilePath();
		if (File::doesFileExist(strFilePathDIRExt))
		{
			m_reader.reset();
			m_reader.setFilePath(strFilePathDIRExt);
			m_strFilePath = strFilePathDIRExt; // temp
			if (!m_reader.open(m_bFormatUsesBinaryData))
			{
				checkToCloseReader();
				// todo throw new FAILED_TO_OPEN_FILE;
				return;
			}

			// version 1
			m_uiIMGVersion = IMG_1;
			m_uiEntryCount = (uint32)(m_reader.getSize() / 32);
			checkToCloseReader();
			return;
		}
	}

	string
		strFirst8Bytes = m_reader.readString(8),
		strFirst4Bytes = strFirst8Bytes.substr(0, 4);
	uint32
		uiSecond4BytesUi = String::unpackUint32(strFirst8Bytes.substr(4, 4), false);

	if (strFirst4Bytes == "VER2")
	{
		// version 2
		m_uiIMGVersion = IMG_2;
		m_uiEntryCount = uiSecond4BytesUi;
		checkToCloseReader();
		return;
	}

	string strFirst20Bytes = strFirst8Bytes + m_reader.readString(12);

	if (strFirst4Bytes == "VERF")
	{
		// version fastman92
		m_uiIMGVersion = IMG_FASTMAN92;
		uint32 uiArchiveFlags = uiSecond4BytesUi;
		string strAuthorName = String::rtrim(strFirst20Bytes.substr(8, 12));

		uint32 uiDecryptionValidCheck = m_reader.readUint32();
		m_uiEntryCount = m_reader.readUint32();
		string strReserved = m_reader.readString(8);

		m_uiSubVersion = uiArchiveFlags & 15;
		m_uiEncryptionType = (uiArchiveFlags >> 4) & 15;
		m_ucGameType = (uiArchiveFlags >> 8) & 7;

		m_bEncrypted = m_uiEncryptionType != 0;

		checkToCloseReader();

		return;
	}

	uint32 uiIdentifier = String::unpackUint32(strFirst4Bytes, false);
	bool bEncrypted = false;
	string strFirst16Bytes = strFirst20Bytes.substr(0, 16);

	if (uiIdentifier != 0xA94E2A52)
	{
		bEncrypted = true;
		strFirst16Bytes = IMGManager::decryptVersion3IMGString(strFirst16Bytes);
		uiIdentifier = String::unpackUint32(strFirst16Bytes.substr(0, 4), false);
		uiSecond4BytesUi = String::unpackUint32(strFirst16Bytes.substr(4, 4), false);
	}

	if (uiIdentifier == 0xA94E2A52)
	{
		if (uiSecond4BytesUi == 3)
		{
			// version 3
			m_uiIMGVersion = IMG_3;
			m_uiEntryCount = String::unpackUint32(strFirst16Bytes.substr(8, 4), false);
			m_bEncrypted = bEncrypted;
			m_uiEncryptionType = ENCRYPTION_GTA_V3_IMG;
			checkToCloseReader();
			return;
		}
	}

	string strFilePathDIRExt = getDIRFilePath();
	if (File::doesFileExist(strFilePathDIRExt))
	{
		m_reader.reset();
		m_reader.setFilePath(strFilePathDIRExt);
		m_strFilePath = strFilePathDIRExt; // temp
		if (!m_reader.open(m_bFormatUsesBinaryData))
		{
			checkToCloseReader();
			// todo throw new FAILED_TO_OPEN_FILE;
			return;
		}

		// version 1
		m_uiIMGVersion = IMG_1;
		m_uiEntryCount = (uint32)(m_reader.getSize() / 32);
		checkToCloseReader();
		return;
	}

	checkToCloseReader();
	throw EXCEPTION_UNKNOWN_FORMAT;
}

// serialization
void				IMGFormat::_unserializeBefore(void)
{
	checkMetaDataIsLoaded();
	m_reader.resetFileSeek();
}

void				IMGFormat::_unserializeAfter(void)
{
	if (m_uiIMGVersion == IMG_1)
	{
		checkXBOXCompressionStatusesForEntries();
	}

	if (m_uiIMGVersion == IMG_3)
	{
		unserializeResourceTypes();
	}
	else
	{
		unserializERWVersions(); // todo - fix function name - capital e in name
	}
}

void				IMGFormat::checkXBOXCompressionStatusesForEntries(void)
{
	if (m_reader.getStreamType() == DATA_STREAM_FILE && m_uiIMGVersion == IMG_1)
	{
		m_reader.reset();
		m_reader.setFilePath(getIMGFilePath());
		m_strFilePath = getIMGFilePath();
		m_reader.open(true); // open handle to IMG file (handle is closed in Format::unserializeVia*() methods
	}

	for (IMGEntry *pIMGEntry : getEntries())
	{
		m_reader.setSeek(pIMGEntry->getEntryOffset());
		if (m_reader.readUint32() == 0x67A3A1CE)
		{
			// entry is compressed with LZO 1X 999
			pIMGEntry->setCompressionAlgorithmId(COMPRESSION_LZO_1X_999);
		}

		//Events::trigger(UNSERIALIZE_IMG_ENTRY, this);
	}
}

// validation
bool				IMGFormat::validate(void)
{
	if (getVersion() == IMG_FASTMAN92)
	{
		// check if IMG is fastman92 format and is encrypted
		if (isEncrypted())
		{
			setErrorCode(EXCEPTION_UNKNOWN_ENCRYPTION_ALGORITHM_USED);
			return false;
		}

		// check if IMG is fastman92 format and has an unsupported game type
		if (getGameType() != 0)
		{
			setErrorCode(EXCEPTION_UNSUPPORTED_GAME_USED);
			return false;
		}
	}

	// no errors occurred
	return true;
}

// img format object creation
IMGFormat*		IMGFormat::createIMGFormat(EIMGVersion uiIMGVersion)
{
	switch (uiIMGVersion)
	{
	case IMG_1:				return new IMGFormatVersion1;
	case IMG_2:				return new IMGFormatVersion2;
	case IMG_3:				return new IMGFormatVersion3;
	case IMG_FASTMAN92:		return new IMGFormatVersionFastman92;
	}
	return nullptr;
}

IMGFormat*		IMGFormat::createIMGFormat(EIMGVersion uiIMGVersion, string& strIMGFilePath, bool bParam1IsFilePath)
{
	switch (uiIMGVersion)
	{
	case IMG_1:				return new IMGFormatVersion1(strIMGFilePath, bParam1IsFilePath);
	case IMG_2:				return new IMGFormatVersion2(strIMGFilePath, bParam1IsFilePath);
	case IMG_3:				return new IMGFormatVersion3(strIMGFilePath, bParam1IsFilePath);
	case IMG_FASTMAN92:		return new IMGFormatVersionFastman92(strIMGFilePath, bParam1IsFilePath);
	}
	return nullptr;
}

IMGFormat*		IMGFormat::createIMGFormat(string& strIMGFilePath, bool bParam1IsFilePath)
{
	IMGFormat img(strIMGFilePath, bParam1IsFilePath);
	if (!img.checkMetaDataIsLoaded())
	{
		return nullptr;
	}
	return createIMGFormat(img.getVersion(), strIMGFilePath, bParam1IsFilePath);
}

// version
IMGFormat*			IMGFormat::setVersion(EIMGVersion uiIMGVersion)
{
	IMGFormat *pNewIMGFormat = createIMGFormat(uiIMGVersion);
	pNewIMGFormat->setVersionDirect(uiIMGVersion);
	pNewIMGFormat->setPlatform(getPlatform());
	pNewIMGFormat->setSubVersion(getSubVersion());
	pNewIMGFormat->setEncryptionType(getEncryptionType());
	pNewIMGFormat->setGameType(getGameType());
	pNewIMGFormat->setEncrypted(isEncrypted());
	return pNewIMGFormat;
}

EIMGVersion			IMGFormat::getVersion(void)
{
	if (m_uiIMGVersion == IMG_UNKNOWN)
	{
		checkMetaDataIsLoaded();
	}

	return m_uiIMGVersion;
}

// read entry content, with try/catch
string		IMGFormat::readEntryContent(uint32 uiEntryIndex)
{
	string strEntryData;
	try
	{
		IMGEntry *pIMGEntry = getEntryByIndex(uiEntryIndex);
		m_reader.setSeek(pIMGEntry->getEntryOffset());
		strEntryData = m_reader.readString(pIMGEntry->getEntrySize());
	}
	catch (EExceptionCode uiErrorCode)
	{
		m_ucErrorCode = uiErrorCode;
	}
	return strEntryData;
}

// IMG/DIR file path
string		IMGFormat::getIMGFilePath(void)
{
	return Path::replaceFileExtensionWithCase(m_strFilePath, "IMG");
}

string		IMGFormat::getDIRFilePath(void)
{
	return Path::replaceFileExtensionWithCase(m_strFilePath, "DIR");
}

// unserialization
void		IMGFormat::unserializeAppDataComponents(void) // todo - make virtual
{
	switch (getVersion())
	{
	case IMG_1:
	case IMG_2:
	case IMG_FASTMAN92:
		unserializERWVersions();
		break;
	case IMG_3:
		unserializeResourceTypes();
		break;
	}
}

// app-level-data unserialization
void		IMGFormat::unserializERWVersions(void) // todo fix name(s) letter case
{
	if (m_reader.getStreamType() == DATA_STREAM_FILE && m_uiIMGVersion == IMG_1)
	{
		m_reader.reset();
		m_reader.setFilePath(getIMGFilePath());
		m_strFilePath = getIMGFilePath();
		m_reader.open(true); // open handle to IMG file (handle is closed in Format::unserializeVia*() methods
	}

	for (IMGEntry *pIMGEntry : getEntries())
	{
		pIMGEntry->unserializeRWVersion(&m_reader);

		Events::trigger(UNSERIALIZE_IMG_ENTRY, this);
	}
}

void		IMGFormat::unserializeResourceTypes(void)
{
	for (IMGEntry *pIMGEntry : getEntries())
	{
		pIMGEntry->unserializeResourceType(&m_reader);

		Events::trigger(UNSERIALIZE_IMG_ENTRY, this);
	}
}

// entry fetching
vector<IMGEntry*>		IMGFormat::getEntriesByNames(vector<string>& vecEntryNames)
{
	set<string> stEntryNames;
	for (string& strEntryName : vecEntryNames)
	{
		stEntryNames.insert(String::toUpperCase(Path::removeFileExtension(strEntryName)));
	}

	vector<IMGEntry*> vecIMGEntries;
	for (IMGEntry *pIMGEntry : getEntries())
	{
		if (stEntryNames.find(String::toUpperCase(Path::removeFileExtension(pIMGEntry->getEntryName()))) != stEntryNames.end())
		{
			vecIMGEntries.push_back(pIMGEntry);
		}
	}
	return vecIMGEntries;
}

void					IMGFormat::getModelAndTextureSetNamesFromEntries(
	unordered_map<IMGEntry*, vector<string>>& umapIMGModelNames,
	unordered_map<IMGEntry*, vector<string>>& umapIMGTextureSetNames
)
{
	for (IMGEntry *pIMGEntry : getEntries())
	{
		if (pIMGEntry->isModelFile())
		{
			umapIMGModelNames[pIMGEntry].push_back(String::toUpperCase(Path::removeFileExtension(pIMGEntry->getEntryName())));
		}
		else if (pIMGEntry->isTextureFile())
		{
			umapIMGTextureSetNames[pIMGEntry].push_back(String::toUpperCase(Path::removeFileExtension(pIMGEntry->getEntryName())));
		}

		Events::trigger(TASK_PROGRESS);
	}
}

// extension counts
void					IMGFormat::addEntryExtensionCount(string strEntryExtension)
{
	strEntryExtension = String::toUpperCase(strEntryExtension);
	if (m_umapExtensionCounts.count(strEntryExtension) > 0)
	{
		m_umapExtensionCounts[strEntryExtension]++;
	}
	else
	{
		m_umapExtensionCounts[strEntryExtension] = 1;
	}
}
void					IMGFormat::removeEntryExtensionCount(string strEntryExtension)
{
	strEntryExtension = String::toUpperCase(strEntryExtension);
	if (m_umapExtensionCounts[strEntryExtension] > 1)
	{
		m_umapExtensionCounts[strEntryExtension]--;
	}
	else
	{
		m_umapExtensionCounts.erase(strEntryExtension);
	}
}

uint32					IMGFormat::getEntryExtensionCount(string strEntryExtension)
{
	strEntryExtension = String::toUpperCase(strEntryExtension);
	if (m_umapExtensionCounts.count(strEntryExtension) > 0)
	{
		return m_umapExtensionCounts[strEntryExtension];
	}
	else
	{
		return 0;
	}
}

void					IMGFormat::loadEntryExtensionCounts(void)
{
	m_umapExtensionCounts.clear();
	for (IMGEntry *pIMGEntry : getEntries())
	{
		addEntryExtensionCount(pIMGEntry->getEntryExtension());
	}
}

vector<EFileType>			IMGFormat::getFileTypes(void)
{
	set<EFileType> setFileTypes;
	vector<EFileType> vecFileTypes;
	EFileType uiEntryType;
	for (IMGEntry *pEntry : getEntries())
	{
		uiEntryType = pEntry->getFileType();
		if (setFileTypes.count(uiEntryType) == 0)
		{
			setFileTypes.insert(uiEntryType);
			vecFileTypes.push_back(uiEntryType);
		}
	}
	return vecFileTypes;
}

vector<string>				IMGFormat::getFileTypesText(void)
{
	return StdVector::mapSorted(getFileTypes(), Format::getFileTypeText);
}

map<string, EFileType, SortByStringKey>		IMGFormat::getFileTypesAsMap(void)
{
	map<string, EFileType, SortByStringKey> mapFileTypes;
	for (EFileType uiFileType : getFileTypes())
	{
		mapFileTypes[Format::getFileTypeText(uiFileType)] = uiFileType;
	}
	return mapFileTypes;
}

vector<string>				IMGFormat::getFileVersions(void)
{
	set<string> setFileVersions;
	vector<string> vecFileVersions;
	string strVersionText;
	for (IMGEntry *pEntry : getEntries())
	{
		strVersionText = pEntry->getVersionText();
		if (setFileVersions.count(strVersionText) == 0)
		{
			setFileVersions.insert(strVersionText);
			vecFileVersions.push_back(strVersionText);
		}
	}
	StdVector::sortAZCaseInsensitive(vecFileVersions);
	return vecFileVersions;
}

map<string, pair<uint32, EFileType>, SortByStringKey>		IMGFormat::getFileTypedVersionsAsMap(void)
{
	map<string, pair<uint32, EFileType>, SortByStringKey> mapFileTypedVersions;
	set<string> setFileVersions;
	string strVersionText;
	for (IMGEntry *pEntry : getEntries())
	{
		strVersionText = pEntry->getVersionText();
		if (setFileVersions.count(strVersionText) == 0)
		{
			setFileVersions.insert(strVersionText);
			mapFileTypedVersions[strVersionText] = pair<uint32, EFileType>(pEntry->getRawVersion(), pEntry->getFileType());
		}
	}
	return mapFileTypedVersions;
}








ECompressionAlgorithm								IMGFormat::getCompressionAlgorithmIdFromFastman92CompressionAlgorithmId(EIMGVersionFastman92CompressionAlgorithm eFastman92CompressionAlgorithmId)
{
	switch (eFastman92CompressionAlgorithmId)
	{
	case IMGFASTMAN92COMPRESSIONALGORITHM_UNKNOWN:		return COMPRESSION_UNKNOWN;
	case IMGFASTMAN92COMPRESSIONALGORITHM_UNCOMPRESSED:	return COMPRESSION_NONE;
	case IMGFASTMAN92COMPRESSIONALGORITHzLIB:			return COMPRESSION_ZLIB;
	case IMGFASTMAN92COMPRESSIONALGORITHM_LZ4:			return COMPRESSION_LZ4;
	}
	return COMPRESSION_UNKNOWN;
}

uint32			IMGFormat::getVersion3NamesLength(void)
{
	uint32 uiLength = 0;
	for (IMGEntry *pIMGEntry : getEntries())
	{
		uiLength += pIMGEntry->getEntryName().length();
	}
	uiLength += getEntryCount();
	return uiLength;
}

IMGEntry*							IMGFormat::addEntryViaFile(string& strEntryFilePath, string strEntryName)
{
	if (strEntryName == "")
	{
		strEntryName = Path::getFileName(strEntryFilePath);
	}

	string strEntryData = File::getFileContent(strEntryFilePath);

	IMGEntry *pIMGEntry = new IMGEntry(this);

	m_uiEntryCount++;

	pIMGEntry->setNewEntry(true);
	pIMGEntry->setEntryName(strEntryName);
	pIMGEntry->setEntryOffset(getNextEntryOffset());
	pIMGEntry->setEntrySize(strEntryData.length()); // this is also set inside IMGEntry::setEntryData()
	pIMGEntry->setEntryExtension(String::toUpperCase(Path::getFileExtension(strEntryName)));
	pIMGEntry->setFileCreationDate(File::getFileCreationDate(strEntryFilePath));

	if (getVersion() == IMG_3)
	{
		pIMGEntry->setRageResourceType(RageManager::get()->getResourceTypeManager()->getResourceTypeByFileExtension(Path::getFileExtension(pIMGEntry->getEntryName())));
		pIMGEntry->setFlags((uint16)IMGEntry::getVersion3IMGSizeDeduction(pIMGEntry->getEntrySize()));
	}
	else
	{
		pIMGEntry->unserializeRWVersion(nullptr, "", strEntryData.substr(0, 12));
	}

	pIMGEntry->setEntryData(strEntryData, true);

	addEntry(pIMGEntry);
	return pIMGEntry;
}

IMGEntry*							IMGFormat::addEntryViaData(string& strEntryName, string& strEntryData)
{
	IMGEntry *pIMGEntry = new IMGEntry(this);

	m_uiEntryCount++;

	pIMGEntry->setNewEntry(true);
	pIMGEntry->setEntryName(strEntryName);
	pIMGEntry->setEntryOffset(getNextEntryOffset());
	pIMGEntry->setEntrySize(strEntryData.length()); // this is also set inside IMGEntry::setEntryData()
	pIMGEntry->setEntryExtension(String::toUpperCase(Path::getFileExtension(strEntryName)));
	// todo pIMGEntry->setFileCreationDate(Math::getTimeNow());

	if (getVersion() == IMG_3)
	{
		pIMGEntry->setRageResourceType(RageManager::get()->getResourceTypeManager()->getResourceTypeByFileExtension(Path::getFileExtension(pIMGEntry->getEntryName())));
		pIMGEntry->setFlags((uint16)IMGEntry::getVersion3IMGSizeDeduction(pIMGEntry->getEntrySize()));
	}
	else
	{
		pIMGEntry->unserializeRWVersion(nullptr, "", strEntryData.substr(0, 12));
	}

	pIMGEntry->setEntryData(strEntryData, true);

	addEntry(pIMGEntry);
	return pIMGEntry;
}

void					IMGFormat::addEntry(IMGEntry *pIMGEntry)
{
	VectorPool::addEntry(pIMGEntry);

	Events::trigger(ADD_IMG_ENTRY, pIMGEntry);

	string strEntryExtension = Path::getFileExtension(pIMGEntry->getEntryName());
	addEntryExtensionCount(strEntryExtension);
	if (getEntryExtensionCount(strEntryExtension) == 1)
	{
		Events::trigger(ADD_IMG_ENTRY_FILE_EXTENSION, pIMGEntry);
	}
}

void								IMGFormat::addEntries(vector<string>& vecEntryFilePaths)
{
	for (string& strEntryFilePath : vecEntryFilePaths)
	{
		addEntryViaFile(strEntryFilePath);
	}
}

void								IMGFormat::addAllEntriesInFolder(string& strFolderPath)
{
	vector<string> vecFileNames = File::getFileNames(strFolderPath);
	for (string& strFileName : vecFileNames)
	{
		addEntryViaFile(strFileName);
	}
}

void								IMGFormat::removeEntry(IMGEntry *pIMGEntry)
{
	string strEntryExtension = Path::getFileExtension(pIMGEntry->getEntryName());
	removeEntryExtensionCount(strEntryExtension);
	if (getEntryExtensionCount(strEntryExtension) == 0)
	{
		Events::trigger(REMOVE_IMG_ENTRY_FILE_EXTENSION, pIMGEntry);
	}

	Events::trigger(REMOVE_IMG_ENTRY, pIMGEntry);

	VectorPool::removeEntry(pIMGEntry);
	m_uiEntryCount--;
}

void								IMGFormat::removeAllEntries(void)
{
	VectorPool::removeAllEntries();
	m_uiEntryCount = 0;
}

IMGEntry*							IMGFormat::replaceEntryViaFile(string& strEntryName, string& strEntryFilePath, string strNewEntryName)
{
	string strEntryData = File::getFileContent(strEntryFilePath);
	IMGEntry *pIMGEntry = replaceEntryViaData(strEntryName, strEntryData, strNewEntryName);
	pIMGEntry->setFileCreationDate(File::getFileCreationDate(strEntryFilePath));
	return pIMGEntry;
}
IMGEntry*							IMGFormat::replaceEntryViaData(string& strEntryName, string& strEntryData, string strNewEntryName)
{
	IMGEntry *pIMGEntry = getEntryByName(strEntryName);
	if (!pIMGEntry)
	{
		return nullptr;
	}

	if (strNewEntryName != "")
	{
		pIMGEntry->setEntryName(strNewEntryName);
	}

	pIMGEntry->setEntryData(strEntryData);
	return pIMGEntry;
}

uint32						IMGFormat::replaceEntries(vector<string>& vecPaths, vector<string>& vecReplacedEntryNames, vector<IMGEntry*>& vecReplacedEntries)
{
	uint32 uiReplaceCount = 0;

	for (string& strPath : vecPaths)
	{
		string strNewEntryName = Path::getFileName(strPath);
		IMGEntry *pIMGEntry = getEntryByName(strNewEntryName);
		if (!pIMGEntry)
		{
			Events::triggerConst(REPLACE_IMG_ENTRY_SKIPPED_ENTRY, this);
			continue;
		}

		// body
		string strFileContent = File::getFileContent(strPath);
		uint32 uiFileSize = strFileContent.length();
		pIMGEntry->setEntryData(strFileContent);

		// header
		pIMGEntry->setEntryName(strNewEntryName);
		pIMGEntry->setEntrySize(uiFileSize);
		if (getVersion() == IMG_3)
		{
			pIMGEntry->setRageResourceType(RageManager::get()->getResourceTypeManager()->getResourceTypeByFileExtension(Path::getFileExtension(strNewEntryName)));
		}
		else
		{
			pIMGEntry->setRWVersionByVersionCC(strFileContent.length() >= 12 ? String::unpackUint32(strFileContent.substr(8, 4), false) : 0);
		}

		uiReplaceCount++;
		vecReplacedEntryNames.push_back(strNewEntryName);
		vecReplacedEntries.push_back(pIMGEntry);

		Events::triggerConst(REPLACE_IMG_ENTRY, this);
	}

	return uiReplaceCount;
}

IMGEntry*							IMGFormat::addOrReplaceEntryViaFile(string& strEntryFilePath, string strEntryName)
{
	IMGEntry *pIMGEntry = getEntryByName(strEntryName);
	if (pIMGEntry)
	{
		pIMGEntry = replaceEntryViaFile(strEntryName, strEntryFilePath);
	}
	else
	{
		pIMGEntry = addEntryViaFile(strEntryFilePath, strEntryName);
	}
	return pIMGEntry;
}
IMGEntry*							IMGFormat::addOrReplaceEntryViaData(string& strEntryName, string& strEntryData)
{
	IMGEntry *pIMGEntry = getEntryByName(strEntryName);
	if (pIMGEntry)
	{
		pIMGEntry = replaceEntryViaData(strEntryName, strEntryData);
	}
	else
	{
		pIMGEntry = addEntryViaData(strEntryName, strEntryData);
	}
	return pIMGEntry;
}

unordered_map<IMGEntry*, string>	IMGFormat::getAllEntriesData(void)
{
	return getEntriesData(getEntries());
}

unordered_map<IMGEntry*, string>	IMGFormat::getEntriesData(vector<IMGEntry*>& vecEntries)
{
	unordered_map<IMGEntry*, string> umapEntriesData;
	
	m_reader.setFilePath(getFilePath()); // todo - use IMG file not DIR file
	if(!openFile())
	{
		return umapEntriesData;
	}

	for (IMGEntry *pIMGEntry : vecEntries)
	{
		umapEntriesData.insert(make_pair(pIMGEntry, readEntryContent(getIndexByEntry(pIMGEntry))));
	}
	m_reader.close();
	
	return umapEntriesData;
}

uint32			IMGFormat::getNextEntryOffset(void)
{
	if (getEntryCount() == 0)
	{
		switch (getVersion())
		{
		case IMG_1:
			return 0;
		case IMG_2:
		case IMG_3:
			return 2048;
		}
		return 0;
	}

	IMGEntry *pIMGEntry = getEntryByHighestOffset();
	return pIMGEntry->getEntryOffset() + pIMGEntry->getPaddedEntrySize();
}

vector<IMGEntry*>		IMGFormat::getEntriesByName(string strText)
{
	strText = String::toUpperCase(strText);
	vector<IMGEntry*> vecIMGEntries;
	for (IMGEntry *pIMGEntry : getEntries())
	{
		if (String::isIn(String::toUpperCase(pIMGEntry->getEntryName()), strText))
		{
			vecIMGEntries.push_back(pIMGEntry);
		}
		Events::trigger(TASK_PROGRESS);
	}
	return vecIMGEntries;
}

vector<IMGEntry*>		IMGFormat::getEntriesByExtension(string strExtension, bool bWildcard)
{
	strExtension = String::toUpperCase(strExtension);
	vector<IMGEntry*> vecIMGEntries;
	for (IMGEntry *pIMGEntry : getEntries())
	{
		if (bWildcard)
		{
			if (String::isIn(String::toUpperCase(Path::getFileExtension(pIMGEntry->getEntryName())), strExtension))
			{
				vecIMGEntries.push_back(pIMGEntry);
			}
		}
		else
		{
			if (String::toUpperCase(Path::getFileExtension(pIMGEntry->getEntryName())) == strExtension)
			{
				vecIMGEntries.push_back(pIMGEntry);
			}
		}
		Events::trigger(TASK_PROGRESS);
	}
	return vecIMGEntries;
}

vector<IMGEntry*>		IMGFormat::getEntriesByVersion(uint32 uiFileTypeId, uint32 uiFileVersionId)
{
	vector<IMGEntry*> vecIMGEntries;
	for (IMGEntry *pIMGEntry : getEntries())
	{
		if (pIMGEntry->getFileType() == uiFileTypeId && pIMGEntry->getRawVersion() == uiFileVersionId)
		{
			vecIMGEntries.push_back(pIMGEntry);
		}
		Events::trigger(TASK_PROGRESS);
	}
	return vecIMGEntries;
}

IMGEntry*				IMGFormat::getEntryByName(string& strEntryName)
{
	strEntryName = String::toUpperCase(strEntryName);
	for (IMGEntry *pIMGEntry : getEntries())
	{
		if (strEntryName == String::toUpperCase(pIMGEntry->getEntryName()))
		{
			return pIMGEntry;
		}
	}
	return nullptr;
}

IMGEntry*				IMGFormat::getEntryByNameWithoutExtension(string& strEntryNameWithoutExtension)
{
	strEntryNameWithoutExtension = String::toUpperCase(strEntryNameWithoutExtension);
	for (IMGEntry *pIMGEntry : getEntries())
	{
		if (strEntryNameWithoutExtension == String::toUpperCase(Path::removeFileExtension(pIMGEntry->getEntryName())))
		{
			return pIMGEntry;
		}
	}
	return nullptr;
}

IMGEntry*				IMGFormat::getEntryByHighestOffset(void)
{
	uint32 uiHighestOffset = 0;
	IMGEntry *pHighestOffsetIMGEntry = nullptr;
	for (IMGEntry *pIMGEntry : getEntries())
	{
		if (pIMGEntry->getEntryOffsetInSectors() >= uiHighestOffset)
		{
			uiHighestOffset = pIMGEntry->getEntryOffsetInSectors();
			pHighestOffsetIMGEntry = pIMGEntry;
		}
	}
	return pHighestOffsetIMGEntry;
}

uint32			IMGFormat::getEntryCountForName(string& strEntryName, bool bWilcard, bool bWildcardIsLeftMatch)
{
	uint32 uiNameCount = 0;
	string strEntryNameUpper = String::toUpperCase(strEntryName);
	if (bWilcard)
	{
		if (bWildcardIsLeftMatch)
		{
			uint32 uiEntryNameMatchLength = strEntryNameUpper.length();
			for (IMGEntry *pIMGEntry : getEntries())
			{
				if (String::toUpperCase(pIMGEntry->getEntryName().substr(0, uiEntryNameMatchLength)) == strEntryNameUpper)
				{
					uiNameCount++;
				}
			}
		}
		else
		{
			for (IMGEntry *pIMGEntry : getEntries())
			{
				if (String::isIn(String::toUpperCase(pIMGEntry->getEntryName()), strEntryNameUpper))
				{
					uiNameCount++;
				}
			}
		}
	}
	else
	{
		for (IMGEntry *pIMGEntry : getEntries())
		{
			if (strEntryNameUpper == String::toUpperCase(pIMGEntry->getEntryName()))
			{
				uiNameCount++;
			}
		}
	}
	return uiNameCount;
}

void					IMGFormat::unsetNewAndReplacedFlagForAllEntries(void)
{
	for (IMGEntry *pIMGEntry : getEntries())
	{
		pIMGEntry->setNewEntry(false);
		pIMGEntry->setReplacedEntry(false);
	}
}

uint32			IMGFormat::getEntryPaddedSize(uint32 uiDataLength)
{
	return (uint32)(ceil((float)uiDataLength / (float)2048.0f) * (float)2048.0f);
}

uint32			IMGFormat::merge(string& strSecondIMGPath, vector<string>& vecImportedEntryNames)
{
	DataReader *pDataReader = &m_reader;

	// parse second IMG file for entry information
	IMGFormat *pIMGFileIn = createIMGFormat(strSecondIMGPath);
	pIMGFileIn->setFilePath(strSecondIMGPath);
	if (!pIMGFileIn->unserialize())
	{
		pIMGFileIn->unload();
		delete pIMGFileIn;
		return 0;
	}

	// open second IMG file to read entry body data from
	pDataReader->setFilePath(getFilePath());
	pDataReader->open(true);

	// import entries from second IMG into first IMG
	bool bVersion3IMG = getVersion() == IMG_3;
	for (IMGEntry *pInEntry : pIMGFileIn->getEntries())
	{
		// entry header data
		IMGEntry *pOutEntry = new IMGEntry(this);
		pOutEntry->setEntryName(pInEntry->getEntryName());
		pOutEntry->setEntrySize(pInEntry->getEntrySize());
		if (bVersion3IMG)
		{
			pOutEntry->setRageResourceType(pInEntry->getRageResourceType());
			pOutEntry->setFlags(pInEntry->getFlags());
		}
		else
		{
			pOutEntry->setRWVersion(pInEntry->getRWVersion());
		}
		addEntry(pOutEntry);
		m_uiEntryCount++;

		// entry body data
		pDataReader->setSeek(pInEntry->getEntryOffset());
		string strEntryData = pDataReader->readString(pInEntry->getEntrySize());
		pOutEntry->setEntryData(strEntryData, true);

		vecImportedEntryNames.push_back(pOutEntry->getEntryName());

		Events::trigger(TASK_PROGRESS);
	}

	// finalize
	pDataReader->close();

	uint32 uiImportedEntryCount = pIMGFileIn->getEntryCount();
	pIMGFileIn->unload();

	return uiImportedEntryCount;
}

void					IMGFormat::split(vector<IMGEntry*>& vecIMGEntries, string& strOutPath, EIMGVersion uiIMGVersion)
{
	IMGFormat *pIMGFile = createIMGFormat(uiIMGVersion);
	
	pIMGFile->setFilePath(getIMGFilePath()); // todo - rename to format::setInputPath()
	pIMGFile->m_writer.setFilePath(strOutPath);

	// todo - remove from here? pIMGFile->openFile(); // open input - todo - rename to openInputFile()

	bool bVersion3IMG = uiIMGVersion == IMG_3;
	for (IMGEntry *pIMGEntry : vecIMGEntries)
	{
		IMGEntry *pIMGEntry2 = new IMGEntry(pIMGFile);
		pIMGEntry2->setEntryName(pIMGEntry->getEntryName());
		pIMGEntry2->setEntryOffsetInSectors(pIMGEntry->getEntryOffsetInSectors());
		pIMGEntry2->setEntrySize(pIMGEntry->getEntrySize());
		pIMGFile->addEntry(pIMGEntry2);

		if (bVersion3IMG)
		{
			pIMGEntry2->setRageResourceType(pIMGEntry->getRageResourceType());
			pIMGEntry2->setFlags(pIMGEntry->getFlags());
		}
		else
		{
			pIMGEntry2->setRWVersion(pIMGEntry->getRWVersion());
		}

		Events::triggerConst(SPLIT_IMG_ENTRY, this);
	}

	pIMGFile->serialize(strOutPath);

	pIMGFile->closeOutput(); // todo - still needed?

	pIMGFile->unload();
	delete pIMGFile;
}

void					IMGFormat::exportSingle(IMGEntry *pIMGEntry, string& strFolderPath)
{
	if (!pIMGEntry->canBeRead())
	{
		return;
	}

	strFolderPath = Path::addSlashToEnd(strFolderPath);

	File::storeFile(strFolderPath + pIMGEntry->getEntryName(), pIMGEntry->getEntryData(), false, true);
}

void					IMGFormat::exportMultiple(vector<IMGEntry*>& vecIMGEntries, string& strFolderPath)
{
	exportMultiple((vector<FormatEntry*>&)vecIMGEntries, strFolderPath);
}

void					IMGFormat::exportMultiple(vector<FormatEntry*>& vecIMGEntries, string& strFolderPath)
{
	strFolderPath = Path::addSlashToEnd(strFolderPath);

	m_reader.setFilePath(getIMGFilePath());
	if(!openFile())
	{
		return;
	}

	for (IMGEntry *pIMGEntry : (vector<IMGEntry*>&)vecIMGEntries)
	{
		if (!pIMGEntry->canBeRead())
		{
			continue;
		}
		
		File::storeFile(strFolderPath + pIMGEntry->getEntryName(), readEntryContent(getIndexByEntry(pIMGEntry)), false, true);
		
		Events::trigger(TASK_PROGRESS);
	}

	m_reader.close();
}

void					IMGFormat::exportAll(string& strFolderPath)
{
	strFolderPath = Path::addSlashToEnd(strFolderPath);

	m_reader.setFilePath(getIMGFilePath());
	if(!openFile())
	{
		return;
	}
	
	uint32 uiEntryIndex = 0;
	for (IMGEntry *pIMGEntry : getEntries())
	{
		if (!pIMGEntry->canBeRead())
		{
			uiEntryIndex++;
			continue;
		}
		
		File::storeFile(strFolderPath + pIMGEntry->getEntryName(), readEntryContent(uiEntryIndex++), false, true);
		
		Events::trigger(TASK_PROGRESS);
	}
	m_reader.close();
}

vector<IMGEntry*>		IMGFormat::getUnknownVersionEntries(void)
{
	vector<IMGEntry*> vecIMGEntries;
	for (IMGEntry *pIMGEntry : getEntries())
	{
		if (pIMGEntry->doesHaveUnknownVersion())
		{
			vecIMGEntries.push_back(pIMGEntry);
		}
	}
	return vecIMGEntries;
}

uint32			IMGFormat::getEntryCountForCompressionType(ECompressionAlgorithm ECompressionAlgorithmValue)
{
	uint32 uiCount = 0;
	for (IMGEntry *pIMGEntry : getEntries())
	{
		if (pIMGEntry->getCompressionAlgorithmId() == ECompressionAlgorithmValue)
		{
			uiCount++;
		}
	}
	return uiCount;
}

vector<string>			IMGFormat::getEntryNames(void)
{
	vector<string> vecEntryNames;
	vecEntryNames.resize(getEntryCount());
	uint32 i = 0;
	for (IMGEntry *pIMGEntry : getEntries())
	{
		vecEntryNames[i] = pIMGEntry->getEntryName();
		++i;
	}
	return vecEntryNames;
}