#pragma once

#include "nsbxgi.h"
#include "EDATLoaderEntryType.h"
#include <string>
#include <deque>

class bxgi::DATLoaderEntry
{
public:
	DATLoaderEntry(DATLoaderFormat *pFormat);

	void						unload(void) {}

	void						serialize(void);

	std::string					getEntryTypeText(void);

	void						setFormat(DATLoaderFormat *pFormat) { m_pFormat = pFormat; }
	DATLoaderFormat*			getFormat(void) { return m_pFormat; }

	void						setEntryType(EDATLoaderEntryType eEntryType) { m_uiEntryType = eEntryType; }
	EDATLoaderEntryType			getEntryType(void) { return m_uiEntryType; }

	void						setEntryValues(std::deque<std::string>& deqValues) { m_deqValues = deqValues; }
	std::deque<std::string>&	getEntryValues(void) { return m_deqValues; }

private:
	DATLoaderFormat*			m_pFormat;
	EDATLoaderEntryType			m_uiEntryType;
	std::deque<std::string>		m_deqValues;
};