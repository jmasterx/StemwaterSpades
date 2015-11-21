#include "Game/Shader/Shader.hpp"
#include "Agui/Backends/Allegro5/Allegro5Image.hpp"

#include <iostream>
#ifdef _WIN32
#include <allegro5/allegro_windows.h>
#include <allegro5/allegro_direct3d.h>
#endif

namespace cge
{
	void Shader::destroy()
	{
		if(m_shader)
		{
			if(m_inUse)
			{
				stop();
			}
			al_destroy_shader(m_shader);
			m_shader = NULL;
		}
	}

	void Shader::create()
	{
		destroy();
		m_shader = al_create_shader(isGlsl() ? ALLEGRO_SHADER_GLSL : ALLEGRO_SHADER_HLSL);
	
	}

	bool Shader::loadSource( const std::string & srcpath, bool pixelShader )
	{
		return al_attach_shader_source_file(m_shader,
			pixelShader ? ALLEGRO_PIXEL_SHADER : ALLEGRO_VERTEX_SHADER,srcpath.c_str());
	}

	Shader::Shader()
		: m_shader(NULL),m_inUse(false)
	{

	}

	Shader::Shader( const std::string & vtxsrcpath, const std::string & pxsrcpath)
		: m_shader(NULL),m_inUse(false)
	{
		load(vtxsrcpath,pxsrcpath);
	}

	bool Shader::link()
	{
		return al_build_shader(m_shader);
	}

	bool Shader::load( const std::string & vtxsrcpath, const std::string & pxsrcpath)
	{
		std::string appendV = isHlsl() ? ".hlslv" : ".glslv";
		std::string appendP = isHlsl() ? ".hlslp" : ".glslp";

		create();
		if(!m_shader)
		{
			std::cout << "create fail\n";
			return false;
		}

		if(vtxsrcpath != "")
		{
			if(!loadSource(vtxsrcpath + appendV,false))
			{
				std::cout << "vtx src load fail\n";
				fprintf(stderr, "%s\n", al_get_shader_log(m_shader));
				return false;
			}
		}

		if(pxsrcpath != "")
		{
			if(!loadSource(pxsrcpath + appendP,true))
			{
				std::cout << "px src load fail\n";
				fprintf(stderr, "%s\n", al_get_shader_log(m_shader));
				return false;
			}
		}
	
		if(!link())
		{
			std::cout << "link fail\n";
			return false;
		}
		return true;
	}

	void Shader::use()
	{
		if(!m_enabled)
			return;
		m_inUse = true;
		mountShader();
		al_use_shader(NULL);
		al_use_shader(m_shader);
	}

	void Shader::stop()
	{
		al_use_shader(NULL);
	}

	bool Shader::setSampler( agui::Image* img, const std::string& name, int unit /*= 0*/ )
	{
		return al_set_shader_sampler(name.c_str(),((agui::Allegro5Image*)img)->getBitmap(),unit);
	}

	bool Shader::setSampler( Sprite* img, const std::string& name, int unit /*= 0*/ )
	{
		return al_set_shader_sampler(name.c_str(),img->getBitmap(),unit);
	}

	bool Shader::setSampler( ALLEGRO_BITMAP* img, const std::string& name, int unit /*= 0*/ )
	{
		return al_set_shader_sampler(name.c_str(),img,unit);
	}

	Shader::~Shader()
	{	
		destroy();
	
	}

	ALLEGRO_SHADER* Shader::getShader()
	{
		return m_shader;
	}

	void Shader::setVal( const std::string& name, float val )
	{
		al_set_shader_float(name.c_str(),val);
	}

	void Shader::setVal( const std::string& name, int val )
	{
		al_set_shader_int(name.c_str(),val);
	}

	void Shader::setVal( const std::string& name, bool val )
	{
		al_set_shader_bool(name.c_str(),val);
	}

	void Shader::printLog()
	{
		std::cout << al_get_shader_log(m_shader) << "\n"; 
	}


	void Shader::mountShader()
	{
	}

	void Shader::setGlsl( bool glsl )
	{
		m_glsl = glsl;
	}

	bool Shader::isGlsl()
	{
		return m_glsl;
	}

	bool Shader::isHlsl()
	{
		return !isGlsl();
	}

	Shader* Shader::loadShader( const std::string& name )
	{
		if(!m_enabled)
			return NULL;

		std::string path = "res/shader/";
		path += name;
		return new Shader(path,path);
	}

	void Shader::setEnabled( bool enabled )
	{
		m_enabled = enabled;
	}

	bool Shader::isEnabled()
	{
		return m_enabled;
	}

	bool Shader::m_enabled(true);

	bool Shader::m_glsl(false);

}
