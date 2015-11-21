//
//  Platf.h
//  AllegroIOSTest
//
//  Created by Josh on 2/19/15.
//  Copyright (c) 2015 Allegro. All rights reserved.
//

#ifndef CGE_PLATF_HPP
#define CGE_PLATF_HPP
namespace cge
{
    class Platf {
        static bool _isRetina;
		static bool _isHD;
    public:
        static void _setRetina(bool retina);
		static void _setHD(bool hd);
        
        static bool isRetina();
        
        static bool isMobile();
        
        static bool isIOS();
        
        static bool isMini();
        
        static float retinaScalar();

		static bool isHD();
    };
}
#endif
