#include "IDEFormat.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "IDEEntry.h"
#include "Format/IDE/Entry/IDEEntry_Section.h"
#include "Format/IDE/Entry/IDEEntry_Other.h"
#include "Format/IDE/Entry/DataEntry/IDEEntry_UnknownSection.h"
#include "Format/IDE/Entry/DataEntry/IDEEntry_OBJS.h"
#include "Format/IDE/Entry/DataEntry/IDEEntry_TOBJ.h"
#include "Format/IDE/Entry/DataEntry/IDEEntry_ANIM.h"
#include "Format/IDE/Entry/DataEntry/IDEEntry_PEDS.h"
#include "Format/IDE/Entry/DataEntry/IDEEntry_WEAP.h"
#include "Format/IDE/Entry/DataEntry/IDEEntry_CARS.h"
#include "Format/IDE/Entry/DataEntry/IDEEntry_HIER.h"
#include "Format/IDE/Entry/DataEntry/IDEEntry_TXDP.h"
#include "Format/IDE/Entry/DataEntry/IDEEntry_PATH.h"
#include "Format/IDE/Entry/DataEntry/IDEEntry_HAND.h"
#include "Format/IDE/Entry/DataEntry/IDEEntry_TANM.h"
#include "Format/IDE/Entry/DataEntry/IDEEntry_TREE.h"
#include "Format/IDE/Entry/DataEntry/IDEEntry_VNOD.h"
#include "Format/IDE/Entry/DataEntry/IDEEntry_LINK.h"
#include "Format/IDE/Entry/DataEntry/IDEEntry_MLO.h"
#include "Format/IDE/Entry/DataEntry/IDEEntry_AMAT.h"
#include "Format/IDE/Entry/DataEntry/IDEEntry_LODM.h"
#include "Format/IDE/Entry/DataEntry/IDEEntry_AGRPS.h"
#include "Format/IDE/Entry/DataEntry/2DFX/IDEEntry_2DFX_Light.h"
#include "Format/IDE/Entry/DataEntry/2DFX/IDEEntry_2DFX_Particle.h"
#include "Format/IDE/Entry/DataEntry/2DFX/IDEEntry_2DFX_Ped.h"
#include "Format/IDE/Entry/DataEntry/2DFX/IDEEntry_2DFX_SunReflection.h"
#include "Format/IDE/Entry/DataEntry/2DFX/IDEEntry_2DFX_Unknown1.h"
#include "Format/IDE/Entry/DataEntry/2DFX/IDEEntry_2DFX_Explosion.h"
#include "Format/IDE/Entry/DataEntry/2DFX/IDEEntry_2DFX_Ladder.h"
#include "Format/IDE/Entry/DataEntry/2DFX/IDEEntry_2DFX_SpawnPoint.h"
#include "Format/IDE/Entry/DataEntry/PATH/IDEEntry_PATH_Group.h"
#include "Format/IDE/Entry/DataEntry/PATH/IDEEntry_PATH_Node.h"
#include "Helper/SectionLines/ESectionLinesEntryType.h"
// todo #include "CDataType.h"
#include "Static/String.h"
#include "Exception/EExceptionCode.h"
#include <unordered_map>

using namespace std;
using namespace bxgi;

IDEFormat::IDEFormat(void) :
	SectionLinesFormat()
{
}

// serialization
void				IDEFormat::_unserialize(void)
{
	unserializeText();
}

void				IDEFormat::_serialize(void)
{
	serializeText();
}

IDEEntry_Data*		IDEFormat::unserializeDataEntry(EIDESection EIDESectionValue)
{
	IDEEntry_Data *pIDEEntry = nullptr;
	switch (EIDESectionValue)
	{
	case IDE_SECTION_2DFX:
	{
		pair<E2DFXType, uint32> pairData = detect2DFXTypeAndGame();
		pIDEEntry = createDataEntry(EIDESectionValue, pairData.first);
		pIDEEntry->setFormatGames(pairData.second);
		break;
	}
	case IDE_SECTION_PATH:
		pIDEEntry = createDataEntry(EIDESectionValue, detectPATHType());
		break;
	default:
		pIDEEntry = createDataEntry(EIDESectionValue);
		break;
	}
	if (!pIDEEntry)
	{
		throw EXCEPTION_UNKNOWN_FORMAT;
	}

	return pIDEEntry;
}

// detect 2dfx type and game
pair<E2DFXType, uint32>			IDEFormat::detect2DFXTypeAndGame(void)
{
	DataReader *pDataReader = &m_reader;

	uint32 uiLineTokenCount = pDataReader->getLineTokens().size();

	pDataReader->setActiveLineTokenIndex(4);
	uint32 uiToken5 = pDataReader->readTokenUint32();

	pDataReader->setActiveLineTokenIndex(8);
	uint32 uiToken9 = uiLineTokenCount >= 9 ? pDataReader->readTokenUint32() : -1;

	pDataReader->setActiveLineTokenIndex(0);

	// check for GTA 4 2DFX type - verifying 2dfx type id and token count
	switch (uiToken5)
	{
	case 0:
		if (uiLineTokenCount == 42)
		{
			return{ _2DFX_LIGHT, GAME_FLAG_GTA_IV };
		}
		break;
	case 1:
		if (uiLineTokenCount == 18)
		{
			return{ _2DFX_PARTICLE, GAME_FLAG_GTA_IV };
		}
		break;
	case 2:
		if (uiLineTokenCount == 13)
		{
			return{ _2DFX_EXPLOSION, GAME_FLAG_GTA_IV };
		}
		break;
	case 14:
		if (uiLineTokenCount == 19)
		{
			return{ _2DFX_LADDER, GAME_FLAG_GTA_IV };
		}
		break;
	case 17:
		if (uiLineTokenCount == 13)
		{
			return{ _2DFX_SPAWN_POINT, GAME_FLAG_GTA_IV };
		}
		break;
	}

	// check for GTA 3 era 2DFX type - verifying 2dfx type id and token count
	switch (uiToken9)
	{
	case 0:
		if (uiLineTokenCount == 20)
		{
			return{ _2DFX_LIGHT, GAME_FLAG_GTA_III | GAME_FLAG_GTA_VC };
		}
		break;
	case 1:
		if (uiLineTokenCount == 14)
		{
			return{ _2DFX_PARTICLE, GAME_FLAG_GTA_III | GAME_FLAG_GTA_VC };
		}
		break;
	case 2:
		if (uiLineTokenCount == 14)
		{
			return{ _2DFX_UNKNOWN_1, GAME_FLAG_GTA_III | GAME_FLAG_GTA_VC };
		}
		break;
	case 3:
		if (uiLineTokenCount == 16)
		{
			return{ _2DFX_PED, GAME_FLAG_GTA_VC };
		}
		break;
	case 4:
		if (uiLineTokenCount == 9)
		{
			return{ _2DFX_SUN_REFLECTION, GAME_FLAG_GTA_VC };
		}
		break;
	}

	// check for GTA 4 2DFX type - verifying just the 2dfx type id, because the token count is not documented
	switch (uiToken5)
	{
	case 10: return{ _2DFX_ESCALATOR, GAME_FLAG_GTA_IV };
	case 12: return{ _2DFX_PROC_OBJECT, GAME_FLAG_GTA_IV };
	case 13: return{ _2DFX_WORLD_POINT, GAME_FLAG_GTA_IV };
	case 15: return{ _2DFX_AUDIO, GAME_FLAG_GTA_IV };
	case 18: return{ _2DFX_LIGHT_SHAFT, GAME_FLAG_GTA_IV };
	case 19: return{ _2DFX_SCROLL_BAR, GAME_FLAG_GTA_IV };
	case 21: return{ _2DFX_SWAYABLE_ATTR, GAME_FLAG_GTA_IV };
	case 22: return{ _2DFX_BOUYANCY_ATTR, GAME_FLAG_GTA_IV };
	case 23: return{ _2DFX_WALK_DONT_WALK_ATTR, GAME_FLAG_GTA_IV };
	}

	throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
}

// detect path type
EIDEPathType			IDEFormat::detectPATHType(void)
{
	DataReader *pDataReader = &m_reader;

	switch (pDataReader->getLineTokens().size())
	{
	case 3:		return IDE_PATH_GROUP;
	case 9:		return IDE_PATH_NODE;
	default:	return IDE_PATH_UNKNOWN;
	}
}

// fetch model names
vector<string>				IDEFormat::getModelNamesInSections(vector<EIDESection>& vecIDESections)
{
	vector<string> vecModelSetNames;
	for (EIDESection uiIDESection : vecIDESections)
	{
		vector<vector<IDEEntry*>> vecIDEEntries2 = getSectionsBySection(uiIDESection);
		for (vector<IDEEntry*> vecIDEEntries : vecIDEEntries2)
		{
			for (IDEEntry *pIDEEntry : vecIDEEntries)
			{
				if (pIDEEntry->getEntryType() == SECTION_LINES_ENTRY_DATA)
				{
					vecModelSetNames.push_back(((IDEEntry_Data*)pIDEEntry)->getModelName());
				}
			}
		}
	}
	return vecModelSetNames;
}

// fetch texture set names
vector<string>				IDEFormat::getTextureSetNamesInSections(vector<EIDESection>& vecIDESections)
{
	vector<string> vecTextureSetNames;
	for (EIDESection uiIDESection : vecIDESections)
	{
		vector<vector<IDEEntry*>> vecIDEEntries2 = getSectionsBySection(uiIDESection);
		for (vector<IDEEntry*> vecIDEEntries : vecIDEEntries2)
		{
			for (IDEEntry *pIDEEntry : vecIDEEntries)
			{
				if (pIDEEntry->getEntryType() == SECTION_LINES_ENTRY_DATA)
				{
					vecTextureSetNames.push_back(((IDEEntry_Data*)pIDEEntry)->getTXDName());
				}
			}
		}
	}
	return vecTextureSetNames;
}

// fetch section type
EIDESection					IDEFormat::getSectionFromText(string strIDESectionText)
{
	strIDESectionText = strIDESectionText.substr(0, 5);
	strIDESectionText = String::toUpperCase(strIDESectionText);
	strIDESectionText = String::zeroPad(strIDESectionText, 4);
	strIDESectionText = String::reverse(strIDESectionText);

	const char *pIDESectionTextValue = strIDESectionText.c_str();

	uint32 uiValue = *(const uint32*)pIDESectionTextValue;
	switch (uiValue)
	{
	case 'OBJS':	return IDE_SECTION_OBJS;
	case 'TOBJ':	return IDE_SECTION_TOBJ;
	case 'HIER':	return IDE_SECTION_HIER;
	case 'CARS':	return IDE_SECTION_CARS;
	case 'PEDS':	return IDE_SECTION_PEDS;
	case 'PATH':	return IDE_SECTION_PATH;
	case '2DFX':	return IDE_SECTION_2DFX;
	case 'WEAP':	return IDE_SECTION_WEAP;
	case 'ANIM':	return IDE_SECTION_ANIM;
	case 'TXDP':	return IDE_SECTION_TXDP;
	case 'HAND':	return IDE_SECTION_HAND;
	case 'TANM':	return IDE_SECTION_TANM;
	case 'TREE':	return IDE_SECTION_TREE;
	case 'VNOD':	return IDE_SECTION_VNOD;
	case 'LINK':	return IDE_SECTION_LINK;
	case 'MLO\0':	return IDE_SECTION_MLO;
	case 'AMAT':	return IDE_SECTION_AMAT;
	case 'LODM':	return IDE_SECTION_LODM;
	case 'AGRP':
		if (strIDESectionText.length() >= 5 && *(pIDESectionTextValue + 4) == 'S')
		{
			return IDE_SECTION_AGRPS;
		}
		break;
	}
	return IDE_SECTION_UNKNOWN;
}

// section name
string						IDEFormat::getSectionText(EIDESection EIDESectionValue)
{
	switch (EIDESectionValue)
	{
	case IDE_SECTION_OBJS:	return "OBJS";
	case IDE_SECTION_TOBJ:	return "TOBJ";
	case IDE_SECTION_HIER:	return "HIER";
	case IDE_SECTION_CARS:	return "CARS";
	case IDE_SECTION_PEDS:	return "PEDS";
	case IDE_SECTION_PATH:	return "PATH";
	case IDE_SECTION_2DFX:	return "2DFX";
	case IDE_SECTION_WEAP:	return "WEAP";
	case IDE_SECTION_ANIM:	return "ANIM";
	case IDE_SECTION_TXDP:	return "TXDP";
	case IDE_SECTION_HAND:	return "HAND";
	case IDE_SECTION_TANM:	return "TANM";
	case IDE_SECTION_TREE:	return "TREE";
	case IDE_SECTION_VNOD:	return "VNOD";
	case IDE_SECTION_LINK:	return "LINK";
	case IDE_SECTION_MLO:	return "MLO";
	case IDE_SECTION_AMAT:	return "AMAT";
	case IDE_SECTION_LODM:	return "LODM";
	case IDE_SECTION_AGRPS:	return "AGRPS";
	default:				return "UNKNOWN";
	}
}

// create entry
IDEEntry_Data*				IDEFormat::createDataEntry(EIDESection EIDESectionValue, uint32 uiSectionSpecificType)
{
	switch (EIDESectionValue)
	{
	case IDE_SECTION_OBJS:	return new IDEEntry_OBJS(this);
	case IDE_SECTION_TOBJ:	return new IDEEntry_TOBJ(this);
	case IDE_SECTION_HIER:	return new IDEEntry_HIER(this);
	case IDE_SECTION_CARS:	return new IDEEntry_CARS(this);
	case IDE_SECTION_PEDS:	return new IDEEntry_PEDS(this);
	case IDE_SECTION_PATH:
	{
		EIDEPathType EIDEPathTypeValue = (EIDEPathType)uiSectionSpecificType;
		switch (EIDEPathTypeValue)
		{
		case IDE_PATH_GROUP:	return new IDEEntry_PATH_Group(this);
		case IDE_PATH_NODE:		return new IDEEntry_PATH_Node(this);
		default:				return new IDEEntry_UnknownSection(this);
		}
		break;
	}
	case IDE_SECTION_2DFX:
	{
		E2DFXType E2DFXTypeValue = (E2DFXType)uiSectionSpecificType;
		switch (E2DFXTypeValue)
		{
		case _2DFX_LIGHT:			return new IDEEntry_2DFX_Light(this);
		case _2DFX_PARTICLE:			return new IDEEntry_2DFX_Particle(this);
		case _2DFX_PED:				return new IDEEntry_2DFX_Ped(this);
		case _2DFX_SUN_REFLECTION:	return new IDEEntry_2DFX_SunReflection(this);
		case _2DFX_UNKNOWN_1:		return new IDEEntry_2DFX_Unknown1(this);
		default:						return new IDEEntry_UnknownSection(this);
		}
		break;
	}
	case IDE_SECTION_WEAP:	return new IDEEntry_WEAP(this);
	case IDE_SECTION_ANIM:	return new IDEEntry_ANIM(this);
	case IDE_SECTION_TXDP:	return new IDEEntry_TXDP(this);
	case IDE_SECTION_HAND:	return new IDEEntry_HAND(this);
	case IDE_SECTION_TANM:	return new IDEEntry_TANM(this);
	case IDE_SECTION_TREE:	return new IDEEntry_TREE(this);
	case IDE_SECTION_VNOD:	return new IDEEntry_VNOD(this);
	case IDE_SECTION_LINK:	return new IDEEntry_LINK(this);
	case IDE_SECTION_MLO:	return new IDEEntry_MLO(this);
	case IDE_SECTION_AMAT:	return new IDEEntry_AMAT(this);
	case IDE_SECTION_LODM:	return new IDEEntry_LODM(this);
	case IDE_SECTION_AGRPS:	return new IDEEntry_AGRPS(this);
	default:				return new IDEEntry_UnknownSection(this);
	}
	return nullptr;
}

// detect section-specific type
uint32						IDEFormat::detectSectionSpecificType(EIDESection EIDESectionValue)
{
	switch (EIDESectionValue)
	{
	case IDE_SECTION_2DFX:	return detect2DFXTypeAndGame().first;
	case IDE_SECTION_PATH:	return detectPATHType();
	}
	return 0;
}

// format games
uint32						IDEFormat::getFormatGames(void)
{
	vector<EIDESection> vecIDESections = {
		IDE_SECTION_OBJS,
		IDE_SECTION_TOBJ,
		IDE_SECTION_HIER,
		IDE_SECTION_CARS,
		IDE_SECTION_PEDS,
		IDE_SECTION_PATH,
		IDE_SECTION_2DFX,
		IDE_SECTION_WEAP,
		IDE_SECTION_ANIM,
		IDE_SECTION_TXDP,
		IDE_SECTION_HAND,
		IDE_SECTION_TANM,
		IDE_SECTION_TREE,
		IDE_SECTION_VNOD,
		IDE_SECTION_LINK,
		IDE_SECTION_MLO,
		IDE_SECTION_AMAT,
		IDE_SECTION_LODM,
		IDE_SECTION_AGRPS
	};
	for (EIDESection uiIDESection : vecIDESections)
	{
		if (getSectionEntries()[uiIDESection].size() > 0)
		{
			return ((IDEEntry_Data*)getSectionEntries()[uiIDESection][0])->getFormatGames();
		}
	}
	return 0;
}