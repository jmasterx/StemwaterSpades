#ifndef CGE_LOBBY_CONTAINER_HPP
#define CGE_LOBBY_CONTAINER_HPP
#include "Game/UI/GuiFactory.hpp"
#include "Game/Handler/LobbyEventListener.hpp"
#include "Game/UI/LobbyFilterManager.hpp"
#include "Agui/WidgetListener.hpp"
#include "Game/Element/DynamicElement.hpp"
namespace cge
{
	class LobbyContainer : public LobbyEventListener,
		public agui::WidgetListener, public DynamicElement
	{
		BorderLayout* m_outerBorder;
		BorderLayout* m_innerBorder;
		LobbyFilterManager* m_filterManager;
		bool m_filterOn;
	public:
		LobbyContainer(GuiFactory* factory, LanguageManager* language);
		void addOuter(agui::Widget* source,agui::BorderLayout::BorderLayoutEnum which);
		void addInner(agui::Widget* source,agui::BorderLayout::BorderLayoutEnum which);
		BorderLayout* getInner();
		BorderLayout* getOuter();
		agui::Widget* getWidget();
		virtual void toggleFilterMenu(bool on);
		void setFilterManager(LobbyFilterManager* filter);
		virtual void sizeChanged(agui::Widget* source,
			const agui::Dimension &size);
		virtual void resize(int w, int h);
		virtual ~LobbyContainer(void);
	};
}

#endif
