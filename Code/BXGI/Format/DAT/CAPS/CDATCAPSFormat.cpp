#include "CDATCAPSFormat.h"
#include "Stream/CDataReader.h"
#include "Stream/CDataWriter.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

CDATCAPSFormat::CDATCAPSFormat(void) :
	CFormat(true, LITTLE_ENDIAN),
	m_uiOptimalRasterFormatForTextures_RGBA(0),
	m_uiOptimalRasterFormatForTextures_RGB(0),
	m_uiOptimalRasterFormatForTextures_Greyscale(0),
	m_uiOptimalRasterFormatForTextures_Palette256(0)
{
}

void					CDATCAPSFormat::unserialize(void)
{
	CDataReader *pDataReader = CDataReader::get();
	
	m_uiOptimalRasterFormatForTextures_RGBA			= pDataReader->readUint32();
	m_uiOptimalRasterFormatForTextures_RGB			= pDataReader->readUint32();
	m_uiOptimalRasterFormatForTextures_Greyscale	= pDataReader->readUint32();
	m_uiOptimalRasterFormatForTextures_Palette256	= pDataReader->readUint32();
}

void					CDATCAPSFormat::serialize(void)
{
	CDataWriter *pDataWriter = CDataWriter::get();
	
	pDataWriter->write(m_uiOptimalRasterFormatForTextures_RGBA);
	pDataWriter->write(m_uiOptimalRasterFormatForTextures_RGB);
	pDataWriter->write(m_uiOptimalRasterFormatForTextures_Greyscale);
	pDataWriter->write(m_uiOptimalRasterFormatForTextures_Palette256);
}