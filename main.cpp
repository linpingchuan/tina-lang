//
// Created by lin on 19-2-27.
//
#include <common.h>

int main(int argc, char **argv) {
    std::string command="I love You,Tina";
    tina::db::run(command);

    tina::db::stop();
    return 0;
}
