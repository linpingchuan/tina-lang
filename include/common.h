//
// Created by lin on 19-3-13.
//

#ifndef TINA_LANG_COMMON_H
#define TINA_LANG_COMMON_H

#include <db.h>

namespace tina {
    namespace db {
        static tina::db::TinaEngine *engine;

        static void run(char **argv) {
            engine = new tina::db::TinaEngine();
            engine->start();
        }

        static void stop() {
            delete engine;
        }
    }
}
#endif //TINA_LANG_COMMON_H
