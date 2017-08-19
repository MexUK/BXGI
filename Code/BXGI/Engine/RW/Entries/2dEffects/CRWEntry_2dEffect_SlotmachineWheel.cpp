#include "CRWEntry_2dEffect_SlotmachineWheel.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace bxcf;
using namespace bxgi;

CRWEntry_2dEffect_SlotmachineWheel::CRWEntry_2dEffect_SlotmachineWheel(void) :
	C2dEffect(_2DFX_SLOT_MACHINE_WHEEL),
	m_iWheelIndex(0)
{
};

void							CRWEntry_2dEffect_SlotmachineWheel::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();
	
	m_iWheelIndex = pDataReader->readInt32();
}

void							CRWEntry_2dEffect_SlotmachineWheel::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();
	
	pDataWriter->writeInt32(m_iWheelIndex);
}