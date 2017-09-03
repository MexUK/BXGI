#pragma once

#include "nsbxgi.h"
#include "Object/Singleton.h"
#include "Object/Manager.h"
#include "CRWVersion.h"
#include <string>

class bxgi::CRWVersionManager;

class bxgi::CRWManager : public bxcf::Manager, public bxcf::Singleton<bxgi::CRWManager>
{
public:
	CRWManager(void);
	~CRWManager(void);

	void											init(void);
	void											uninit(void);

	bxgi::CRWVersionManager*						getVersionManager(void) { return m_pVersionManager; }

	void											setSerializationRWVersion(uint32 uiSerializationRWVersion) { m_uiSerializationRWVersion = uiSerializationRWVersion; }
	uint32											getSerializationRWVersion(void) { return m_uiSerializationRWVersion; }

	static bool										isRWFileExtension(std::string& strFileExtension);
	static bool										isRWModelExtension(std::string& strFileExtension);
	static bool										isRWTextureExtension(std::string& strFileExtension);

private:
	bxgi::CRWVersionManager*						m_pVersionManager;
	uint32											m_uiSerializationRWVersion;
};