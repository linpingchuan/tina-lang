//
// Created by lin on 19-3-13.
//

#ifndef TINA_LANG_COMMON_H
#define TINA_LANG_COMMON_H

#include <db.h>
#include <string>
#include <iostream>
#include <glog/logging.h>
#include <gflags/gflags.h>

namespace tina {
    namespace db {
        static std::string tina_version = "0.0.1";
        static tina::db::TinaEngine *engine;

        static void run(std::string &strs);

        static void stop();

        static void show_version();

        static void run(std::string &strs) {
            google::InitGoogleLogging("Tina");
            google::SetLogDestination(google::GLOG_INFO, "./tina.log");

            LOG(INFO) << "accept command :" + strs;

            engine = new tina::db::TinaEngine();
            engine->start();

            show_version();
        }

        static void stop() {
            delete engine;
        }

        static void show_version() {
            std::string show_version = "Tina version " + tina_version + " 2019-3-13 16:11:23\n"
                    "Enter \".help\" for usage hints.\n"
                    "Connected to a transient database.\n"
                    "\".open\" FILENAME to reopen on a persisent database.\n";
            std::cout << show_version << std::endl;
        }
    }
}
#endif //TINA_LANG_COMMON_H
