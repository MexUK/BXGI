#pragma once

#include "nsbxgi.h"
#include "Format/IDE/Entry/DataEntry/CIDEEntry_PATH.h"
#include "Type/Vector/Vec3f.h"

class bxgi::CIDEEntry_PATH_Node : public bxgi::CIDEEntry_PATH
{
public:
	CIDEEntry_PATH_Node(bxgi::CIDEFormat *pIDEFormat) :
		CIDEEntry_PATH(pIDEFormat, IDE_PATH_NODE),
		m_uiNodeType(0),
		m_uiNextNode(0),
		m_uiIsCrossRoad(0),
		m_vecPosition{ 0.0f, 0.0f, 0.0f },
		m_fUnknown1(0.0f),
		m_uiLeftLaneCount(0),
		m_uiRightLaneCount(0)
	{
	};

	void							unserialize(void);
	void							serialize(void);

	void							setNodeType(uint32 uiNodeType) { m_uiNodeType = uiNodeType; }
	uint32							getNodeType(void) { return m_uiNodeType; }

	void							setNextNode(uint32 uiNextNode) { m_uiNextNode = uiNextNode; }
	uint32							getNextNode(void) { return m_uiNextNode; }

	void							setIsCrossRoad(uint32 uiIsCrossRoad) { m_uiIsCrossRoad = uiIsCrossRoad; }
	uint32							isCrossRoad(void) { return m_uiIsCrossRoad; }

	void							setPosition(bxcf::Vec3f& vecPosition) { m_vecPosition = vecPosition; }
	bxcf::Vec3f&					getPosition(void) { return m_vecPosition; }

	void							setUnknown1(float32 fUnknown1) { m_fUnknown1 = fUnknown1; }
	float32							getUnknown1(void) { return m_fUnknown1; }

	void							setLeftLaneCount(uint32 uiLeftLaneCount) { m_uiLeftLaneCount = uiLeftLaneCount; }
	uint32							getLeftLaneCount(void) { return m_uiLeftLaneCount; }

	void							setRightLaneCount(uint32 uiRightLaneCount) { m_uiRightLaneCount = uiRightLaneCount; }
	uint32							getRightLaneCount(void) { return m_uiRightLaneCount; }

private:
	// GTA III era only
	uint32							m_uiNodeType;
	uint32							m_uiNextNode;
	uint32							m_uiIsCrossRoad;
	bxcf::Vec3f						m_vecPosition;
	float32							m_fUnknown1;
	uint32							m_uiLeftLaneCount;
	uint32							m_uiRightLaneCount;
};