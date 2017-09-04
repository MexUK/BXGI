#pragma once

#include "nsbxgi.h"
#include "Format/Format.h"
#include "Format/DAT/Path/EDATPathFormat.h"
#include "Format/DAT/Path/DATPathEntry.h"
#include <string>
#include <vector>

class bxcf::Vec3f;

class bxgi::DATPathFormat : public bxcf::Format
{
public:
	DATPathFormat(void);
	
	void												unload(void);

	void												setPathsFormat(bxgi::EDATPathFormat eFormat) { m_header.m_uiPathsFormat = eFormat; }
	bxgi::EDATPathFormat								getPathsFormat(void) { return m_header.m_uiPathsFormat; }

	void												applyOffsetToPositions(bxcf::Vec3f& vecPositionOffset);

	void												addPathNode(DATEntry_Paths_General_PathNode pathNode);
	void												addNaviNode(DATEntry_Paths_General_NaviNode naviNode);
	void												addLink(DATEntry_Paths_General_Link link);

	bxgi::DATEntry_Paths_General_Header&				getHeader(void) { return m_header; }
	std::vector<DATEntry_Paths_General_PathNode>&		getPathNodes(void) { return m_vecPathNodes; }
	std::vector<DATEntry_Paths_General_NaviNode>&		getNaviNodes(void) { return m_vecNaviNodes; }
	std::vector<DATEntry_Paths_General_Link>&			getLinks(void) { return m_vecLinks; }

	/*
	todo - [after DAT Paths rewrite] these 3 lines still needed?
	DATPathFormat*			unserializeViaData_Paths(std::string strEntryData, uint32 uiAreaId, EDATPathFormat EDATPathFormatValue = DAT_PATH_UNKNOWN);
	DATPathFormat*			parseViaData_Paths_Default(std::string strEntryData, uint32 uiAreaId);
	DATPathFormat*			parseViaData_Paths_Fastman92(std::string strEntryData, uint32 uiAreaId);
	*/

private:
	void												unserialize(void);
	void												serialize(void);

	void												unserializeDefault(void);
	void												unserializeFastman92(void);

	void												serializeDefault(void);
	void												serializeFastman92(void); 
	
	bxgi::EDATPathFormat								detectPathsFormat(void);

public: // todo - make this access specifier private
	uint32													m_uiFileIndex;
	bxgi::DATEntry_Paths_General_Header						m_header;
	std::vector<bxgi::DATEntry_Paths_General_PathNode>		m_vecPathNodes;
	std::vector<bxgi::DATEntry_Paths_General_NaviNode>		m_vecNaviNodes;
	std::vector<bxgi::DATEntry_Paths_General_Link>			m_vecLinks;
};