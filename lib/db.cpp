//
// Created by lin on 19-3-12.
//
#include <db.h>
#include <glog/logging.h>
#include <iostream>
#include <csignal>

// 启动 tina engine
tina::db::Engine *tina::db::TinaEngine::start() {
    LOG(INFO) << "Hello,I'm starting work now";
    while (true) {
        // find a string
        parse();
        // trying to dispatcher
        dispatch();
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

std::string *tina::db::TinaEngine::show_happy() {
    std::string *show_happy = new std::string("I am love Tina :)");
    std::cout << *show_happy << std::endl;
    return show_happy;
}

tina::db::Engine *tina::db::TinaEngine::parse() {
    auto input_buffer = new client::InputBuffer();
    input_buffer->print_prompt();
    client::InputBuffer::read_input(*input_buffer);

    context->input_buffer = input_buffer;
    return this;
}

tina::db::Engine *tina::db::TinaEngine::dispatch() {
    auto buffer = context->input_buffer;
    if (buffer->buffer[0] == '.') {
        auto meta_command_result = dispatch_meta_command();
        switch (meta_command_result) {
            case META_COMMAND_SUCCESS:
                // pass
                break;
            case META_COMMAND_UNRECOGNIZED_COMMAND:
                printf("Unrecognized meta command '%s',type '.h' or '.help' get more usages\n", buffer->buffer);
                break;
        }
    } else {
        prepare_statement();
    }

}

tina::db::MetaCommandResult tina::db::TinaEngine::dispatch_meta_command() {
    auto buffer = context->input_buffer;
    if (strcmp(buffer->buffer, ".exit") == 0) {
        bye();
        exit(EXIT_SUCCESS);
    } else if (strcmp(buffer->buffer, ".version") == 0) {
        show_version();
        return META_COMMAND_SUCCESS;
    } else if (strcmp(buffer->buffer, ".h") == 0 ||
               strcmp(buffer->buffer, ".help") == 0
            ) {
        show_version();
        return META_COMMAND_SUCCESS;
    } else if (strcmp(buffer->buffer, ".happy") == 0) {
        show_happy();
        return META_COMMAND_SUCCESS;
    } else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }

}

tina::db::MetaPrepareResult tina::db::TinaEngine::dispatch_prepare_command() {
    return PREPARE_UNRECOGNIZED_STATEMENT;
}

tina::db::Engine *tina::db::TinaEngine::prepare_statement() {
    auto buffer = context->input_buffer;
    LOG(INFO) << "prepare statement: " << buffer->buffer << std::endl;
    auto prepare_result = dispatch_prepare_command();
    switch (prepare_result) {
        case PREPARE_SUCCESS:
            LOG(INFO) << "prepare statement success: " << buffer->buffer << std::endl;
            execute_statement();
            break;
        case PREPARE_UNRECOGNIZED_STATEMENT:
//            LOG(ERROR) << "prepare statement error: " << buffer->buffer << std::endl;
            printf("Unrecognized keyword at start of '%s'.\n", buffer->buffer);
            break;
    }
    return this;
}

tina::db::Engine *tina::db::TinaEngine::execute_statement() {

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
