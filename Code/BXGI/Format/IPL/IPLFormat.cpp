#include "IPLFormat.h"
#include "Format/IPL/IPLEntry.h"
#include "Helper/SectionLines/ESectionLinesEntryType.h"
#include "Entry/DataEntry/IPLEntry_INST.h"
#include "Entry/DataEntry/IPLEntry_ZONE.h"
#include "Entry/DataEntry/IPLEntry_CULL.h"
#include "Entry/DataEntry/IPLEntry_PICK.h"
#include "Entry/DataEntry/IPLEntry_PATH.h"
#include "Entry/DataEntry/IPLEntry_OCCL.h"
#include "Entry/DataEntry/IPLEntry_MULT.h"
#include "Entry/DataEntry/IPLEntry_GRGE.h"
#include "Entry/DataEntry/IPLEntry_ENEX.h"
#include "Entry/DataEntry/IPLEntry_CARS.h"
#include "Entry/DataEntry/IPLEntry_JUMP.h"
#include "Entry/DataEntry/IPLEntry_TCYC.h"
#include "Entry/DataEntry/IPLEntry_AUZO.h"
#include "Entry/DataEntry/IPLEntry_MZON.h"
#include "Entry/DataEntry/IPLEntry_VNOD.h"
#include "Entry/DataEntry/IPLEntry_LINK.h"
#include "Entry/DataEntry/IPLEntry_BLOK.h"
#include "Entry/DataEntry/IPLEntry_MLOPLUS.h"
#include "Entry/DataEntry/IPLEntry_2DFX.h"
#include "Entry/DataEntry/IPLEntry_LODM.h"
#include "Entry/DataEntry/IPLEntry_SLOW.h"
#include "Entry/DataEntry/IPLEntry_RTFX.h"
#include "Entry/DataEntry/IPLEntry_UnknownSection.h"
#include "Entry/DataEntry/PATH/IPLEntry_PATH_Group.h"
#include "Entry/DataEntry/PATH/IPLEntry_PATH_Node.h"
#include "Static/String.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Exception/EExceptionCode.h"
#include <vector>

using namespace std;
using namespace bxcf;
using namespace bxgi;

IPLFormat::IPLFormat(void) :
	SectionLinesFormat(),
	m_bIsBinary(false)
{
}

// serialization
void		IPLFormat::_unserialize(void)
{
	if (isBinary())
	{
		// binary IPL file
		return unserializeBinary();
	}
	else
	{
		// text IPL file
		return unserializeText();
	}
}

void		IPLFormat::_serialize(void)
{
	if (isBinary())
	{
		// binary IPL file
		return serializeBinary();
	}
	else
	{
		// text IPL file
		return serializeText();
	}
}

void		IPLFormat::unserializeBinary(void)
{
	DataReader *pDataReader = DataReader::get();

	// header
	string strFourCC = pDataReader->readString(4);
	uint32 uiEntryCount_INST = pDataReader->readUint32();
	uint32 uiEntryCount_Unknown1 = pDataReader->readUint32();
	uint32 uiEntryCount_Unknown2 = pDataReader->readUint32();
	uint32 uiEntryCount_Unknown3 = pDataReader->readUint32();
	uint32 uiEntryCount_CARS = pDataReader->readUint32();
	uint32 uiEntryCount_Unknown4 = pDataReader->readUint32();
	uint32 uiSectionOffset_INST = pDataReader->readUint32();
	uint32 uiSectionSizeDummy_INST = pDataReader->readUint32();
	uint32 uiSectionOffset_Unknown1 = pDataReader->readUint32();
	uint32 uiSectionSizeDummy_Unknown1 = pDataReader->readUint32();
	uint32 uiSectionOffset_Unknown2 = pDataReader->readUint32();
	uint32 uiSectionSizeDummy_Unknown2 = pDataReader->readUint32();
	uint32 uiSectionOffset_Unknown3 = pDataReader->readUint32();
	uint32 uiSectionSizeDummy_Unknown3 = pDataReader->readUint32();
	uint32 uiSectionOffset_CARS = pDataReader->readUint32();
	uint32 uiSectionSizeDummy_CARS = pDataReader->readUint32();
	uint32 uiSectionOffset_Unknown4 = pDataReader->readUint32();
	uint32 uiSectionSizeDummy_Unknown4 = pDataReader->readUint32();

	// entries - INST
	pDataReader->setSeek(uiSectionOffset_INST);
	for (uint32 i = 0; i < uiEntryCount_INST; i++)
	{
		IPLEntry_INST *pIPLEntry_INST = new IPLEntry_INST(this);
		pIPLEntry_INST->unserialize();
		getSectionEntries()[IPL_SECTION_INST].push_back(pIPLEntry_INST);
	}

	//RG_IPLEntry_INST RG_IPLEntries = pDataReader->readStruct<RG_IPLEntry_INST>();

	// entries - CARS
	pDataReader->setSeek(uiSectionOffset_CARS);
	for (uint32 i = 0; i < uiSectionOffset_CARS; i++)
	{
		IPLEntry_CARS *pIPLEntry_CARS = new IPLEntry_CARS(this);
		pIPLEntry_CARS->unserialize();
		getSectionEntries()[IPL_SECTION_CARS].push_back(pIPLEntry_CARS);
	}
}

void		IPLFormat::serializeBinary(void)
{
	DataWriter *pDataWriter = DataWriter::get();

	// header
	string strFourCC = "BNRY";
	vector<vector<IPLEntry*>> vecSections_INST = getSectionsBySection(IPL_SECTION_INST);
	vector<vector<IPLEntry*>> vecSections_CARS = getSectionsBySection(IPL_SECTION_CARS);
	uint32
		uiEntryCount_INST = 0,
		uiEntryCount_CARS = 0;

	for (auto& vecIPLEntries : vecSections_INST)
	{
		uiEntryCount_INST += vecIPLEntries.size();
	}
	for (auto& vecIPLEntries : vecSections_CARS)
	{
		uiEntryCount_CARS += vecIPLEntries.size();
	}

	uint32
		uiSectionOffset_INST = 76,
		uiSectionOffset_CARS = uiSectionOffset_INST + (uiEntryCount_INST * 40);

	pDataWriter->writeStringRef(strFourCC);
	pDataWriter->writeUint32(uiEntryCount_INST);
	pDataWriter->writeUint32(0);
	pDataWriter->writeUint32(0);
	pDataWriter->writeUint32(0);
	pDataWriter->writeUint32(uiEntryCount_CARS);
	pDataWriter->writeUint32(0);
	pDataWriter->writeUint32(uiSectionOffset_INST);
	pDataWriter->writeUint32(uiSectionOffset_INST);
	pDataWriter->writeUint32(0);
	pDataWriter->writeUint32(0);
	pDataWriter->writeUint32(0);
	pDataWriter->writeUint32(0);
	pDataWriter->writeUint32(0);
	pDataWriter->writeUint32(0);
	pDataWriter->writeUint32(uiSectionOffset_CARS);
	pDataWriter->writeUint32(uiSectionOffset_CARS);
	pDataWriter->writeUint32(0);
	pDataWriter->writeUint32(0);

	// entries - INST
	for (auto& vecIPLEntries : vecSections_INST)
	{
		for (auto pIPLEntry_INST : vecIPLEntries)
		{
			pIPLEntry_INST->serialize();
		}
	}

	// entries - CARS
	for (auto& vecIPLEntries : vecSections_CARS)
	{
		for (auto pIPLEntry_CARS : vecIPLEntries)
		{
			pIPLEntry_CARS->serialize();
		}
	}
}

IPLEntry_Data*		IPLFormat::unserializeDataEntry(EIPLSection EIPLSectionValue)
{
	IPLEntry_Data *pIPLEntry = nullptr;
	switch (EIPLSectionValue)
	{
	case IPL_SECTION_PATH:
		pIPLEntry = createDataEntry(EIPLSectionValue, detectPATHType());
		break;
	default:
		pIPLEntry = createDataEntry(EIPLSectionValue);
		break;
	}
	if (!pIPLEntry)
	{
		throw EXCEPTION_UNKNOWN_FORMAT;
	}

	return pIPLEntry;
}

// detect path type
EIPLPathType		IPLFormat::detectPATHType(void)
{
	DataReader *pDataReader = DataReader::get();

	switch (pDataReader->getLineTokens().size())
	{
	case 2:		return IPL_PATH_GROUP;
	case 12:	return IPL_PATH_NODE;
	default:	return IPL_PATH_UNKNOWN;
	}
}

// fetch section type
EIPLSection					IPLFormat::getSectionFromText(string strIPLSectionText)
{
	strIPLSectionText = strIPLSectionText.substr(0, 5);
	strIPLSectionText = String::toUpperCase(strIPLSectionText);
	strIPLSectionText = String::zeroPad(strIPLSectionText, 4);
	strIPLSectionText = String::reverse(strIPLSectionText);

	const char *pIPLSectionTextValue = strIPLSectionText.c_str();

	uint32 uiValue = *(const uint32*)pIPLSectionTextValue;
	switch (uiValue)
	{
	case 'INST':	return IPL_SECTION_INST;
	case 'ZONE':	return IPL_SECTION_ZONE;
	case 'CULL':	return IPL_SECTION_CULL;
	case 'PICK':	return IPL_SECTION_PICK;
	case 'PATH':	return IPL_SECTION_PATH;
	case 'OCCL':	return IPL_SECTION_OCCL;
	case 'MULT':	return IPL_SECTION_MULT;
	case 'GRGE':	return IPL_SECTION_GRGE;
	case 'ENEX':	return IPL_SECTION_ENEX;
	case 'CARS':	return IPL_SECTION_CARS;
	case 'JUMP':	return IPL_SECTION_JUMP;
	case 'TCYC':	return IPL_SECTION_TCYC;
	case 'AUZO':	return IPL_SECTION_AUZO;
	case 'MZON':	return IPL_SECTION_MZON;
	case 'VNOD':	return IPL_SECTION_VNOD;
	case 'LINK':	return IPL_SECTION_LINK;
	case 'BLOK':	return IPL_SECTION_BLOK;
	case 'MLO+':	return IPL_SECTION_MLOPLUS;
	case '2DFX':	return IPL_SECTION_2DFX;
	case 'LODM':	return IPL_SECTION_LODM;
	case 'SLOW':	return IPL_SECTION_SLOW;
	case 'RTFX':	return IPL_SECTION_RTFX;
	}
	return IPL_SECTION_UNKNOWN;
}

// section name
string						IPLFormat::getSectionText(EIPLSection EIPLSectionValue)
{
	switch (EIPLSectionValue)
	{
	case IPL_SECTION_INST:		return "INST";
	case IPL_SECTION_ZONE:		return "ZONE";
	case IPL_SECTION_CULL:		return "CULL";
	case IPL_SECTION_PICK:		return "PICK";
	case IPL_SECTION_PATH:		return "PATH";
	case IPL_SECTION_OCCL:		return "OCCL";
	case IPL_SECTION_MULT:		return "MULT";
	case IPL_SECTION_GRGE:		return "GRGE";
	case IPL_SECTION_ENEX:		return "ENEX";
	case IPL_SECTION_CARS:		return "CARS";
	case IPL_SECTION_JUMP:		return "JUMP";
	case IPL_SECTION_TCYC:		return "TCYC";
	case IPL_SECTION_AUZO:		return "AUZO";
	case IPL_SECTION_MZON:		return "MZON";
	case IPL_SECTION_VNOD:		return "VNOD";
	case IPL_SECTION_LINK:		return "LINK";
	case IPL_SECTION_BLOK:		return "BLOK";
	case IPL_SECTION_MLOPLUS:	return "MLO+";
	case IPL_SECTION_2DFX:		return "2DFX";
	case IPL_SECTION_LODM:		return "LODM";
	case IPL_SECTION_SLOW:		return "SLOW";
	case IPL_SECTION_RTFX:		return "RTFX";
	}
	return "Unknown IPL Section";
}

// create entry
IPLEntry_Data*				IPLFormat::createDataEntry(EIPLSection EIPLSectionValue, uint32 uiSectionSpecificType)
{
	switch (EIPLSectionValue)
	{
	case IPL_SECTION_INST:		return new IPLEntry_INST(this);
	case IPL_SECTION_ZONE:		return new IPLEntry_ZONE(this);
	case IPL_SECTION_CULL:		return new IPLEntry_CULL(this);
	case IPL_SECTION_PICK:		return new IPLEntry_PICK(this);
	case IPL_SECTION_PATH:
	{
		EIPLPathType EIPLPathTypeValue = (EIPLPathType)uiSectionSpecificType;
		switch (EIPLPathTypeValue)
		{
		case IPL_PATH_GROUP:	return new IPLEntry_PATH_Group(this);
		case IPL_PATH_NODE:		return new IPLEntry_PATH_Node(this);
		}
		break;
	}
	case IPL_SECTION_OCCL:		return new IPLEntry_OCCL(this);
	case IPL_SECTION_MULT:		return new IPLEntry_MULT(this);
	case IPL_SECTION_GRGE:		return new IPLEntry_GRGE(this);
	case IPL_SECTION_ENEX:		return new IPLEntry_ENEX(this);
	case IPL_SECTION_CARS:		return new IPLEntry_CARS(this);
	case IPL_SECTION_JUMP:		return new IPLEntry_JUMP(this);
	case IPL_SECTION_TCYC:		return new IPLEntry_TCYC(this);
	case IPL_SECTION_AUZO:		return new IPLEntry_AUZO(this);
	case IPL_SECTION_MZON:		return new IPLEntry_MZON(this);
	case IPL_SECTION_VNOD:		return new IPLEntry_VNOD(this);
	case IPL_SECTION_LINK:		return new IPLEntry_LINK(this);
	case IPL_SECTION_BLOK:		return new IPLEntry_BLOK(this);
	case IPL_SECTION_MLOPLUS:	return new IPLEntry_MLOPLUS(this);
	case IPL_SECTION_2DFX:		return new IPLEntry_2DFX(this);
	case IPL_SECTION_LODM:		return new IPLEntry_LODM(this);
	case IPL_SECTION_SLOW:		return new IPLEntry_SLOW(this);
	case IPL_SECTION_RTFX:		return new IPLEntry_RTFX(this);
	default:					return new IPLEntry_UnknownSection(this);
	}
	return nullptr;
}

// detect section-specific type
uint32						IPLFormat::detectSectionSpecificType(EIPLSection EIPLSectionValue)
{
	switch (EIPLSectionValue)
	{
	case IPL_SECTION_PATH:	return detectPATHType();
	}
	return 0;
}