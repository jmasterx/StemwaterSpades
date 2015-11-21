#include "Game/Utility/Vec2.hpp"
#include "math.h"

Vec2::Vec2(void)
: m_x(0.0f), m_y(0.0f),m_ang(0.0f)
{

}

Vec2::Vec2( float x, float y )
: m_x(x),m_y(y),m_ang(0.0f)
{

}

Vec2::~Vec2(void)
{
}

float Vec2::getX() const
{
	return m_x;
}

float Vec2::getY() const
{
	return m_y;
}


bool Vec2::operator==( const Vec2& v ) const
{
	return v.m_x == m_x && v.m_y == m_y;
}

bool Vec2::operator!=( const Vec2& v ) const
{
	return !(v == *this);
}

Vec2& Vec2::operator+=( const Vec2& v )
{
	m_x += v.m_x;
	m_y += v.m_y;
	return *this;
}

Vec2& Vec2::operator+=( const float& f )
{
	m_x += f;
	m_y += f;
	return *this;
}

Vec2& Vec2::operator-=( const Vec2& v )
{
	m_x -= v.m_x;
	m_y -= v.m_y;
	return *this;
}

Vec2& Vec2::operator-=( const float& f )
{
	m_x -= f;
	m_y -= f;
	return *this;
}

Vec2& Vec2::operator*=( const Vec2& v )
{
	m_x *= v.m_x;
	m_y *= v.m_y;
	return *this;
}

Vec2& Vec2::operator*=( const float& f )
{
	m_x *= f;
	m_y *= f;
	return *this;
}

Vec2& Vec2::operator/=( const Vec2& v )
{
	m_x /= v.m_x;
	m_y /= v.m_y;
	return *this;
}

Vec2& Vec2::operator/=( const float& f )
{
	m_x /= f;
	m_y /= f;
	return *this;
}

const Vec2 Vec2::operator+( const Vec2& v ) const
{
	return Vec2(*this) += v; 
}

const Vec2 Vec2::operator+( const float& f ) const
{
	return Vec2(*this) += f; 
}

const Vec2 Vec2::operator-( const Vec2& v ) const
{
	return Vec2(*this) -= v; 
}

const Vec2 Vec2::operator-( const float& f ) const
{
	return Vec2(*this) -= f; 
}

const Vec2 Vec2::operator*( const Vec2& v ) const
{
	return Vec2(*this) *= v; 
}

const Vec2 Vec2::operator*( const float& f ) const
{
	return Vec2(*this) *= f; 
}

const Vec2 Vec2::operator/( const Vec2& v ) const
{
	return Vec2(*this) /= v; 
}

const Vec2 Vec2::operator/( const float& f ) const
{
	return Vec2(*this) /= f; 
}

float Vec2::dot( const Vec2& v ) const
{
	return ((v.m_x * m_x) + (v.m_y * m_y));

}

float Vec2::length() const
{
	return sqrt((m_x * m_x) + (m_y * m_y));
}

void Vec2::normalize()
{
	float len = length();
	if(len == 0.0f)
	{
		return;
	}

	m_x /= len;
	m_y /= len;
}

Vec2 Vec2::normalized()
{
	Vec2 v = Vec2(*this);
	v.normalize();
	return v;
}

void Vec2::setX( float x )
{
	m_x = x;
}

void Vec2::setY( float y )
{
	m_y = y;
}

void Vec2::setAngle( float ang )
{
	m_ang = ang;
}

float Vec2::getAngle() const
{
	return m_ang;
}

