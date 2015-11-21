#include "Game/Dialog/SoundOptionsDialog.hpp"
namespace cge
{
	SoundOptionsDialog::~SoundOptionsDialog(void)
	{
		m_masterSlider->removeActionListener(this);
		m_gameSlider->removeActionListener(this);
		m_giftSlider->removeActionListener(this);
		m_ambianceSlider->removeActionListener(this);
	}

	SoundOptionsDialog::SoundOptionsDialog( GuiFactory* factory )
		: GeneralDialogBox(factory,factory->getLanguageManager(),factory->getGuiColorManager(),
		factory->getFontManager())
	{
		setTitle("sound.options");
		setOkText("close");
		addWhitespace(5);
		m_masterSlider = createSlider("master.volume");
		addWhitespace(5);
		m_gameSlider = createSlider("game.sounds");
		addWhitespace(5);
		m_giftSlider = createSlider("gift.sounds");
		//addWhitespace(5);
		//m_ambianceSlider = createSlider("music.ambiance");

		addWhitespace(5);
		m_cbTurn = createCheckBox("turn.sound");
		int maxV = 50;
		m_masterSlider->setMinValue(0); m_masterSlider->setMaxValue(maxV);
		m_gameSlider->setMinValue(0); m_gameSlider->setMaxValue(maxV);
		m_giftSlider->setMinValue(0); m_giftSlider->setMaxValue(maxV);
		//m_ambianceSlider->setMinValue(0); m_ambianceSlider->setMaxValue(maxV);

		m_masterSlider->addActionListener(this);
		m_gameSlider->addActionListener(this);
		m_giftSlider->addActionListener(this);
		//m_ambianceSlider->addActionListener(this);
		m_cbTurn->addActionListener(this);
        
        if(Platf::isRetina())
        {
            int sz = 400;
            m_masterSlider->agui::Widget::setSize(sz, m_masterSlider->getHeight());
            m_gameSlider->agui::Widget::setSize(sz, m_gameSlider->getHeight());
            m_giftSlider->agui::Widget::setSize(sz, m_giftSlider->getHeight());
            //m_ambianceSlider->agui::Widget::setSize(sz, m_ambianceSlider->getHeight());
        }
	}

	void SoundOptionsDialog::dialogWillAppear()
	{
		m_masterSlider->setValue((int)(getNumericSetting("master.volume") * (float)m_masterSlider->getMaxValue()));
		m_gameSlider->setValue((int)(getNumericSetting("game.sounds") * (float)m_gameSlider->getMaxValue()));
		m_giftSlider->setValue((int)(getNumericSetting("gift.sounds") * (float)m_giftSlider->getMaxValue()));
		//m_ambianceSlider->setValue((int)(getNumericSetting("music.ambiance") * (float)m_ambianceSlider->getMaxValue()));
		m_cbTurn->setChecked(getBoolSetting("turn.sound"));
	}

	bool SoundOptionsDialog::wantCloseButton() const
	{
		return false;
	}

	void SoundOptionsDialog::frameActionReceived( GeneralFrame* frame,const std::string& action )
	{
		hideGeneralFrame();
	}

	float SoundOptionsDialog::getWidthScalar() const
	{
		return 0.6f;
	}

	float SoundOptionsDialog::getHeightScalar() const
	{
		return 0.5f;
	}

	void SoundOptionsDialog::actionPerformed( const agui::ActionEvent &evt )
	{
		GeneralDialogBox::actionPerformed(evt);

		if(evt.getSource() == m_masterSlider)
		{
			float val = (float)m_masterSlider->getValue() / m_masterSlider->getMaxValue();
			DISPATCH_SCENE_EVENT
				(*it)->setNumericSetting("master.volume",val);
		}
		else if(evt.getSource() == m_gameSlider)
		{
			float val = (float)m_gameSlider->getValue() / m_gameSlider->getMaxValue();
			DISPATCH_SCENE_EVENT
				(*it)->setNumericSetting("game.sounds",val);
		}
		else if(evt.getSource() == m_giftSlider)
		{
			float val = (float)m_giftSlider->getValue() / m_giftSlider->getMaxValue();
			DISPATCH_SCENE_EVENT
				(*it)->setNumericSetting("gift.sounds",val);
		}
		else if(evt.getSource() == m_cbTurn)
		{
			bool val = m_cbTurn->checked();
			DISPATCH_SCENE_EVENT
				(*it)->setBoolSetting("turn.sound",val);
		}
        /*
		else if(evt.getSource() == m_ambianceSlider)
		{
			float val = (float)m_ambianceSlider->getValue() / m_ambianceSlider->getMaxValue();
			DISPATCH_SCENE_EVENT
				(*it)->setNumericSetting("music.ambiance",val);
		}
         */
	}
}
