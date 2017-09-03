#include "RWEntry_2dEffect_SlotmachineWheel.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace bxcf;
using namespace bxgi;

RWEntry_2dEffect_SlotmachineWheel::RWEntry_2dEffect_SlotmachineWheel(void) :
	_2dEffect(_2DFX_SLOT_MACHINE_WHEEL),
	m_iWheelIndex(0)
{
};

void							RWEntry_2dEffect_SlotmachineWheel::unserialize(void)
{
	DataReader *pDataReader = DataReader::get();
	
	m_iWheelIndex = pDataReader->readInt32();
}

void							RWEntry_2dEffect_SlotmachineWheel::serialize(void)
{
	DataWriter *pDataWriter = DataWriter::get();
	
	pDataWriter->writeInt32(m_iWheelIndex);
}