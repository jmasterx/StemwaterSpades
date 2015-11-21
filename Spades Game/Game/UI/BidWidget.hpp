#ifndef CGE_BID_WIDGET_HPP
#define CGE_BID_WIDGET_HPP
#include "Agui/Widget.hpp"
#include "Game/Element/DynamicElement.hpp"
#include "Game/UI/Button.hpp"
#include "Game/UI/GuiFontManager.hpp"
#include "Agui/ActionListener.hpp"
#include "Game/Handler/ClientEventListener.hpp"
#include "Game/Handler/SceneEventProvider.hpp"
namespace cge
{
	class BidWidget : public agui::Widget, public agui::ActionListener, public DynamicElement,
		public ClientEventListener, public SceneEventProvider
	{
		GuiFontManager* m_fontManager;
		std::vector<Button*> m_buttons;
		Button* m_nilButton;
		Button* m_bidButton;
		int m_bid;
		int m_numButtonsShowing;
		bool m_nilShowing;
		std::vector<ClientEventListener*> m_listeners;
	protected:
		virtual void paintBackground(const agui::PaintEvent &paintEvent);
		virtual void paintComponent(const agui::PaintEvent &paintEvent);
	public:
		BidWidget(const std::vector<Button*>& buttons,
		Button* nilButton,
		Button* bidButton,
			GuiFontManager* fontManager,
			const std::string& bidText,
			const std::string& nilText);
		virtual void actionPerformed(const agui::ActionEvent &evt);
		virtual void resize(int w, int h);
		virtual void resizeButtons();
		void layoutRow(int yStart, int indexStart, int numOnRow, int width,float spread = 1.0f,
			float angStrength = 1.0f);
		void finishedBidding();
		void startBidding(int numButtons,bool niled, bool moonshot);
		void setBidText(const std::string& text);
		void setNilText(const std::string& text);
		virtual void bidBegin(int numButtons, bool nilActive);
		void addListener(ClientEventListener* listener);
		void removeListener(ClientEventListener* listener);
		int getContentsHeight() const;
		virtual ~BidWidget(void);
	};
}

#endif
