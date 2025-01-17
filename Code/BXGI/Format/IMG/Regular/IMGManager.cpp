#include "IMGManager.h"
#include "Format/IMG/Regular/IMGEntry.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Static/String.h"
#include "Static/Path.h"
#include "Static/File.h"
#include "Encryption/AES/Rijndael/Rijndael.h"
#include "Encryption/EncryptionManager.h"
#include "Static/Debug.h"
#include "Format/COL/COLManager.h"
#include "Compression/ECompressionAlgorithm.h"
#include "Compression/CompressionManager.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

IMGManager::IMGManager(void)
{
}

void			IMGManager::init(void)
{
}
void			IMGManager::uninit(void)
{
}

// utility
EIMGVersion		IMGManager::detectIMGVersion(string& strIMGFilePath, string strHeader16B, bool& bIsEncryptedOut) // todo - still used?
{
	bIsEncryptedOut = false;

	// strIMGFilePath can be a path to an IMG file or a DIR file
	if (!File::doesFileExist(strIMGFilePath))
	{
		return IMG_UNKNOWN;
	}
	
	string strFileExtensionUpper = String::toUpperCase(Path::getFileExtension(strIMGFilePath));
	if (strFileExtensionUpper == "DIR")
	{
		// input file is: DIR file
		if (File::doesFileExist(Path::replaceFileExtension(strIMGFilePath, "img")))
		{
			// both IMG and DIR files exist in same folder
			return IMG_1;
		}
		else
		{
			// DIR file exists but IMG file does not exist in same folder
			return IMG_UNKNOWN;
		}
	}
	else
	{
		// input file is: IMG file
		string strHeader4B = "";
		uint32 uiFileSize = 0;
		if (strHeader16B == "")
		{
			uiFileSize = File::getFileSize(strIMGFilePath);
		}
		if (strHeader16B == "")
		{
			if (uiFileSize >= 16)
			{
				strHeader16B = File::getPartialBinaryFile(strIMGFilePath, 0, 16);
			}
		}
		if (strHeader16B == "")
		{
			if (uiFileSize >= 4)
			{
				strHeader4B = File::getPartialBinaryFile(strIMGFilePath, 0, 4);
			}
		}
		else
		{
			strHeader4B = strHeader16B.substr(0, 4);
		}

		if (strHeader4B == "VER2")
		{
			return IMG_2;
		}
		else if (strHeader4B == "VERF")
		{
			return IMG_FASTMAN92;
		}
		else if (strHeader4B.length() == 4 && String::unpackUint32(strHeader4B, false) == 0xA94E2A52)
		{
			return IMG_3; // unencrypted
		}
		else if (strHeader16B.length() == 16 && String::unpackUint32(IMGManager::decryptVersion3IMGString(strHeader16B).substr(0, 4), false) == 0xA94E2A52)
		{
			bIsEncryptedOut = true;
			return IMG_3; // encrypted
		}
		else
		{
			string strDIRFilePath = Path::replaceFileExtension(strIMGFilePath, "dir");
			if (File::doesFileExist(strDIRFilePath))
			{
				return IMG_1;
			}
		}
	}

	return IMG_UNKNOWN;
}

bool			IMGManager::detectIMGEncryptionState(string& strIMGFilePath)
{
	string strHeader16B = File::getPartialBinaryFile(strIMGFilePath, 0, 16);
	string strHeader4B = strHeader16B.substr(0, 4);

	// version 3
	if (String::unpackUint32(IMGManager::decryptVersion3IMGString(strHeader16B).substr(0, 4), false) == 0xA94E2A52)
	{
		return true;
	}

	// version fastman92
	if (((String::toNumber(strHeader16B.substr(4, 1)) >> 24) & 15) != 0)
	{
		return true;
	}

	return false;
}

uint32	IMGManager::getIMGEntryCount(string& strIMGFilePath, EIMGVersion eVersion)
{
	if (eVersion == IMG_UNKNOWN)
	{
		eVersion = detectIMGVersion(strIMGFilePath);
	}

	if (eVersion == IMG_1)
	{
		return File::getFileSize(Path::replaceFileExtension(strIMGFilePath, "dir")) / 32;
	}
	else if (eVersion == IMG_2)
	{
		return String::unpackUint32(File::getPartialBinaryFile(strIMGFilePath, 0, 8).substr(4, 4), false);
	}
	else if (eVersion == IMG_3)
	{
		if (detectIMGEncryptionState(strIMGFilePath))
		{
			string strHeader16B = File::getPartialBinaryFile(strIMGFilePath, 0, 16);
			return String::unpackUint32(IMGManager::decryptVersion3IMGString(strHeader16B).substr(8, 4), false);
		}
		else
		{
			return String::unpackUint32(File::getPartialBinaryFile(strIMGFilePath, 0, 12).substr(8, 4), false);
		}
	}
	else if (eVersion == IMG_FASTMAN92)
	{
		string strHeader28B = File::getPartialBinaryFile(strIMGFilePath, 0, 28);
		return String::unpackUint32(strHeader28B.substr(24, 4), false);
	}
	else
	{
		return 0;
	}
}

string			IMGManager::encryptVersion3IMGString(string strData)
{
	string strKey = "\x1a\xb5\x6f\xed\x7e\xc3\xff\x1\x22\x7b\x69\x15\x33\x97\x5d\xce\x47\xd7\x69\x65\x3f\xf7\x75\x42\x6a\x96\xcd\x6d\x53\x7\x56\x5d";
	for (uint32 i = 0; i < 16; i++)
	{
		strData = EncryptionManager::encryptAESECB(strData, strKey);
	}
	return strData;
}
string			IMGManager::decryptVersion3IMGString(string strData)
{
	string strKey = "\x1a\xb5\x6f\xed\x7e\xc3\xff\x1\x22\x7b\x69\x15\x33\x97\x5d\xce\x47\xd7\x69\x65\x3f\xf7\x75\x42\x6a\x96\xcd\x6d\x53\x7\x56\x5d";
	for (uint32 i = 0; i < 16; i++)
	{
		strData = EncryptionManager::decryptAESECB(strData, strKey);
	}
	return strData;
}

vector<string>		IMGManager::getDefaultGameIMGSubPaths(EPlatformedGame EPlatformedGameValue)
{
	vector<string> vecGameIMGPaths;
	switch (EPlatformedGameValue)
	{
	case PC_GTA_III:
		vecGameIMGPaths.push_back("models/gta3.img");
		vecGameIMGPaths.push_back("models/txd.img");
		break;
	case PC_GTA_VC:
		vecGameIMGPaths.push_back("models/gta3.img");
		vecGameIMGPaths.push_back("anim/cuts.img");
		break;
	case PC_GTA_SA:
		vecGameIMGPaths.push_back("models/gta3.img");
		vecGameIMGPaths.push_back("models/cutscene.img");
		vecGameIMGPaths.push_back("models/gta_int.img");
		vecGameIMGPaths.push_back("models/player.img");
		vecGameIMGPaths.push_back("data/Paths/carrec.img");
		vecGameIMGPaths.push_back("data/script/script.img");
		vecGameIMGPaths.push_back("anim/anim.img");
		vecGameIMGPaths.push_back("anim/cuts.img");
		break;
	case PC_SOL:
		vecGameIMGPaths.push_back("models/game.img");
		break;
	}
	return vecGameIMGPaths;
}

string				IMGManager::getCompressionTypeText(ECompressionAlgorithm ECompressionAlgorithmValue)
{
	switch (ECompressionAlgorithmValue)
	{
	case COMPRESSION_NONE:		return "Not Compressed";
	case COMPRESSION_ZLIB:		return "ZLib";
	case COMPRESSION_LZ4:		return "LZ4";
	case COMPRESSION_UNKNOWN:
	default:
		return "Unknown Compression Type";
	}
}
string				IMGManager::getEncryptionText(bool bIsEncrypted)
{
	if (bIsEncrypted)
	{
		return "Encrypted";
	}
	else
	{
		return "Not Encrypted";
	}
}

// IMG versions
vector<string>		IMGManager::getVersionsText(void)
{
	vector<string> vecIMGVersionsText = {
		"IMG 1 (GTA III / VC)",
		"IMG 2 (GTA SA)",
		"IMG 3 (GTA IV)",
		"IMG Fastman92"
	};
	return vecIMGVersionsText;
}

EIMGVersion			IMGManager::getVersionFromVersionIndex(int32 iVersionIndex)
{
	switch (iVersionIndex)
	{
	case 0:			return IMG_1;
	case 1:			return IMG_2;
	case 2:			return IMG_3;
	case 3:			return IMG_FASTMAN92;
	}
	return IMG_UNKNOWN;
}

string				IMGManager::getVersionText(EIMGVersion eVersion, bool bIsEncrypted)
{
	switch (eVersion)
	{
	case IMG_1:				return "1";
	case IMG_2:				return "2";
	case IMG_3:
		if (bIsEncrypted)
		{
							return "3 (Encrypted)";
		}
		else
		{
							return "3 (Unencrypted)";
		}
	case IMG_FASTMAN92:		return "fastman92";
	default:				return "Unknown";
	}
}

string				IMGManager::getVersionGames(EIMGVersion eVersion)
{
	switch (eVersion)
	{
	case IMG_1:				return "GTA III / VC";
	case IMG_2:				return "GTA SA";
	case IMG_3:				return "GTA IV";
	case IMG_FASTMAN92:		return "GTA SA Modded";
	default:				return "Unknown";
	}
}

string				IMGManager::getVersionNameWithGames(EIMGVersion eVersion, bool bIsEncrypted)
{
	string strVersionGames = getVersionGames(eVersion);
	return getVersionText(eVersion, bIsEncrypted) + (strVersionGames == "" ? "" : " (" + strVersionGames + ")");
}