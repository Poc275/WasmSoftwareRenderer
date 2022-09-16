#pragma once
class Coordinate
{
public:
	Coordinate(void);
	Coordinate(float x, float y, float z);
	Coordinate(float x, float y, float z, float w);
	Coordinate(const Coordinate& coord);
	~Coordinate(void);

	float GetX(void) const;
	float GetY(void) const;
	float GetZ(void) const;
	float GetW(void) const;

	void SetX(float x);
	void SetY(float y);
	void SetZ(float z);
	void SetW(float w);

	Coordinate& operator= (const Coordinate& rhs);

protected:
	float _x;
	float _y;
	float _z;
	float _w;

private:
	void Init(float x, float y, float z, float w);
	void Copy(const Coordinate& coord);
};
