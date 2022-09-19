#include "Texture.h"

// Constructors
Texture::Texture(void)
{
	_u = 0;
	_v = 0;
}

Texture::Texture(short u, short v)
{
	_u = u;
	_v = v;
}

Texture::Texture(const Texture& t)
{
	Copy(t);
}

// Destructor
Texture::~Texture(void) {}

// Properties
short Texture::GetTextureU() const
{
	return _u;
}

short Texture::GetTextureV() const
{
	return _v;
}

void Texture::SetTextureU(const short u)
{
	_u = u;
}

void Texture::SetTextureV(const short v)
{
	_v = v;
}

// Operator overloads
Texture& Texture::operator=(const Texture& rhs)
{
	if (this != &rhs)
	{
		Copy(rhs);
	}
	return *this;
}

// Private methods
void Texture::Copy(const Texture& t)
{
	_u = t.GetTextureU();
	_v = t.GetTextureV();
}
