#pragma once

#include "CSCeneManager.h"
#include "CResourceManager.h"
#include "CCollisionManager.h"
#include "CParticleManager.h"
#include "CTileManager.h"
#include "CRenderManager.h"

#define WINCX 1280
#define WINCY 720

#define TILECX 64
#define TILECY 64

#define TILEX 30
#define TILEY 20

#define EARTH_SHIELD 8

#define SCENE		CSceneManager::Get_Instance()
#define RESOURCE	CResourceManager::Get_Instance()
#define COLLISION	CCollisionManager::Get_Instance()
#define PARTICLE	CParticleManager::Get_Instance()
#define TILE		CTileManager::Get_Instance()
#define RENDER		CRenderManager::Get_Instance()