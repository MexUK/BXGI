#pragma once

#include "bxgi.h"
#include "Type/Types.h"
#include "eIDESection.h"
#include "Helper/SectionLines/CSectionLinesEntry.h"

class bxgi::CIDEFormat;

class bxgi::CIDEEntry : public bxgi::CSectionLinesEntry<CIDEFormat, eIDESection>
{
public:
	CIDEEntry(CIDEFormat *pIDEFormat);
};