#ifndef H_XEFFECTS_CB
#define H_XEFFECTS_CB

#include "effectWrapper.h"

class DepthShaderCB : public irr::video::IShaderConstantSetCallBack
{
public:
	DepthShaderCB(effectHandler* effectIn) : effect(effectIn), PassTimer(false) {};
	
	virtual void OnSetMaterial(const irr::video::SMaterial& material)
	{
		PassTimer = ((irr::s32)material.MaterialType) == effect->getEffectMaterialType(EET_MRWIGGLE);
	}

	virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData)
	{
		irr::video::IVideoDriver* driver = services->getVideoDriver();

		worldViewProj = driver->getTransform(irr::video::ETS_PROJECTION);			
		worldViewProj *= driver->getTransform(irr::video::ETS_VIEW);
		worldViewProj *= driver->getTransform(irr::video::ETS_WORLD);

		services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);
		
		services->setVertexShaderConstant("MaxD", &FarLink, 1);

		if(PassTimer)
		{
			irr::f32 currtime = (irr::f32)effect->getTime();
			services->setVertexShaderConstant("Timer", reinterpret_cast<irr::f32*>(&currtime), 1);
		}
	}

	effectHandler* effect;
	bool PassTimer;
	irr::f32 FarLink;
	irr::core::matrix4 worldViewProj;
};

class ShadowShaderCB : public irr::video::IShaderConstantSetCallBack
{
public:
	ShadowShaderCB(effectHandler* effectIn) : effect(effectIn) {};

	virtual void OnSetMaterial(const irr::video::SMaterial& material) {}

	virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData)
	{
		irr::video::IVideoDriver* driver = services->getVideoDriver();

		irr::core::matrix4 worldViewProj = driver->getTransform(irr::video::ETS_PROJECTION);			
		worldViewProj *= driver->getTransform(irr::video::ETS_VIEW);
		worldViewProj *= driver->getTransform(irr::video::ETS_WORLD);
		services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);

		worldViewProj = ProjLink;			
		worldViewProj *= ViewLink;
		worldViewProj *= driver->getTransform(irr::video::ETS_WORLD);
		services->setVertexShaderConstant("mWorldViewProj2", worldViewProj.pointer(), 16);

		driver->getTransform(irr::video::ETS_WORLD).getInverse(invWorld);
		irr::core::vector3df lightPosOS = LightLink;
		invWorld.transformVect(lightPosOS); 
		services->setVertexShaderConstant("LightPos", reinterpret_cast<irr::f32*>(&lightPosOS.X), 4);
		
		services->setVertexShaderConstant("MaxD", reinterpret_cast<irr::f32*>(&FarLink), 1);

		services->setPixelShaderConstant("LightColour", reinterpret_cast<irr::f32*>(&LightColour.r), 4);
		
	}

	effectHandler* effect;
	irr::core::matrix4 invWorld;

	irr::video::SColorf LightColour;
	irr::core::matrix4 ProjLink;
	irr::core::matrix4 ViewLink;
	irr::core::vector3df LightLink;
	irr::f32 FarLink;
};

class LightShaderCB : public irr::video::IShaderConstantSetCallBack
{
public:
	LightShaderCB(effectHandler* effectIn) : effect(effectIn) {};

	effectHandler* effect;
	irr::core::matrix4 worldViewProj;
	irr::core::matrix4 invWorld;

	virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData)
	{
		irr::video::IVideoDriver* driver = services->getVideoDriver();

		worldViewProj = driver->getTransform(irr::video::ETS_PROJECTION);			
		worldViewProj *= driver->getTransform(irr::video::ETS_VIEW);
		worldViewProj *= driver->getTransform(irr::video::ETS_WORLD);
		services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);
	
		driver->getTransform(irr::video::ETS_WORLD).getInverse(invWorld);

		irr::core::vector3df lightPosOS;
		
		if(driver->getDynamicLightCount() > 0)
		{
			lightPosOS = driver->getDynamicLight(0).Position;
			invWorld.transformVect(lightPosOS); 
		}

		services->setVertexShaderConstant("LightPos", reinterpret_cast<irr::f32*>(&lightPosOS), 4);
	}
};

class WiggleShaderCB : public irr::video::IShaderConstantSetCallBack
{
public:
	WiggleShaderCB(effectHandler* effectIn) : effect(effectIn) {};

	effectHandler* effect;
	irr::core::matrix4 worldViewProj;
	irr::core::matrix4 invWorld;

	virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData)
	{
		irr::video::IVideoDriver* driver = services->getVideoDriver();

		worldViewProj = driver->getTransform(irr::video::ETS_PROJECTION);			
		worldViewProj *= driver->getTransform(irr::video::ETS_VIEW);
		worldViewProj *= driver->getTransform(irr::video::ETS_WORLD);
		services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);

		worldViewProj = driver->getTransform(irr::video::ETS_WORLD);
		services->setVertexShaderConstant("World", worldViewProj.pointer(), 16);

		worldViewProj = driver->getTransform(irr::video::ETS_WORLD);
		worldViewProj.getInverse(worldViewProj);
		worldViewProj.getTransposed(worldViewProj);
		services->setVertexShaderConstant("mInvWorld", worldViewProj.pointer(), 16);

		worldViewProj = driver->getTransform(irr::video::ETS_VIEW);
		worldViewProj.getInverse(worldViewProj);
		services->setVertexShaderConstant("ViewI", worldViewProj.pointer(), 16);

		driver->getTransform(irr::video::ETS_WORLD).getInverse(invWorld);

		irr::core::vector3df lightPosOS;
		
		if(driver->getDynamicLightCount() > 0)
		{
			lightPosOS = driver->getDynamicLight(0).Position;
			invWorld.transformVect(lightPosOS); 
		}

		services->setVertexShaderConstant("LightPos", reinterpret_cast<irr::f32*>(&lightPosOS), 4);

		irr::f32 currtime = effect->getTime();
		services->setVertexShaderConstant("Timer", reinterpret_cast<irr::f32*>(&currtime), 1);
	}
};

class AnisoShaderCB : public irr::video::IShaderConstantSetCallBack
{
public:
	AnisoShaderCB(effectHandler* effectIn) : effect(effectIn) {};

	effectHandler* effect;
	irr::core::matrix4 worldViewProj;
	irr::core::matrix4 invWorld;

	virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData)
	{
		irr::video::IVideoDriver* driver = services->getVideoDriver();

		worldViewProj = driver->getTransform(irr::video::ETS_PROJECTION);			
		worldViewProj *= driver->getTransform(irr::video::ETS_VIEW);
		worldViewProj *= driver->getTransform(irr::video::ETS_WORLD);
		services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);

		worldViewProj = driver->getTransform(irr::video::ETS_WORLD);
		services->setVertexShaderConstant("World", worldViewProj.pointer(), 16);

		worldViewProj = driver->getTransform(irr::video::ETS_VIEW);
		worldViewProj.getInverse(worldViewProj);
		services->setVertexShaderConstant("ViewI", worldViewProj.pointer(), 16);

		driver->getTransform(irr::video::ETS_WORLD).getInverse(invWorld);

		irr::core::vector3df lightPosOS;
		
		if(driver->getDynamicLightCount() > 0)
		{
			lightPosOS = driver->getDynamicLight(0).Position;
			invWorld.transformVect(lightPosOS); 
		}

		services->setVertexShaderConstant("LightPos", reinterpret_cast<irr::f32*>(&lightPosOS), 4);
	}
};

class PhongShaderCB : public irr::video::IShaderConstantSetCallBack
{
public:
	PhongShaderCB(effectHandler* effectIn) : effect(effectIn) {};

	effectHandler* effect;
	irr::core::matrix4 worldViewProj;
	irr::core::matrix4 invWorld;

	virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData)
	{
		irr::video::IVideoDriver* driver = services->getVideoDriver();

		worldViewProj = driver->getTransform(irr::video::ETS_PROJECTION);			
		worldViewProj *= driver->getTransform(irr::video::ETS_VIEW);
		worldViewProj *= driver->getTransform(irr::video::ETS_WORLD);
		services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);

		worldViewProj = driver->getTransform(irr::video::ETS_WORLD);
		services->setVertexShaderConstant("World", worldViewProj.pointer(), 16);

		driver->getTransform(irr::video::ETS_WORLD).getInverse(invWorld);

		irr::core::vector3df lightPosOS;
		
		if(driver->getDynamicLightCount() > 0)
		{
			lightPosOS = driver->getDynamicLight(0).Position;
			invWorld.transformVect(lightPosOS); 
		}

		services->setVertexShaderConstant("LightPos", reinterpret_cast<irr::f32*>(&lightPosOS.X), 4);

		irr::core::vector3df eyePos;
		
		if(effect->getActiveSceneManager()->getActiveCamera())
		{
			eyePos = effect->getActiveSceneManager()->getActiveCamera()->getPosition();
		}

		services->setVertexShaderConstant("CamPos", reinterpret_cast<irr::f32*>(&eyePos.X), 4);
	}
};

class BRDFShaderCB : public irr::video::IShaderConstantSetCallBack
{
public:
	BRDFShaderCB(effectHandler* effectIn) : effect(effectIn) {};

	effectHandler* effect;
	irr::core::matrix4 worldViewProj;
	irr::core::matrix4 invWorld;

	virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData)
	{
		irr::video::IVideoDriver* driver = services->getVideoDriver();

		worldViewProj = driver->getTransform(irr::video::ETS_PROJECTION);			
		worldViewProj *= driver->getTransform(irr::video::ETS_VIEW);
		worldViewProj *= driver->getTransform(irr::video::ETS_WORLD);
		services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);

		worldViewProj = driver->getTransform(irr::video::ETS_WORLD);
		services->setVertexShaderConstant("World", worldViewProj.pointer(), 16);

		driver->getTransform(irr::video::ETS_WORLD).getInverse(invWorld);

		irr::core::vector3df lightPosOS;
		
		if(driver->getDynamicLightCount() > 0)
		{
			lightPosOS = driver->getDynamicLight(0).Position;
			invWorld.transformVect(lightPosOS);
		}

		services->setVertexShaderConstant("LightPos", reinterpret_cast<irr::f32*>(&lightPosOS.X), 4);
		
		irr::core::vector3df eyePos;
		
		if(effect->getActiveSceneManager()->getActiveCamera())
		{
			eyePos = effect->getActiveSceneManager()->getActiveCamera()->getPosition();
		}

		services->setVertexShaderConstant("CamPos", reinterpret_cast<irr::f32*>(&eyePos.X), 4);

		
		if(services->getVideoDriver()->getDriverType() == irr::video::EDT_OPENGL)
		{
			int texVar = 0;
			services->setPixelShaderConstant("hmapSampler", (float*)(&texVar), 1); 

			texVar = 1;
			services->setPixelShaderConstant("nmapSampler", (float*)(&texVar), 1); 
		}
	}
};

#endif