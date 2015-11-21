#ifndef CGE_DISPLAY_HPP
#define CGE_DISPLAY_HPP
#include "Game/platform.hpp"
#include "Game/Resource/Sprite.hpp"
#include "Game/Engine/SceneManagerMessage.hpp"
#include "Game/Shader/Shader.hpp"
//represents a display device
namespace cge
{
	class Display
	{
	  DisplayContext* context;
	  SceneManagerMessage* m_sceneMessenger;
	  int minWidth;
	  int minHeight;
	  int m_defaultPosX;
	  int m_defaultPosY;
	  std::string m_caption;
	  void initContext(DisplayContext* c);
	  void resizeSceneForFullscreen(int w, int h);
	public:
		Display(int width, int height, bool fullscreen, bool forceD3D, bool disableShaders);
		void setResolution(int width, int height);
		void setResolution(const Vec2& size);
		void setPosition(int x, int y);
		void setPosition(const Vec2& position);
		bool toggleFullscreen(bool fullscreen);
		void toggleMaximized(bool maximized);
		bool isMaximized() const;
		bool isFullscreen() const;
		Vec2 getPosition() const;
		Vec2 getResolution() const;
		Vec2 getMinResolution() const;
		int getMaxTextureSize()  const;
		void setWindowCaption(const std::string& caption);
		void setSceneMessenger(SceneManagerMessage* messenger);
		static Vec2 getOptimalResoluton(); 
		void setIcon(Sprite* icon);
		DisplayContext* getContext() const;
		static Vec2 getMonitorResolution();
		void doneLoading();
		void toggleMaximizeButton(bool on);
		std::string getVendor() const;
		std::string getGraphicsModel() const;
		const std::string& getWindowCaption() const;
		bool isAppAlreadyRunning() const;
		bool isIntelGMA4000() const;
		int getVramAmount() const;
        bool isLowQuality() const;
        void resetCaption();
		virtual ~Display(void);
	};
}

#endif

