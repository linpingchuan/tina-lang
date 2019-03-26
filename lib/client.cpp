//
// Created by lin on 19-3-25.
//
#include <client.h>
#include <iostream>
#include <cstring>

void tina::db::client::InputBuffer::print_prompt() {
    std::cout << "db > ";
}

void tina::db::client::InputBuffer::read_input(InputBuffer &input_buffer) {
    ssize_t bytes_read = getline(&(input_buffer.buffer), &(input_buffer.buffer_length), stdin);
    if (bytes_read <= 0) {
        std::cout << "Error reading input\n";
        exit(EXIT_FAILURE);
    }

    // Ignore trailing newline
    input_buffer.input_length = bytes_read - 1;
    input_buffer.buffer[bytes_read - 1] = 0;
}

