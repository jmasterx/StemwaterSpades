#ifndef CGE_SHADER_HPP
#define CGE_SHADER_HPP
#include <string>
#include "Game/platform.hpp"
#include "Agui/Image.hpp"
#include "Game/Resource/Sprite.hpp"
namespace cge
{
	class Shader 
	{
	public:
		~Shader();
		Shader();
		Shader( const std::string & vtxsrcpath, const std::string & pxsrcpath);
	bool load( const std::string & vtxsrcpath, const std::string & pxsrcpath);
	void use();
	void stop();
	bool setSampler(agui::Image* img, const std::string& name, int unit = 0);
	bool setSampler(Sprite* img, const std::string& name, int unit = 0);
	bool setSampler(ALLEGRO_BITMAP* img, const std::string& name, int unit = 0);
	void destroy();
	void printLog();
	ALLEGRO_SHADER* getShader();
	void setVal(const std::string& name, float val);
	void setVal(const std::string& name, int val);
	void setVal(const std::string& name, bool val);
	void mountShader();
	static void setGlsl(bool glsl);
	static bool isGlsl();
	static bool isHlsl();
	static Shader* loadShader(const std::string& name);
	static void setEnabled(bool enabled);
	static bool isEnabled();
	private:
		void create();
		bool loadSource(const std::string & srcpath, bool pixelShader);
		bool link();
		static bool m_glsl;
		static bool m_enabled;
		ALLEGRO_SHADER* m_shader;
		std::string m_src;
		bool m_inUse;
	};
}

#endif
