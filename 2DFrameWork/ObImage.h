#pragma once
enum class ANIMSTATE
{
    STOP, //정지
    LOOP, //반복재생
    ONCE, //한번만 재생
    REVERSE_LOOP, //역으로 반복재생
    REVERSE_ONCE
};

class ObImage : public GameObject
{
    friend class ObTileMap;

private:
    static ID3D11Buffer*        vertexBuffer;
    static ID3D11Buffer*        uvBuffer;

public:
    static void CreateStaticMember();
    static void DeleteStaticMember();

public:
    ANIMSTATE   Get_AnimState()     { return animState; }
    float       Get_AnimTime()      { return animTime; }
    float       Get_AnimInterval()  { return animInterval; }

private:
    ID3D11ShaderResourceView*   SRV;
    ID3D11SamplerState*         sampler;
    D3D11_SAMPLER_DESC	        samplerDesc;

    ANIMSTATE                   animState;
    float                       animTime;     //재생시킬 때 필요한 변수
    float                       animInterval; //재생간격
    bool                        animXAxis;    //가로재생?

public:
    Vector4                     uv;
    Int2                        maxFrame;
    Int2                        frame;
    bool                        reverseLR;
    wstring                     file;

private:
    void PlayAnim();

public:
    ObImage(wstring file);
    ObImage(const ObImage& other);              // 복사 생성자
    ObImage& operator=(const ObImage& other);   // 복사 대입 연산자

    ~ObImage();

    void Update() override;
    void Render() override;
    void ChangeAnim(ANIMSTATE anim, float interval, bool xAxis = true);
    void ChangeSampler(D3D11_FILTER filter = D3D11_FILTER_MIN_MAG_MIP_POINT,
        D3D11_TEXTURE_ADDRESS_MODE addressU = D3D11_TEXTURE_ADDRESS_WRAP,
        D3D11_TEXTURE_ADDRESS_MODE addressV = D3D11_TEXTURE_ADDRESS_WRAP);
};

