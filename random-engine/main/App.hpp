#ifndef APP_H
#define APP_H

#include <memory>

class AppImpl;

class App {
    public:
        App();
        ~App();
        void mainLoop();
    private:
        std::unique_ptr<AppImpl> appImpl;
};
#endif