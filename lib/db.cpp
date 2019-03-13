//
// Created by lin on 19-3-12.
//
#include <db.h>
#include <glog/logging.h>

// 启动 tina engine
tina::db::Engine *tina::db::TinaEngine::start() {
    LOG(INFO) << "Hello,I'm starting work now";
    return this;
}

void tina::db::TinaEngine::bye() {
    delete this->context;
    LOG(INFO) << "Bye...";

}

tina::db::Context *tina::db::TinaContext::initial() {
    LOG(INFO) << "Hello, Entering tina context...";
    return this;
}

void tina::db::TinaContext::destroy() {
    LOG(INFO) << "Bye from tina context...";
}
