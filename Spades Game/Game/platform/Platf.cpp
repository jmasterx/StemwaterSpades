//
//  Platf.cpp
//  AllegroIOSTest
//
//  Created by Josh on 2/19/15.
//  Copyright (c) 2015 Allegro. All rights reserved.
//

#include "Game/Platform/Platf.hpp"
#include "Game/platform.hpp"
namespace cge {
    bool Platf::isIOS() {
#ifdef CGE_IPHONE
        return true;
#else
        return false;
#endif
    }
    
    bool Platf::isMobile()
    {
#ifdef CGE_MOBILE
        return true;
#else
        return false;
#endif
    }
    
    bool Platf::isRetina()
    {
        return _isRetina;
    }
    
    bool  Platf::isMini()
    {
        return !isRetina() && isMobile();
    }
    
    void Platf::_setRetina(bool retina)
    {
        _isRetina = retina;
    }
    
    float Platf::retinaScalar() {
        if(isRetina())
        return 2.0f;
        
        return 1.0f;
    }

	void Platf::_setHD( bool hd )
	{
		_isHD = hd;
	}

	bool Platf::isHD()
	{
		return _isHD;
	}


	bool Platf::_isHD(false);



    bool Platf::_isRetina(false);
}