#ifndef CDFFFormat_H
#define CDFFFormat_H

#include "bxgi.h"
#include "Engine/RW/CRWFormat.h"
#include "Engine/RW/CRWSection.h"
#include "Intermediate/Model/CIntermediateModelFormat.h"
#include <string>
#include <vector>
#include <unordered_map>

class bxgi::CRWVersion;
class bxgi::CTextureEntry;
class bxgi::C2dEffect;

class bxgi::CDFFFormat : public bxgi::CRWFormat
{
public:
	bxgi::CIntermediateModelFormat*									convertToIntermediateModelFormat(void);
};

#endif