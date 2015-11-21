//
//  OSXHandler.h
//  A51Test
//
//  Created by Josh on 5/4/15.
//  Copyright (c) 2015 josh. All rights reserved.
//

#ifndef CGE_OSX_HANDLER_HPP
#define CGE_OSX_HANDLER_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <allegro5/allegro.h>
struct IAPProduct {
    std::string name;
    std::string description;
    std::string price;
    std::string priceLocale;
    std::string productID;
    std::string priceSymbol;
};
namespace cge {
    class OSXHandler {
    public:
        static OSXHandler& getInstance()
        {
            static OSXHandler  instance; // Guaranteed to be destroyed.
            // Instantiated on first use.
            return instance;
        }
        void launchURL(const std::string& url);
        void checkForDuplicates();
        void setWindowTitle(ALLEGRO_DISPLAY* d, const std::string& caption);
    private:
        OSXHandler() {};                   // Constructor? (the {} brackets) are needed here.
        // C++ 03
        // ========
        // Dont forget to declare these two. You want to make sure they
        // are unacceptable otherwise you may accidentally get copies of
        // your singleton appearing.
        OSXHandler(OSXHandler const&);              // Don't Implement
        void operator=(OSXHandler const&); // Don't implement
        
    };
}
#endif /* defined(__AllegroIOSTest__IAPHandler__) */