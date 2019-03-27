//
// Created by lin on 19-3-25.
//

#ifndef TINA_LANG_CLIENT_H
#define TINA_LANG_CLIENT_H

#include <cstddef>
#include <stdio.h>
#include <db.h>

namespace tina {
    namespace db {
        class Context;
        class TinaEngine;

        namespace client {

            class InputBuffer;

            class InputBuffer {
            public:
                InputBuffer() : buffer(nullptr), buffer_length(0), input_length(0) {}

                void print_prompt();

                static void read_input(InputBuffer &input_buffer);


                friend class tina::db::Context;
                friend class tina::db::TinaEngine;
            private:
                char *buffer;
                size_t buffer_length;
                ssize_t input_length;
            };
        }
    }
}
#endif //TINA_LANG_CLIENT_H
