#pragma once

#include "bxgi.h"
#include "Type/Types.h"
#include "Format/IDE/Entry/CIDEEntry_Data.h"
#include "Format/e2DFXType.h"
#include "Type/Vector/Vec3u.h"
#include "Type/Vector/Vec3f.h"
#include "Type/Vector/Vec4f.h"
#include <string>

class bxgi::CIDEEntry_2DFX : public bxgi::CIDEEntry_Data
{
public:
	CIDEEntry_2DFX(bxgi::CIDEFormat *pIDEFormat, bxgi::e2DFXType e2DFXTypeValue) :
		CIDEEntry_Data(pIDEFormat, IDE_SECTION_2DFX),
		m_e2DFXType(e2DFXTypeValue),
		m_uiObjectId(0),
		m_vecPosition{ 0.0f, 0.0f, 0.0f },
		m_uiUnknown1(0),
		m_ui2dfxType(0),
		m_vecRotation{ 0.0f, 0.0f, 0.0f, 0.0f }
	{
		m_vecColour.x = 0;
		m_vecColour.y = 0;
		m_vecColour.z = 0;
	}
	~CIDEEntry_2DFX(void)
	{
	}

	virtual void				unserialize(void);
	virtual void				serialize(void);

	void						set2DFXType(bxgi::e2DFXType e2DFXTypeValue) { m_e2DFXType = e2DFXTypeValue; }
	bxgi::e2DFXType					get2DFXType(void) { return m_e2DFXType; }

	void						setObjectId(uint32 uiObjectId) { m_uiObjectId = uiObjectId; }
	uint32						getObjectId(void) { return m_uiObjectId; }

	void						setPosition(bxcf::Vec3f& vecPosition) { m_vecPosition = vecPosition; }
	bxcf::Vec3f&					getPosition(void) { return m_vecPosition; }

	void						setColour(bxcf::Vec3u& vecColour) { m_vecColour = vecColour; }
	bxcf::Vec3u&				getColour(void) { return m_vecColour; }

	void						setUnknown1(uint32 uiUnknown1) { m_uiUnknown1 = uiUnknown1; }
	uint32						getUnknown1(void) { return m_uiUnknown1; }

	void						set2dfxType(uint32 ui2dfxType) { m_ui2dfxType = ui2dfxType; }
	uint32						get2dfxType(void) { return m_ui2dfxType; }

	void						setModelName(std::string& strModelName) { m_strModelName = strModelName; }
	std::string&				getModelName(void) { return m_strModelName; }

	void						setRotation(bxcf::Vec4f& vecRotation) { m_vecRotation = vecRotation; }
	bxcf::Vec4f&					getRotation(void) { return m_vecRotation; }

private:
	bxgi::e2DFXType					m_e2DFXType;
	bxcf::Vec3f					m_vecPosition; // GTA III era + GTA IV
	uint32						m_ui2dfxType; // GTA III era + GTA IV
	union
	{
		struct // GTA III era only
		{
			uint32						m_uiObjectId;
			bxcf::Vec3u				m_vecColour;
			uint32						m_uiUnknown1;
		};
		struct // GTA IV only
		{
			std::string					m_strModelName;
			bxcf::Vec4f					m_vecRotation;
		};
	};
};