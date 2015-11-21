#ifndef CGE_DATA_WIDGETS_HANDLER_HPP
#define CGE_DATA_WIDGETS_HANDLER_HPP
#include "Game/UI/GuiFactory.hpp"
#include "Game/Handler/ClientEventListener.hpp"
#include "Game/Element/DynamicElement.hpp"
namespace cge
{
	class DataWidgetsHandler : public ClientEventListener, public DynamicElement
	{
		Frame* m_frame;
		BorderLayout* m_borderLayout;
		PriorTrickWidget* m_priorTrick;
		PassedCardsWidget* m_passedCards;
		ScoreWidget* m_scoreWidget;
		bool m_passingCards;
		int m_widgetSize;
		int m_numTeams;
		int m_width;
		int m_height;
	public:
		DataWidgetsHandler(GuiFactory* factory, agui::Gui* gui, int numPlayers, int numTeams);
		virtual void roundScoresChanged(const std::vector<int>& scores, const std::vector<int> bags);
		virtual void passTwoCardsChanged(const std::vector<Card>& received, const std::vector<Card>& given);
		virtual void lastHandChanged(const std::vector<Card>& lastChards);
		virtual void gameBegin();
		virtual void roundBegan();
		int calcObjSz(int w);
		int calcHeight(int objSz);
		virtual void resize(int w, int h);
		float solveWidthRatio(int numObj) const;
		int solveHeight( int offset, float widthRatio, float h, float k, float r ) const;
		void setPassingCards(bool passing);
		virtual ~DataWidgetsHandler(void);
	};
}

#endif
