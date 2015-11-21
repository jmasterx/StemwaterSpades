#ifndef CGE_VEC2_HPP
#define CGE_VEC2_HPP

class Vec2
{
	float m_x;
	float m_y;
	float m_ang;
public:
	Vec2(void);
	Vec2(float x, float y);
	~Vec2(void);
	float getX() const;
	float getY() const;
	void setX(float x);
	void setY(float y);
	void setAngle(float ang);
	float getAngle() const;
	bool operator==(const Vec2& v) const;
	bool operator!=(const Vec2& v) const;
	Vec2& operator+=(const Vec2& v);
	Vec2& operator+=(const float& f);
	Vec2& operator-=(const Vec2& v);
	Vec2& operator-=(const float& f);
	Vec2& operator*=(const Vec2& v);
	Vec2& operator*=(const float& f);
	Vec2& operator/=(const Vec2& v);
	Vec2& operator/=(const float& f);
	const Vec2 operator+(const Vec2& v) const;
	const Vec2 operator+(const float& f) const;
	const Vec2 operator-(const Vec2& v) const;
	const Vec2 operator-(const float& f) const;
	const Vec2 operator*(const Vec2& v) const;
	const Vec2 operator*(const float& f) const;
	const Vec2 operator/(const Vec2& v) const;
	const Vec2 operator/(const float& f) const;
	float dot(const Vec2& v) const;
	float length() const;
	void normalize();
	Vec2 normalized();
};

#endif
