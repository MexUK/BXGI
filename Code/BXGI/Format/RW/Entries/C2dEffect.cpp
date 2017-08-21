#include "C2dEffect.h"
#include "Format/RW/Entries/2dEffects/CRWEntry_2dEffect_Light.h"
#include "Format/RW/Entries/2dEffects/CRWEntry_2dEffect_ParticleEffect.h"
#include "Format/RW/Entries/2dEffects/CRWEntry_2dEffect_PedAttractor.h"
#include "Format/RW/Entries/2dEffects/CRWEntry_2dEffect_SunGlare.h"
#include "Format/RW/Entries/2dEffects/CRWEntry_2dEffect_EnterExit.h"
#include "Format/RW/Entries/2dEffects/CRWEntry_2dEffect_StreetSign.h"
#include "Format/RW/Entries/2dEffects/CRWEntry_2dEffect_SlotmachineWheel.h"
#include "Format/RW/Entries/2dEffects/CRWEntry_2dEffect_CoverPoint.h"
#include "Format/RW/Entries/2dEffects/CRWEntry_2dEffect_Escalator.h"

using namespace bxcf;
using namespace bxgi;

C2dEffect*			C2dEffect::createRW2dEffect(e2DFXType e2DFXTypeValue)
{
	switch (e2DFXTypeValue)
	{
	case _2DFX_LIGHT:				return new CRWEntry_2dEffect_Light;
	case _2DFX_PARTICLE:			return new CRWEntry_2dEffect_ParticleEffect;
	case _2DFX_PED_ATTRACTOR:		return new CRWEntry_2dEffect_PedAttractor;
	case _2DFX_SUN_GLARE:			return new CRWEntry_2dEffect_SunGlare;
	case _2DFX_ENTER_EXIT:			return new CRWEntry_2dEffect_EnterExit;
	case _2DFX_STREET_SIGN:			return new CRWEntry_2dEffect_StreetSign;
	case _2DFX_SLOT_MACHINE_WHEEL:	return new CRWEntry_2dEffect_SlotmachineWheel;
	case _2DFX_COVER_POINT:			return new CRWEntry_2dEffect_CoverPoint;
	case _2DFX_ESCALATOR:			return new CRWEntry_2dEffect_Escalator;
	}
	return nullptr;
}