#pragma once

#include "nsbxgi.h"
#include "Type/Vector/Vec2f.h"
#include "Type/Vector/Vec3f.h"

class bxgi::WDRBlock_VertexData
{
public:
	bxcf::Vec3f					m_vecPosition;
	bxcf::Vec3f					m_vecNormals;
	uint32						m_uiColour; // RGBA
	bxcf::Vec2f					m_vecTextureCoordinates;
	float32						m_fNormals2[4];
};