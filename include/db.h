//
// Created by lin on 19-3-12.
//

#ifndef TINA_LANG_DB_H
#define TINA_LANG_DB_H

#include <map>
#include "client.h"

namespace tina {
    namespace db {
        class Engine;

        class TinaEngine;

        class Context;

        class Value;

        class TinaContext;

        struct Statement;

        struct Row;

        namespace client {
            class InputBuffer;
        }
        enum MetaCommandResult {
            META_COMMAND_SUCCESS,
            META_COMMAND_UNRECOGNIZED_COMMAND
        };

        enum MetaPrepareResult {
            PREPARE_SUCCESS,
            PREPARE_UNRECOGNIZED_STATEMENT,
            PREPARE_SYNTAX_ERROR
        };

        enum MetaExecuteResult {
            EXECUTE_SUCCESS,
            EXECUTE_UNRECOGNIZED_STATEMENT
        };
        enum StatementType {
            STATEMENT_INSERT,
            STATEMENT_SELECT
        };

        class Context {
        public:
            Context() {}

            virtual Context *initial()=0;

            virtual void destroy()=0;

            friend class TinaEngine;

        protected:
            tina::db::client::InputBuffer *input_buffer;
            tina::db::Statement *statement;
        };

        class Command {
        public:
            Command(std::string &command) : command(&command) {}


        private:
            std::string *command;
            int64_t length;
        };

        class Value {

        };

        class TinaContext : public Context {
        public:
            TinaContext() : Context() { this->initial(); }

            ~TinaContext() { this->destroy(); }

            Context *initial();

            void destroy();

        protected:
            std::map<std::string, Value *> values;
        private:

        };

        class Engine {
        public:
            virtual Engine *start()=0;

            virtual Engine *parse()=0;

            virtual void bye()=0;

        protected:
            db::Context *context;
        };

        class TinaEngine : public Engine {
        public:
            // 初始化资源
            TinaEngine() {
                context = new TinaContext();
            }

            // 清除资源
            ~TinaEngine() {
                this->bye();
            }

            // 启动 tina engine
            Engine *start();

            Engine *parse();

            MetaCommandResult dispatch_meta_command();

            MetaPrepareResult dispatch_prepare_command();

            MetaExecuteResult dispatch_execute_command();

            Engine *dispatch();

            Engine *prepare_statement();

            Engine *execute_statement();

            void bye();

            static std::string *show_version();

            static std::string *show_happy();

        protected:
            static std::string tina_version;
        };

        struct Row {
            static const int64_t  COLUMN_SIZE=255;
            char username[255];
            char email[255];
            int id;
        };

        struct Statement {
            StatementType type;
            std::string statement;
            Row *row;
        };


    }
}


#endif //TINA_LANG_DB_H
