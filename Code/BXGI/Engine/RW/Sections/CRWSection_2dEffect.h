#ifndef CRWSection_2dEffect_H
#define CRWSection_2dEffect_H

#include "bxgi.h"
#include "Engine/RW/CRWSection.h"
#include "Pool/CVectorPool.h"
#include "Engine/RW/Pools/C2dEffectsPool.h"
#include "Type/Vector/Vec2f.h"
#include "Type/Vector/Vec3f.h"
#include <string>

class bxgi::CRWSection_2dEffect : public bxgi::CRWSection
{
public:
	CRWSection_2dEffect(void);
	~CRWSection_2dEffect(void);

	void							unserialize(void);
	void							serialize(void);

	bxgi::C2dEffectsPool*					get2dEffects(void) { return m_p2dEffectsPool; }

private:
	bxgi::C2dEffectsPool*					m_p2dEffectsPool;
};

#endif