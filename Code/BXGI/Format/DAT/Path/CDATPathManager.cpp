#include "CDATPathManager.h"
#include "CDATPathFormat.h"
#include "Static/CString2.h"
#include "Static/CStdVector.h"
#include "Static/CDebug.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

CDATPathManager::CDATPathManager(void)
{
}

void				CDATPathManager::init(void)
{
}
void				CDATPathManager::uninit(void)
{
}

uint32				CDATPathManager::getAreaIdFromPosition(Vec2f& vecPosition, Vec2f& vecMapRangeMin, Vec2f& vecMapRangeMax, Vec2f& vecAreaSize)
{
	//vecPosition.y = -vecPosition.y;
	float32 fMapWidth = fabs(vecMapRangeMin.x) + fabs(vecMapRangeMax.x);
	float32 fMapHeight = fabs(vecMapRangeMin.y) + fabs(vecMapRangeMax.y);
	uint32 uiAreaCount[2] = { (uint32)(fMapWidth / vecAreaSize.x), (uint32)(fMapHeight / vecAreaSize.y) };
	uint32 iAreaX = (uint32) floor((vecPosition.x + fabs(vecMapRangeMin.x)) / vecAreaSize.x);
	uint32 iAreaY = (uint32)floor((vecPosition.y + fabs(vecMapRangeMin.y)) / vecAreaSize.y);
	uint32 iAreaId = (iAreaY * uiAreaCount[1]) + iAreaX;
	//iAreaId = ((uiAreaCount[0] * uiAreaCount[1]) - 1) - iAreaId; // not sure if this is needed
	return iAreaId;
}

uint32				CDATPathManager::getTileCount(Vec2f& vecMapRangeMin, Vec2f& vecMapRangeMax, Vec2f& vecAreaSize)
{
	float32 fMapWidth = fabs(vecMapRangeMin.x) + fabs(vecMapRangeMax.x);
	float32 fMapHeight = fabs(vecMapRangeMin.y) + fabs(vecMapRangeMax.y);
	uint32 uiAreaCount[2] = { (uint32) ceil(fMapWidth / vecAreaSize.x), (uint32) ceil(fMapHeight / vecAreaSize.y) };
	return uiAreaCount[0] * uiAreaCount[1];
}

bool						CDATPathManager::findPathNode(vector<CDATPathFormat*>& veCDATFormatPaths, CDATEntry_Paths_General_PathNode& pathNodeToFind, uint32& uiAreaId_Out, uint32& uiNodeId_Out)
{
	for (auto pDATFile : veCDATFormatPaths)
	{
		uint32 uiNodeIndex = 0;
		for (auto& pathNode : pDATFile->m_vecPathNodes)
		{
			if (pathNode.m_vecPosition.x == pathNodeToFind.m_vecPosition.x
				&& pathNode.m_vecPosition.y == pathNodeToFind.m_vecPosition.y
				&& pathNode.m_vecPosition.z == pathNodeToFind.m_vecPosition.z
				&& pathNode.m_ucNodeType == pathNodeToFind.m_ucNodeType
				&& pathNode.m_ucPathWidth == pathNodeToFind.m_ucPathWidth)
			{
				uiAreaId_Out = pDATFile->m_uiFileIndex;
				uiNodeId_Out = uiNodeIndex;
				return true;
			}
			uiNodeIndex++;
		}
	}
	return false;
}

bool						CDATPathManager::findNaviNode(vector<CDATPathFormat*>& veCDATFormatPaths, CDATEntry_Paths_General_NaviNode& naviNodeToFind, uint32& uiAreaId_Out, uint32& uiNodeId_Out)
{
	for (auto pDATFile : veCDATFormatPaths)
	{
		uint32 uiNodeIndex = 0;
		for (auto& naviNode : pDATFile->m_vecNaviNodes)
		{
			if (naviNode.m_iPosition[0] == naviNodeToFind.m_iPosition[0]
				&& naviNode.m_iPosition[1] == naviNodeToFind.m_iPosition[1]
				&& naviNode.m_ucDirection[0] == naviNodeToFind.m_ucDirection[0]
				&& naviNode.m_ucDirection[1] == naviNodeToFind.m_ucDirection[1]
				&& naviNode.m_ucPathNodeWidth == naviNodeToFind.m_ucPathNodeWidth)
			{
				uiAreaId_Out = pDATFile->m_uiFileIndex;
				uiNodeId_Out = uiNodeIndex;
				return true;
			}
			uiNodeIndex++;
		}
	}
	return false;
}

CDATEntry_Paths_General_PathNode		CDATPathManager::getLowestPathNode(vector<CDATEntry_Paths_General_PathNode>& vecPathNodes)
{
	uint32
		uiLowestAreaId = -1,
		uiLowestNodeId = -1;
	CDATEntry_Paths_General_PathNode
		lowestPathNode;
	for (auto pathNode : vecPathNodes)
	{
		if (pathNode.m_wAreaId < (int32)uiLowestAreaId)
		{
			uiLowestAreaId = pathNode.m_wAreaId;
		}
	}
	for (auto pathNode : vecPathNodes)
	{
		if (pathNode.m_wAreaId == ((int32)uiLowestAreaId) && pathNode.m_wNodeId < (int32)uiLowestNodeId)
		{
			uiLowestNodeId = pathNode.m_wNodeId;
			lowestPathNode = pathNode;
		}
	}
	return lowestPathNode;
}

vector<CDATEntry_Paths_General_PathNode>	CDATPathManager::getTargetPathNodesForNaviNode(vector<CDATPathFormat*> veCDATFormats, CDATEntry_Paths_General_NaviNode& naviNode, uint32 uiNaviNodeAreaId, uint32 uiNaviNodeNodeId, uint32& uiLinkId_Out)
{
	vector<CDATEntry_Paths_General_PathNode> pathNodes;

	CDATEntry_Paths_General_PathNode pathNode1 = veCDATFormats[naviNode.m_usTargetNode_AreaId]->m_vecPathNodes[naviNode.m_usTargetNode_NodeId];
	//pathNodes.push_back(pathNode1);

	CDATEntry_Paths_General_PathNode pathNode2;
	for (auto pDATFile : veCDATFormats)
	{
		for (auto& pathNode : pDATFile->m_vecPathNodes)
		{
			for (uint32 i = 0, j = (pathNode.m_uiFlags & 0xF); i < j; i++)
			{
				auto link = pDATFile->m_vecLinks[pathNode.m_wConnectedNodesStartId + i];
				if (link.m_usNaviAreaId == uiNaviNodeAreaId && link.m_usNaviNodeId == uiNaviNodeNodeId)
				{
					pathNode2 = pathNode;

					if (pathNode1.m_vecPosition.x == pathNode2.m_vecPosition.x
						&& pathNode1.m_vecPosition.y == pathNode2.m_vecPosition.y
						&& pathNode1.m_vecPosition.z == pathNode2.m_vecPosition.z
						&& pathNode1.m_ucPathWidth == pathNode2.m_ucPathWidth
						)
					{
						// same node, do nothing
					}
					else
					{
						uiLinkId_Out = pathNode.m_wConnectedNodesStartId + i;
						pathNodes.push_back(pathNode2);
					}
				}
			}
		}
	}

	return pathNodes;
}