//
// Created by lin on 19-3-12.
//
#include <db.h>

// 启动 tina engine
tina::db::Engine *tina::db::TinaEngine::start() {
    std::cout << "Hello,I am Tina..." << std::endl;
    return this;
}

void tina::db::TinaEngine::bye() {
    std::cout<<"Bye..."<<std::endl;

}

tina::db::Context *tina::db::TinaContext::initial() {
    std::cout<<"Hello, Entering tina context..."<<std::endl;
    return this;
}

void tina::db::TinaContext::destroy() {
    std::cout<<"Bye..."<<std::endl;
}
