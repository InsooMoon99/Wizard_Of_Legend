#pragma once
#include "../2DFrameWork/framework.h"
#pragma comment (lib,"../Build/2DFrameWork/2DFrameWork.lib")

extern int loadingCount;
extern mutex m;

// Json
#include "json.hpp"
#include <fstream>
using json = nlohmann::json;

using Vec2 = Vector2;

#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <format>

#include "CEnums.h"
#include "CDefine.h"
#include "CHelper.h"
#include "CStruct.h"

