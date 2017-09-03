#include "CRWEntry_2dEffect_SlotmachineWheel.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

CRWEntry_2dEffect_SlotmachineWheel::CRWEntry_2dEffect_SlotmachineWheel(void) :
	C2dEffect(_2DFX_SLOT_MACHINE_WHEEL),
	m_iWheelIndex(0)
{
};

void							CRWEntry_2dEffect_SlotmachineWheel::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();
	
	m_iWheelIndex = pDataReader->readInt32();
}

void							CRWEntry_2dEffect_SlotmachineWheel::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();
	
	pDataWriter->writeInt32(m_iWheelIndex);
}