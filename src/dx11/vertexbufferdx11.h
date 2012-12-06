#pragma once
class DX11;
struct ID3D11Buffer;

class VertexBufferDX11
{
public:
	VertexBufferDX11(int vertexSize, int numberOfVertices, void* data, DX11* dx);
	~VertexBufferDX11(void);
	void Draw();

private:
	ID3D11Buffer* pVBuffer; 
	DX11* mDX;
	int mVertexSize;
	int mNumberOfVertices;
};

