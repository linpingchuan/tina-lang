//
// Created by lin on 19-3-13.
//

#ifndef TINA_LANG_COMMON_H
#define TINA_LANG_COMMON_H

#include <db.h>
#include <string>
#include <glog/logging.h>
#include <gflags/gflags.h>

namespace tina {
    namespace db {
        static tina::db::TinaEngine *engine;

        static void run(std::string &strs) {
            google::InitGoogleLogging("Tina");
            google::SetLogDestination(google::GLOG_INFO, "./tina.log");

            LOG(INFO) << "accept command :" + strs;
            engine = new tina::db::TinaEngine();
            engine->start();
        }

        static void stop() {
            delete engine;
        }
    }
}
#endif //TINA_LANG_COMMON_H
