#pragma once

#include "nsbxgi.h"
#include "Type/Vector/Vec3f.h"
#include "Format/RW/RWSection.h"
#include <string>
#include <vector>

class bxgi::RWSection_FrameList : public bxgi::RWSection
{
public:
	bxgi::RWSection_FrameList(void);

	void							_unserialize(void);
	void							_serialize(void);

	void							setFrameCount(uint32 uiFrameCount) { m_uiFrameCount = uiFrameCount; }
	uint32							getFrameCount(void) { return m_uiFrameCount; }

	void							setFrameData(std::string& strFrameData) { m_strFrameData = strFrameData; }
	std::string&					getFrameData(void) { return m_strFrameData; }

public:
	std::vector<bxcf::Vec3f>		m_vecMatRow1;
	std::vector<bxcf::Vec3f>		m_vecMatRow2;
	std::vector<bxcf::Vec3f>		m_vecMatRow3;
	std::vector<bxcf::Vec3f>		m_vecPosition;
	std::vector<uint32>				m_uiCurrentFrameIndex;
	std::vector<uint32>				m_uiMatrixCreationFlags;

private:
	uint32							m_uiFrameCount;
	std::string						m_strFrameData;
};