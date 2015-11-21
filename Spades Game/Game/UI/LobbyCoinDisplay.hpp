#ifndef CGE_LOBBY_COIN_DISPLAY_HPP
#define CGE_LOBBY_COIN_DISPLAY_HPP
#include "Game/Handler/LobbyEventProvider.hpp"
#include "Game/UI/GuiFactory.hpp"
namespace cge
{
	class LobbyCoinDisplay : public LobbyEventProvider
	{
		ImageIcon* m_moneyIcon;
		FlowLayout* m_container;
		Label* m_amount;
		GuiFactory* m_factory;
	public:
		LobbyCoinDisplay(GuiFactory* factory);
		agui::FlowLayout* getWidget();
		virtual void coinAmountChanged(int64 amount, bool update, bool fromPurchase, bool spending);
		void setColor(const agui::Color& color);
		void setCoinSize(int w);
		void setFontSize(agui::Font* f);
		void reMargin();
		virtual ~LobbyCoinDisplay(void);
	};
}
#endif
