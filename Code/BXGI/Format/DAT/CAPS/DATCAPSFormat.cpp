#include "DATCAPSFormat.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

DATCAPSFormat::DATCAPSFormat(void) :
	Format(true, LITTLE_ENDIAN),
	m_uiOptimalRasterFormatForTextures_RGBA(0),
	m_uiOptimalRasterFormatForTextures_RGB(0),
	m_uiOptimalRasterFormatForTextures_Greyscale(0),
	m_uiOptimalRasterFormatForTextures_Palette256(0)
{
}

void					DATCAPSFormat::unserialize(void)
{
	DataReader *pDataReader = &m_reader;
	
	m_uiOptimalRasterFormatForTextures_RGBA			= pDataReader->readUint32();
	m_uiOptimalRasterFormatForTextures_RGB			= pDataReader->readUint32();
	m_uiOptimalRasterFormatForTextures_Greyscale	= pDataReader->readUint32();
	m_uiOptimalRasterFormatForTextures_Palette256	= pDataReader->readUint32();
}

void					DATCAPSFormat::serialize(void)
{
	DataWriter *pDataWriter = &m_writer;
	
	pDataWriter->write(m_uiOptimalRasterFormatForTextures_RGBA);
	pDataWriter->write(m_uiOptimalRasterFormatForTextures_RGB);
	pDataWriter->write(m_uiOptimalRasterFormatForTextures_Greyscale);
	pDataWriter->write(m_uiOptimalRasterFormatForTextures_Palette256);
}