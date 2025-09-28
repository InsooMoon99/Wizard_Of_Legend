#pragma once

#include "CSCeneManager.h"
#include "CResourceManager.h"
#include "CCollisionManager.h"
#include "CParticleManager.h"
#include "CTileManager.h"
#include "CRenderManager.h"
#include "CSkillManager.h"
#include "CSkillDataManager.h"

#define WINCX 1280
#define WINCY 720

#define TILECX 48
#define TILECY 48

#define TILEX 30
#define TILEY 20

#define EARTH_SHIELD 8
#define TARGET_ORB   5
#define STAR_CIRCLE  5
#define SKILL_PANEL  6

#define DEBUGLINE cout << "\n1";

#define SCENE		CSceneManager::Get_Instance()
#define RESOURCE	CResourceManager::Get_Instance()
#define COLLISION	CCollisionManager::Get_Instance()
#define PARTICLE	CParticleManager::Get_Instance()
#define TILE		CTileManager::Get_Instance()
#define RENDER		CRenderManager::Get_Instance()
#define SKILL		CSkillManager::Get_Instance()
#define SKILL_DATA		CSkillDataManager::Get_Instance()