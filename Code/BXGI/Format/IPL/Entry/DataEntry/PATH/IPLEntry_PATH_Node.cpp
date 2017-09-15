#include "IPLEntry_PATH_Node.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "IPLEntry_PATH_Group.h"
#include "Format/IPL/IPLFormat.h"

using namespace bxcf;
using namespace bxgi;

IPLEntry_PATH_Node::IPLEntry_PATH_Node(IPLFormat *pIPLFormat) :
	IPLEntry_PATH(pIPLFormat, IPL_PATH_NODE),
	m_uiNodeType(0),
	m_uiNextNode(0),
	m_uiIsCrossRoad(0),
	m_vecPosition{ 0.0f, 0.0f, 0.0f },
	m_fMedian(0.0f),
	m_uiLeftLaneCount(0),
	m_uiRightLaneCount(0),
	m_uiSpeedLimit(0),
	m_uiFlags(0),
	m_fSpawnRate(0.0f)
{
}

void			IPLEntry_PATH_Node::unserialize(void)
{
	DataReader *pDataReader = &m_pFormat->m_reader;

	if (getLatestPathGroup() == nullptr)
	{
		// path node found before path group
		throw EXCEPTION_INVALID_DATA_ORDER;
	}

	switch (pDataReader->getLineTokens().size())
	{
	case 12:
		IPLEntry_PATH::unserialize();

		setFormatType(0);
		setFormatGames(GAME_FLAG_GTA_VC);

		m_uiNodeType = pDataReader->readTokenUint32();
		m_uiNextNode = pDataReader->readTokenUint32();
		m_uiIsCrossRoad = pDataReader->readTokenUint32();
		m_vecPosition = pDataReader->readTokenVector3D() / 16.0f;
		m_fMedian = pDataReader->readTokenFloat32();
		m_uiLeftLaneCount = pDataReader->readTokenUint32();
		m_uiRightLaneCount = pDataReader->readTokenUint32();
		m_uiSpeedLimit = pDataReader->readTokenUint32();
		m_uiFlags = pDataReader->readTokenUint32();
		m_fSpawnRate = pDataReader->readTokenFloat32();
		break;
	}

	getLatestPathGroup()->addEntry(this);
}

void			IPLEntry_PATH_Node::serialize(void)
{
	DataWriter *pDataWriter = &m_pFormat->m_writer;

	switch (getFormatType())
	{
	case 0:
		IPLEntry_PATH::serialize();

		pDataWriter->writeUint32(m_uiNodeType);
		pDataWriter->writeUint32(m_uiNextNode);
		pDataWriter->writeUint32(m_uiIsCrossRoad);
		pDataWriter->writeVector3D(m_vecPosition * 16.0f);
		pDataWriter->writeFloat32(m_fMedian);
		pDataWriter->writeUint32(m_uiLeftLaneCount);
		pDataWriter->writeUint32(m_uiRightLaneCount);
		pDataWriter->writeUint32(m_uiSpeedLimit);
		pDataWriter->writeUint32(m_uiFlags);
		pDataWriter->writeFloat32(m_fSpawnRate);
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT_TYPE;
	}
}