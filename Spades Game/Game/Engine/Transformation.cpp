#include "Game/Engine/Transformation.hpp"


Transformation::Transformation(void)
{
	identity();
}

Transformation::~Transformation(void)
{
}

ALLEGRO_TRANSFORM& Transformation::getTransform()
{
	return m_transform;
}

void Transformation::identity()
{
	al_identity_transform(&m_transform);
}

void Transformation::translate( float x, float y )
{
	al_translate_transform(&m_transform,x,y);
}

void Transformation::rotate( float deg )
{
	al_rotate_transform(&m_transform,deg);
}

void Transformation::scale( float x, float y )
{
	al_scale_transform(&m_transform,x,y);
}
