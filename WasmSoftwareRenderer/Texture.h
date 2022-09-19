#pragma once

// Class to store texture information (a UV pair)
class Texture
{
public:
	Texture(void);
	Texture(short u, short v);
	Texture(const Texture& t);
	~Texture(void);

	short GetTextureU() const;
	short GetTextureV() const;
	void SetTextureU(const short u);
	void SetTextureV(const short v);

	Texture& operator= (const Texture& rhs);

private:
	short _u;
	short _v;

	void Copy(const Texture& t);
};
