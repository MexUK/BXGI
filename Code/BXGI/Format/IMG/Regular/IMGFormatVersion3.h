#pragma once

#include "nsbxgi.h"
#include "Format/IMG/Regular/IMGFormat.h"

class bxgi::IMGFormatVersion3 : public IMGFormat
{
public:
	IMGFormatVersion3(void);
	IMGFormatVersion3(std::string strFilePathOrData, bool bParam1IsFilePath);

	void					_unserialize(void);
	void					_serialize(void);

	void					setUndecryptedPositionOffset(int32 iUndecryptedPositionOffset) { m_iUndecryptedPositionOffset = iUndecryptedPositionOffset; }
	int32					getUndecryptedPositionOffset(void) { return m_iUndecryptedPositionOffset; }

private:
	int32					m_iUndecryptedPositionOffset;
};