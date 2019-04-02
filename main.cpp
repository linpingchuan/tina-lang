//
// Created by lin on 19-2-27.
//
#include <common.h>
#include <boost/thread/thread.hpp>

void start_db();

int main(int argc, char **argv) {
    boost::thread start_db_thread(&start_db);
    start_db_thread.join();
    return 0;
}


void start_db() {
    std::string command = "I love You,Tina";
    tina::db::run(command);
    tina::db::stop();
}
