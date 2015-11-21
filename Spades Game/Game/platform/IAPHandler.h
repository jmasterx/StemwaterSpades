//
//  IAPHandler.h
//  AllegroIOSTest
//
//  Created by Josh on 2/27/15.
//  Copyright (c) 2015 Allegro. All rights reserved.
//

#ifndef CGE_IAP_HANDLER_HPP
#define CGE_IAP_HANDLER_HPP

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>

struct IAPProduct {
    std::string name;
    std::string description;
    std::string price;
    std::string priceLocale;
    std::string productID;
    std::string priceSymbol;
    std::string bonusPercent;
};
namespace cge {
    class LobbyNetHandler;
    class SceneManager;
    class IAPHandler {
    public:
        static IAPHandler& getInstance()
        {
            static IAPHandler  instance; // Guaranteed to be destroyed.
            // Instantiated on first use.
            return instance;
        }
        void clear();
        void add(const IAPProduct& product);
        const std::vector<IAPProduct>& getProducts();
        void init();
        void buy(const std::string& pid);
        void setUsername(const std::string& name);
        const std::string& getUsername() const;
        void setLobbyNetHandler(LobbyNetHandler* handler);
        void signalCoinChange();
        void playTick();
        void signalLogin();
        void launchURL(const std::string& url);
        void startBackgroundExecution(SceneManager* scene);
        void endBackgroundExecution(SceneManager* scene);
    private:
        std::vector<IAPProduct> m_products;
        std::string m_username;
        unsigned long m_bgTaskID;
        LobbyNetHandler* m_net;
        IAPHandler() : m_net(NULL),m_bgTaskID(0) {};                   // Constructor? (the {} brackets) are needed here.
        // C++ 03
        // ========
        // Dont forget to declare these two. You want to make sure they
        // are unacceptable otherwise you may accidentally get copies of
        // your singleton appearing.
        IAPHandler(IAPHandler const&);              // Don't Implement
        void operator=(IAPHandler const&); // Don't implement

    };
}
#endif /* defined(__AllegroIOSTest__IAPHandler__) */
