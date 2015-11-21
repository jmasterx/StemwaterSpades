#ifndef CGE_SOUND_OPTIONS_HPP
#define CGE_SOUND_OPTIONS_HPP
#include "Game/UI/GeneralDialogBox.hpp"
#include "Game/Handler/SceneEventHandler.hpp"
#include <vector>
namespace cge
{
	class SoundOptionsDialog : public GeneralDialogBox,
		public SceneEventProvider
	{
		virtual void dialogWillAppear();
		Slider* m_masterSlider;
		Slider* m_gameSlider;
		Slider* m_giftSlider;
		Slider* m_ambianceSlider;
		CheckBox* m_cbTurn;
	public:
		SoundOptionsDialog(GuiFactory* factory);
		virtual bool wantCloseButton() const;
		virtual void frameActionReceived(GeneralFrame* frame,const std::string& action);
		virtual float getWidthScalar() const;
		virtual float getHeightScalar() const;
		virtual void actionPerformed(const agui::ActionEvent &evt);
		virtual ~SoundOptionsDialog(void);
	};
}
#endif
