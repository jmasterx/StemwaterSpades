#ifndef CGE_PASS_CARDS_HANDLER_HPP
#define CGE_PASS_CARDS_HANDLER_HPP
#include "Game/Element/DynamicElement.hpp"
#include "Game/Handler/ClientEventListener.hpp"
#include "Game/UI/GuiFactory.hpp"
#include "Agui/Gui.hpp"
#include "Agui/ActionListener.hpp"
namespace cge
{
	class PassCardsHandler : public DynamicElement, public ClientEventListener,
		public agui::ActionListener
	{
		
		GuiFontManager* m_fontManager;
		std::vector<ClientEventListener*> m_listeners;
		OrbButton* m_button;
	public:
		PassCardsHandler(GuiFontManager* fontManager,LanguageManager* languageManager,
			GuiFactory* factory, agui::Gui* gui);
		virtual void resize(int w, int h);
		virtual void actionPerformed(const agui::ActionEvent& evt);
		void addListener(ClientEventListener* listener);
		void removeListener(ClientEventListener* listener);
		virtual void passCardsBegin();
		virtual void passButtonEnabledChanged(bool enabled);
		virtual void passButtonVisibilityChanged( bool visible );
		virtual ~PassCardsHandler(void);
	};
}

#endif
