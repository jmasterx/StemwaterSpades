#ifndef CGE_GUI_FONT_MANAGER_HPP
#define CGE_GUI_FONT_MANAGER_HPP
#include "Agui/Font.hpp"
#include "Game/platform.hpp"
#include <vector>
namespace cge
{
	class GuiFontManager
	{
		std::vector<agui::Font*> m_fonts;
		int m_beginRange;
		int m_endRange;
		std::string m_fontPath;
		float m_defaultSize;

		std::vector<agui::Font*> m_prettyFonts;
		int m_beginRangePretty;
		int m_endRangePretty;
		std::string m_fontPathPretty;
		float m_defaultSizePretty;

		agui::Font* m_tempFont;
		agui::Font* getFontAt(int index);
		agui::Font* getPrettyFontAt(int index);
	public:
		GuiFontManager(const std::string& fontPath,
			int beginRange, int endRange, int defaultSize,
			const std::string& fontPathPretty,
			int beginRangePretty, int endRangePretty, int defaultSizePretty);
		agui::Font* getDefaultFont();

		agui::Font* getPrettyDefaultFont();
		agui::Font* getPrettyFont(int size);
		agui::Font* getPrettyFont(float scale);
		agui::Font* getPrettyFont(const agui::Font* ref, float scale);

		agui::Font* getFont(int size);
		agui::Font* getFont(float scale);
		agui::Font* getFont(const agui::Font* ref, float scale);
		void loadTempFont(float scale);
		void _loadFonts();
		agui::Font* getTempFont();
		virtual ~GuiFontManager(void);
	};
}

#endif
