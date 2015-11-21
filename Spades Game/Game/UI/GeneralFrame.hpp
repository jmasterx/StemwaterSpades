#ifndef CGE_GENERAL_FRAME_HPP
#define CGE_GENERAL_FRAME_HPP
#include "Agui/Widget.hpp"
#include "Game/Resource/Sprite.hpp"
#include "Game/Engine/GraphicsContext.hpp"
#include "Agui/Widgets/Frame/Frame.hpp"
#include "Game/UI/GuiFactory.hpp"
#include "Agui/ActionListener.hpp"
#include "Game/UI/Frame.hpp"
#include <Agui/WidgetListener.hpp>
namespace cge
{
	class GeneralFrame : public agui::Frame,
		public agui::ActionListener, public agui::WidgetListener
	{
		bool m_tabbingEnabled;
		Frame* m_frame;
		Sprite* m_shadowImg;
		agui::Image* m_bg;
		agui::Image* m_bolt;
		agui::Image* m_frameFill;
		agui::Widget* m_top;
		Label* m_sideLabel;
		GraphicsContext* m_context;
		std::stack<agui::Rectangle> m_stackRects;
		agui::Point m_stackOffset;
		float m_bgOpacity;
	    FlowLayout* m_maskingFlow;
		GuiFactory* m_factory;

		FlowLayout* m_topLayout;
		FlowLayout* m_bottomLayout;

		Button* m_exitButton;
		Button* m_cancelButton;
		Button* m_okButton;

		float m_widthScalar;
		float m_heightScalar;

		int m_resizeHeight;
		agui::ActionListener* m_notifyWidget;

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
		GeneralFrame(Sprite* shadow, agui::Image* cardImg, agui::Image* bolt, agui::Image* frameFill, GraphicsContext* context, GuiFactory* factory);
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
		agui::Widget* getWidget();
		void setFrame(Frame* frame);
		void setTop(agui::Widget* top);
		virtual void sizeChanged(agui::Widget* w, const agui::Dimension & size);
		FlowLayout* getMaskingFlow();
		void setCloseButtonVisibility(bool show);
		void setOkButtonVisibility(bool show);
		void setSizeConstants(float width, float height);
		void setSideLabelText(const std::string& text);
		void addWidgetsToBottom(const std::vector<agui::Widget*>& widgets);
		void removeWidgetsFromBottom(const std::vector<agui::Widget*>& widgets);
		virtual ~GeneralFrame(void);
	};
}


#endif

