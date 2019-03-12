//
// Created by lin on 19-3-12.
//

#ifndef TINA_LANG_DB_H
#define TINA_LANG_DB_H

#include <iostream>
#include <map>

namespace tina {
    namespace db {
        class Engine;

        class TinaEngine;

        class Context;

        class Value;

        class TinaContext;

        class Context {
        public:
            Context() {}

            virtual Context *initial()=0;

            virtual void destroy()=0;
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
                std::cout<<"bye"<<std::endl;
                delete context;
            }

            // 启动 tina engine
            Engine *start();

            void bye();
        };



    }
}


#endif //TINA_LANG_DB_H
