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
    void RequestClear(); 

    shared_ptr<CActor> SpawnActor(std::shared_ptr<CActor> actor);

    template<typename T, typename... Args>
    shared_ptr<T> CreateAndSpawn(Args&&... args)
    {
        auto obj = make_shared<T>(std::forward<Args>(args)...);
    	SpawnActor(obj);

    	return obj;
    }

    const vector<shared_ptr<CActor>>& GetActives() const { return m_ActiveObjects; }

    shared_ptr<CActor> GetPlayer() { return m_Player; }
    void SetPlayer(shared_ptr<CActor> player) { m_Player = player; };

private:
    CObjectManager() = default;

    void ForceClear();
    void FlushPendingAdds(); 

private:
    bool m_IsIterating = false;   
    bool m_PendingClear = false;  

    vector<shared_ptr<CActor>> m_PendingAdd;
    vector<shared_ptr<CActor>> m_ActiveObjects;

    shared_ptr<CActor> m_Player;
};


