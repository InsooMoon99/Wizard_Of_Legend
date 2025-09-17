#pragma once

#include "CSCeneManager.h"
#include "CResourceManager.h"
#include "CCollisionManager.h"
#include "CParticleManager.h"

#define WINCX 1280
#define WINCY 720

#define SCENE		CSceneManager::Get_Instance()
#define RESOURCE	CResourceManager::Get_Instance()
#define COLLISION	CCollisionManager::Get_Instance()
#define PARTICLE	CParticleManager::Get_Instance()