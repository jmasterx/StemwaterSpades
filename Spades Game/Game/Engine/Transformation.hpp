#include "Game/platform.hpp"
class Transformation
{
	ALLEGRO_TRANSFORM m_transform;
public:

	ALLEGRO_TRANSFORM& getTransform();
	void identity();
	void translate(float x, float y);
	void rotate(float deg);
	void scale(float x, float y);
	Transformation(void);
	~Transformation(void);
};
