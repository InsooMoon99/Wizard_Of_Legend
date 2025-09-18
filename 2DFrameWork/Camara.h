#pragma once
class Camera : public Singleton<Camera>
{
private:
    Viewport            viewport;       //뷰포트
    Matrix              V, P, VP;       //View x Proj

public:
    Vector2             GetCameraPos() { return position; }

public:
    //카메라위치
    Vector2             position;


public:
    Camera();
    void          Set();
    void          ResizeScreen();
    const Matrix& GetP() { return P; };
    const Matrix& GetVP() { return VP; };
};