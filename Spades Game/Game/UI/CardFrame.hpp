#ifndef CGE_CARD_FRAME_HPP
#define CGE_CARD_FRAME_HPP
#include "Agui/Widget.hpp"
#include "Game/Resource/Sprite.hpp"
#include "Game/Engine/GraphicsContext.hpp"
#include "Agui/Widgets/Frame/Frame.hpp"
#include "Game/UI/GuiFactory.hpp"
#include "Agui/ActionListener.hpp"
namespace cge
{
	class CardFrame : public agui::Frame,
		public agui::ActionListener
	{
		Sprite* m_shadowImg;
		agui::Image* m_cardImg;
		GraphicsContext* m_context;
		std::stack<agui::Rectangle> m_stackRects;
		agui::Point m_stackOffset;
		float m_bgOpacity;
		GuiFactory* m_factory;

		FlowLayout* m_topLayout;
		FlowLayout* m_bottomLayout;

		Button* m_exitButton;
		Button* m_cancelButton;
		Button* m_okButton;

		int m_resizeHeight;
		agui::ActionListener* m_notifyWidget;
		agui::EmptyWidget* m_empty;

		void  _recursivePaintPublicChildren( agui::Widget *root, bool enabled,
			agui::Graphics *graphicsContext );

		void  _recursivePaintPrivateChildren( agui::Widget *root, bool enabled,
			agui::Graphics *graphicsContext );

		void paintPrivateChildren(const agui::PaintEvent &paintEvent );
		void paintPublicChildren(const agui::PaintEvent &paintEvent );

	public:
		void resizeWidgetsInside(bool remarginFrame = true);
		virtual void logic(double timeElapsed);
		virtual void actionPerformed(const agui::ActionEvent& evt);
		void paintTop(const agui::PaintEvent &paintEvent);
		CardFrame(Sprite* shadow, agui::Image* cardImg, GraphicsContext* context, GuiFactory* factory);
		virtual void paintBackground(const agui::PaintEvent &paintEvent);
		virtual void paintComponent(const agui::PaintEvent &paintEvent);
		void setSizeFromHeight(int height);
		virtual void setSize(const agui::Dimension &size);
		virtual void setSize(int width, int height);
		virtual void parentSizeChanged();
		void setResizeExtraHeight(int extraH);
		void setNotifyWidget(agui::ActionListener* listener);
		void showDialog(bool modal = true);
		void hideDialog();
		void setNames(const std::string& okName, const std::string& cancelName);
		virtual ~CardFrame(void);
	};
}


#endif

