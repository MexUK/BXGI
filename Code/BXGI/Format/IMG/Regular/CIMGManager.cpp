#include "CIMGManager.h"
#include "CIMGFormat.h"
#include "CIMGEntry.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"
#include "Static/CString2.h"
#include "Static/CPath.h"
#include "Static/CFile.h"
#include "Encryption/AES/Rijndael/Rijndael.h"
#include "Engine/RAGE/CRageManager.h"
#include "Engine/RAGE/CRageResourceType.h"
#include "Engine/RAGE/CRageResourceTypeManager.h"
#include "Static/CDebug.h"
#include "Format/COL/CCOLManager.h"
#include "Compression/eCompressionAlgorithm.h"
#include "Compression/CCompressionManager.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

bool g_bDefaultBool = false;

CIMGManager::CIMGManager(void)
{
}

void			CIMGManager::init(void)
{
}
void			CIMGManager::uninit(void)
{
}

// utility
string			CIMGManager::getVersionName(eIMGVersion eVersion, bool bIsEncrypted)
{
	switch (eVersion)
	{
	case IMG_1:				return "1";
	case IMG_2:				return "2";
	case IMG_3:
		if (bIsEncrypted)
		{
			return "3 [Encrypted]";
		}
		else
		{
			return "3 [Unencrypted]";
		}
	case IMG_FASTMAN92:		return "fastman92";
	default:				return "Unknown";
	}
}
string			CIMGManager::getVersionGames(eIMGVersion eVersion)
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
string			CIMGManager::getVersionNameWithGames(eIMGVersion eVersion, bool bIsEncrypted)
{
	string strVersionGames = getVersionGames(eVersion);
	return getVersionName(eVersion, bIsEncrypted) + (strVersionGames == "" ? "" : " (" + strVersionGames + ")");
}

eIMGVersion		CIMGManager::detectIMGVersion(string& strIMGFilePath, string& strHeader16B, bool& bIsEncryptedOut)
{
	bIsEncryptedOut = false;

	// strIMGFilePath can be a path to an IMG file or a DIR file
	if (!CFile::doesFileExist(strIMGFilePath))
	{
		return IMG_UNKNOWN;
	}
	
	string strFileExtensionUpper = CString2::toUpperCase(CPath::getFileExtension(strIMGFilePath));
	if (strFileExtensionUpper == "DIR")
	{
		// input file is: DIR file
		if (CFile::doesFileExist(CPath::replaceFileExtension(strIMGFilePath, "img")))
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
			uiFileSize = CFile::getFileSize(strIMGFilePath);
		}
		if (strHeader16B == "")
		{
			if (uiFileSize >= 16)
			{
				strHeader16B = CFile::getFileSubContent(strIMGFilePath, 0, 16, true);
			}
		}
		if (strHeader16B == "")
		{
			if (uiFileSize >= 4)
			{
				strHeader4B = CFile::getFileSubContent(strIMGFilePath, 0, 4, true);
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
		else if (strHeader4B.length() == 4 && CString2::unpackUint32(strHeader4B, false) == 0xA94E2A52)
		{
			return IMG_3; // unencrypted
		}
		else if (strHeader16B.length() == 16 && CString2::unpackUint32(CIMGManager::decryptVersion3IMGString(strHeader16B).substr(0, 4), false) == 0xA94E2A52)
		{
			bIsEncryptedOut = true;
			return IMG_3; // encrypted
		}
		else
		{
			string strDIRFilePath = CPath::replaceFileExtension(strIMGFilePath, "dir");
			if (CFile::doesFileExist(strDIRFilePath))
			{
				return IMG_1;
			}
		}
	}

	return IMG_UNKNOWN;
}

bool			CIMGManager::detectIMGEncryptionState(std::string& strIMGFilePath)
{
	string strHeader16B = CFile::getFileSubContent(strIMGFilePath, 0, 16, true);
	string strHeader4B = strHeader16B.substr(0, 4);

	// version 3
	if (CString2::unpackUint32(CIMGManager::decryptVersion3IMGString(strHeader16B).substr(0, 4), false) == 0xA94E2A52)
	{
		return true;
	}

	// version fastman92
	if (((CString2::toNumber(strHeader16B.substr(4, 1)) >> 24) & 15) != 0)
	{
		return true;
	}

	return false;
}

uint32	CIMGManager::getIMGEntryCount(std::string& strIMGFilePath, eIMGVersion eVersion)
{
	if (eVersion == IMG_UNKNOWN)
	{
		eVersion = detectIMGVersion(strIMGFilePath);
	}

	if (eVersion == IMG_1)
	{
		return CFile::getFileSize(CPath::replaceFileExtension(strIMGFilePath, "dir")) / 32;
	}
	else if (eVersion == IMG_2)
	{
		return CString2::unpackUint32(CFile::getFileSubContent(strIMGFilePath, 0, 8, true).substr(4, 4), false);
	}
	else if (eVersion == IMG_3)
	{
		if (detectIMGEncryptionState(strIMGFilePath))
		{
			string strHeader16B = CFile::getFileSubContent(strIMGFilePath, 0, 16, true);
			return CString2::unpackUint32(CIMGManager::decryptVersion3IMGString(strHeader16B).substr(8, 4), false);
		}
		else
		{
			return CString2::unpackUint32(CFile::getFileSubContent(strIMGFilePath, 0, 12, true).substr(8, 4), false);
		}
	}
	else if (eVersion == IMG_FASTMAN92)
	{
		string strHeader28B = CFile::getFileSubContent(strIMGFilePath, 0, 28, true);
		return CString2::unpackUint32(strHeader28B.substr(24, 4), false);
	}
	else
	{
		return 0;
	}
}

string			CIMGManager::encryptVersion3IMGString(string strData)
{
	for (uint32 i = 0; i < 16; i++)
	{
		CRijndael enc;
		enc.MakeKey("\x1a\xb5\x6f\xed\x7e\xc3\xff\x1\x22\x7b\x69\x15\x33\x97\x5d\xce\x47\xd7\x69\x65\x3f\xf7\x75\x42\x6a\x96\xcd\x6d\x53\x7\x56\x5d",
			"\x1a\xb5\x6f\xed\x7e\xc3\xff\x1\x22\x7b\x69\x15\x33\x97\x5d\xce\x47\xd7\x69\x65\x3f\xf7\x75\x42\x6a\x96\xcd\x6d\x53\x7\x56\x5d",
			32,
			16);

		char *szOutData = new char[strData.length() + 1];
		//enc.DecryptBlock(strData.c_str(), szOutData);
		enc.Encrypt(strData.c_str(), szOutData, strData.length(), CRijndael::ECB);
		szOutData[strData.length()] = '\0';
		uint32 uiLength = strData.length();
		strData = "";
		strData.resize(0);
		strData.append(szOutData, uiLength);
		delete[] szOutData;
	}
	return strData;
}
string			CIMGManager::decryptVersion3IMGString(string strData)
{
	///*
	char sz[1024];
	sprintf_s(sz, "strData length: %u", strData.length());
	//CDebug::log(sz);

	/*
	for (uint32 i = 0; i < 16; i++)
	{
		CRijndael enc;
		enc.MakeKey("\x1a\xb5\x6f\xed\x7e\xc3\xff\x1\x22\x7b\x69\x15\x33\x97\x5d\xce\x47\xd7\x69\x65\x3f\xf7\x75\x42\x6a\x96\xcd\x6d\x53\x7\x56\x5d",
			"\x1a\xb5\x6f\xed\x7e\xc3\xff\x1\x22\x7b\x69\x15\x33\x97\x5d\xce\x47\xd7\x69\x65\x3f\xf7\x75\x42\x6a\x96\xcd\x6d\x53\x7\x56\x5d",
			32,
			16);
		char *szOutData = new char[strData.length() + 1];
		uint32 uiDataLength = strData.length();
		string strNewData = "";
		for (uint32 i2 = 0; i2 < (strData.length() / 16); i2++)
		{
			string strSubData = strData.substr(i2 * 16, 16);
			enc.DecryptBlock(strSubData.c_str(), szOutData);
			//enc.Decrypt(strData.c_str(), szOutData, strData.length(), CRijndael::ECB);
			//szOutData[strData.length()] = '\0';
			strNewData.append(szOutData, 16);
		}
		strData = "";
		strData.append(strNewData.c_str(), uiDataLength);
		delete[] szOutData;
	}
	*/
	///*
	for (uint32 i = 0; i < 16; i++)
	{
		CRijndael enc;
		enc.MakeKey("\x1a\xb5\x6f\xed\x7e\xc3\xff\x1\x22\x7b\x69\x15\x33\x97\x5d\xce\x47\xd7\x69\x65\x3f\xf7\x75\x42\x6a\x96\xcd\x6d\x53\x7\x56\x5d",
			"\x1a\xb5\x6f\xed\x7e\xc3\xff\x1\x22\x7b\x69\x15\x33\x97\x5d\xce\x47\xd7\x69\x65\x3f\xf7\x75\x42\x6a\x96\xcd\x6d\x53\x7\x56\x5d",
			32,
			16);

		char *szOutData = new char[strData.length() + 1];
		//enc.DecryptBlock(strData.c_str(), szOutData);
		enc.Decrypt(strData.c_str(), szOutData, strData.length(), CRijndael::ECB);
		szOutData[strData.length()] = '\0';
		uint32 uiLength = strData.length();
		strData = "";
		strData.resize(0);
		strData.append(szOutData, uiLength);
		delete [] szOutData;
	}
	//*/
	return strData;
	//*/
	/*
	strData = strData.substr(0, 20);
	string recovered;

	for (uint32 i = 0; i < 16; i++)
	{
		CDebug::log("DECRYPTION LOOP");
		AutoSeededRandomPool prng;

		//byte* key_s = (byte*)"\x1a\xb5\x6f\xed\x7e\xc3\xff\x1\x22\x7b\x69\x15\x33\x97\x5d\xce\x47\xd7\x69\x65\x3f\xf7\x75\x42\x6a\x96\xcd\x6d\x53\x7\x56\x5d";
		//SecByteBlock key(key_s, 32);
		byte key[33] = "\x1a\xb5\x6f\xed\x7e\xc3\xff\x1\x22\x7b\x69\x15\x33\x97\x5d\xce\x47\xd7\x69\x65\x3f\xf7\x75\x42\x6a\x96\xcd\x6d\x53\x7\x56\x5d";
		//prng.GenerateBlock(key, 32);
		
		try
		{
			ECB_Mode< AES >::Decryption d;
			d.SetKey(key, 32);

			// The StreamTransformationFilter removes
			//  padding as required.
			StringSource s(strData, true,
				//new CryptoPP::HexDecoder(
					new StreamTransformationFilter(d,
						new StringSink(recovered)
					) // StreamTransformationFilter
				//)
			); // StringSource
		}
		catch (const CryptoPP::Exception& e)
		{
			CDebug::log(e.what());
			exit(1);
		}

		strData = recovered;
	}

	return recovered;
	*/
}

vector<string>		CIMGManager::getDefaultGameIMGSubPaths(ePlatformedGame ePlatformedGameValue)
{
	vector<string> vecGameIMGPaths;
	switch (ePlatformedGameValue)
	{
	case PLATFORMED_GAME_PC_GTA_III:
		vecGameIMGPaths.push_back("models/gta3.img");
		vecGameIMGPaths.push_back("models/txd.img");
		break;
	case PLATFORMED_GAME_PC_GTA_VC:
		vecGameIMGPaths.push_back("models/gta3.img");
		vecGameIMGPaths.push_back("anim/cuts.img");
		break;
	case PLATFORMED_GAME_PC_GTA_SA:
		vecGameIMGPaths.push_back("models/gta3.img");
		vecGameIMGPaths.push_back("models/cutscene.img");
		vecGameIMGPaths.push_back("models/gta_int.img");
		vecGameIMGPaths.push_back("models/player.img");
		vecGameIMGPaths.push_back("data/Paths/carrec.img");
		vecGameIMGPaths.push_back("data/script/script.img");
		vecGameIMGPaths.push_back("anim/anim.img");
		vecGameIMGPaths.push_back("anim/cuts.img");
		break;
	case PLATFORMED_GAME_PC_SOL:
		vecGameIMGPaths.push_back("models/game.img");
		break;
	}
	return vecGameIMGPaths;
}

string				CIMGManager::getCompressionTypeText(eCompressionAlgorithm eCompressionAlgorithmValue)
{
	switch (eCompressionAlgorithmValue)
	{
	case COMPRESSION_NONE:		return "Not Compressed";
	case COMPRESSION_ZLIB:		return "ZLib";
	case COMPRESSION_LZ4:		return "LZ4";
	case COMPRESSION_UNKNOWN:
	default:
		return "Unknown Compression Type";
	}
}
string				CIMGManager::getEncryptionText(bool bIsEncrypted)
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

CIMGFormat*			CIMGManager::parseViaFile(string& strFilePath)
{
	return CFormatManager::parseViaFile(strFilePath);
}