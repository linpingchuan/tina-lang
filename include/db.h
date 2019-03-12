//
// Created by lin on 19-3-12.
//

#ifndef TINA_LANG_DB_H
#define TINA_LANG_DB_H

#include <iostream>

namespace tina {
    namespace db {
        class Engine {
        public:
            virtual Engine *start();
        };

        class TinaEngine {
        public:
            // 初始化资源
            TinaEngine(){}
            // 清除资源
            ~TinaEngine(){}
            // 启动 tina engine
            Engine *start();
        };

    }
}


#endif //TINA_LANG_DB_H
