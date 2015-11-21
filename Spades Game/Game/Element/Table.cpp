#include "Game/Element/Table.hpp"

namespace cge
{
	Table::~Table(void)
	{
	}

	Table::Table()
		:  m_table(NULL),
		m_tableShadow(NULL),
		m_renderTable(false)
	{
	}

	void Table::setTable( Sprite* table, Sprite* tableShadow, bool render )
	{
		m_table = table;
		m_tableShadow = tableShadow;
		m_renderTable = render;
	}

	void Table::render( GraphicsContext* g )
	{
		if(m_table && m_tableShadow)
		{
			int desiredW = getProportions()->getScale() * getProportions()->getTableSize();
			float scaleFactor = (float)desiredW / (float)1150;
			int shadowW = 1417 * scaleFactor;
			int shadowH = 1517 * scaleFactor;
			int x = (getProportions()->getScreenWidth() * getProportions()->getTableOffset().getX()) - (desiredW / 2);
			int y = (getProportions()->getScreenHeight() * getProportions()->getTableOffset().getY()) - (desiredW / 2);

			if(m_renderTable)
			{
				g->drawScaledSprite(m_table,0,0,
					m_table->getWidth(),m_table->getHeight(),
					x,
					y,
					desiredW,desiredW,0);
			}
			else
			{
				g->drawScaledSprite(m_tableShadow,0,0,
					m_tableShadow->getWidth(),m_tableShadow->getHeight(),
					x - 1 ,y - 1,
					shadowW,shadowH,0);
			}
		}
	}


}

