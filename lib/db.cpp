//
// Created by lin on 19-3-12.
//
#include <db.h>
#include <glog/logging.h>
#include <iostream>
#include <client.h>

// 启动 tina engine
tina::db::Engine *tina::db::TinaEngine::start() {
    LOG(INFO) << "Hello,I'm starting work now";
    while (true) {
        // trying to parse command
        parse();
        // trying to dispatcher

        // exit / ctrl + D

    }
    return this;
}

std::string tina::db::TinaEngine::tina_version = "0.0.1";

std::string *tina::db::TinaEngine::show_version() {
    std::string show_version = "\nTina version " + TinaEngine::tina_version + " 2019-3-13 16:11:23\n"
            "Enter \".help\" for usage hints.\n"
            "Connected to a transient database.\n"
            "\".open\" FILENAME to reopen on a persisent database.\n";
    LOG(INFO) << show_version;
    std::cout << show_version << std::endl;
    return &TinaEngine::tina_version;
}

tina::db::Engine *tina::db::TinaEngine::parse() {
    client::InputBuffer *input_buffer = new client::InputBuffer();
    input_buffer->print_prompt();
    client::InputBuffer::read_input(*input_buffer);
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
