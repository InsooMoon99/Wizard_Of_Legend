#pragma once
class Shader
{
    ID3D11VertexShader*			vertexShader = nullptr;
    ID3D11PixelShader*			pixelShader = nullptr;
    ID3D11InputLayout*			vertexLayout = nullptr;

public:
    Shader(wstring file);
    ~Shader();
    void Set();
};

