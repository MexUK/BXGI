#include "IDEEntry_PATH_Node.h"
#include "Exception/EExceptionCode.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "IDEEntry_PATH_Group.h"
#include "Format/IDE/IDEFormat.h"

using namespace bxcf;
using namespace bxgi;

IDEEntry_PATH_Node::IDEEntry_PATH_Node(IDEFormat *pIDEFormat) :
	IDEEntry_PATH(pIDEFormat, IDE_PATH_NODE),
	m_uiNodeType(0),
	m_uiNextNode(0),
	m_uiIsCrossRoad(0),
	m_vecPosition{ 0.0f, 0.0f, 0.0f },
	m_fUnknown1(0.0f),
	m_uiLeftLaneCount(0),
	m_uiRightLaneCount(0)
{
}

void			IDEEntry_PATH_Node::unserialize(void)
{
	DataReader *pDataReader = &m_pFormat->m_reader;

	if (getLatestPathGroup() == nullptr)
	{
		// path node found before path group
		throw EXCEPTION_INVALID_DATA_ORDER;
	}

	switch (pDataReader->getLineTokens().size())
	{
	case 9:
		IDEEntry_PATH::unserialize();

		setFormatType(0);
		setFormatGames(GAME_FLAG_GTA_III | GAME_FLAG_GTA_VC);

		m_uiNodeType = pDataReader->readTokenUint32();
		m_uiNextNode = pDataReader->readTokenUint32();
		m_uiIsCrossRoad = pDataReader->readTokenUint32();
		m_vecPosition = pDataReader->readVector3D() / 16.0f;
		m_fUnknown1 = pDataReader->readTokenFloat32();
		m_uiLeftLaneCount = pDataReader->readTokenUint32();
		m_uiRightLaneCount = pDataReader->readTokenUint32();
		break;
	}

	getLatestPathGroup()->addEntry(this);
}

void			IDEEntry_PATH_Node::serialize(void)
{
	DataWriter *pDataWriter = &m_pFormat->m_writer;

	switch (getFormatType())
	{
	case 0:
		IDEEntry_PATH::serialize();

		pDataWriter->writeToken(m_uiNodeType);
		pDataWriter->writeToken(m_uiNextNode);
		pDataWriter->writeToken(m_uiIsCrossRoad);
		pDataWriter->writeToken(m_vecPosition * 16.0f);
		pDataWriter->writeToken(m_fUnknown1);
		pDataWriter->writeToken(m_uiLeftLaneCount);
		pDataWriter->writeToken(m_uiRightLaneCount);
		break;
	}
}