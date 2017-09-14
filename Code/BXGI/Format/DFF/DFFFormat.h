#pragma once

#include "nsbxgi.h"
#include "Format/RW/RWFormat.h"
#include "Intermediate/Model/IntermediateModelFormat.h"
#include <string>
#include <vector>

class bxgi::DFFFormat : public bxgi::RWFormat
{
public:
	DFFFormat(std::string& strFilePathOrData, bool bStringIsFilePath = true) : bxgi::RWFormat(strFilePathOrData, bStringIsFilePath) {}

	bxgi::IntermediateModelFormat*									convertToIntermediateModelFormat(void);
	std::vector<std::string>										getModelNames(void);
};