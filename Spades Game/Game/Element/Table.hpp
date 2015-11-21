#ifndef CGE_TABLE_HPP
#define CGE_TABLE_HPP
#include "Game/CardView/ProportionsManager.hpp"
#include "Game/Resource/Sprite.hpp"
#include "Game/Engine/GraphicsContext.hpp"
#include "Game/Element/DynamicElement.hpp"
#include "Game/Shader/Shader.hpp"
namespace cge
{
	class Table : public DynamicElement
	{
		Sprite* m_table;
		Sprite* m_tableShadow;
		bool m_renderTable;
	public:
		Table();
		virtual void render(GraphicsContext* g);
		void setTable(Sprite* table, Sprite* tableShadow, bool render);
		virtual ~Table(void);
	};
}

#endif