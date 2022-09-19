#pragma once
#include "Model3D.h"

// Forward reference
class Model3D;
struct Md2Header;

class MD2Loader
{
public:
	MD2Loader(void);
	~MD2Loader(void);
	static bool LoadModel(const char* md2Filename, Model3D& model, const char* textureFilename = 0);

private:
	static bool LoadPCX(const char* filename, BYTE* texture, SDL_Color* palette, const Md2Header* md2Header);
};
