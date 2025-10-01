#pragma once
#include <memory>
#include <vector>

class CActor;

class CObjectManager
{
public:
    static CObjectManager* Instance;
    static CObjectManager* GetInstance()
    {
        if (!Instance) 
            Instance = new CObjectManager();

        return Instance;
    }

public:
    void Init();
    void Update();
    void Render();
    void Release();

    shared_ptr<CActor> SpawnActor(std::shared_ptr<CActor> actor);

    template<typename T, typename... Args>
    shared_ptr<T> CreateAndSpawn(Args&&... args)
    {
        auto obj = make_shared<T>(std::forward<Args>(args)...);
    	SpawnActor(obj);

    	return obj;
    }

    const vector<shared_ptr<CActor>>& GetActives() const { return m_ActiveObjects; }

private:
    CObjectManager() = default;

    vector<shared_ptr<CActor>> m_PendingAdd;
    vector<shared_ptr<CActor>> m_ActiveObjects;

    bool m_IsIterating = false;
};


