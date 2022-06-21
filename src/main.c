#include <stdio.h>

#include "hexDump.h"

int main(int c, char **v)
{
    // encode to memory buffer
    char* data;
    size_t size;
    /*mpack_writer_t writer;
    mpack_writer_init_growable(&writer, &data, &size);

    // write the example on the msgpack homepage
    mpack_build_map(&writer);
    mpack_write_cstr(&writer, "compact");
    mpack_write_bool(&writer, true);
    mpack_write_cstr(&writer, "schema");
    mpack_write_uint(&writer, 0);
    mpack_complete_map(&writer);

    // finish writing
    if (mpack_writer_destroy(&writer) != mpack_ok) {
        fprintf(stderr, "An error occurred encoding the data!\n");
        return;
    }

    // use the data
    do_something_with_data(data, size);
    free(data);*/

    char test_str[] = "this is just a string to test how printf_data32() functions prints the data passed in";
    hexDump("--- test_ints ---", test_str, sizeof(test_str));

    char test_ints[] = { 0x12, 0x34, 0x56, 0x78, 0xfe, 0xed, 0xfa, 0xce, 0x0b, 0xad, 0xda, 0xce, 0x34, 0x56, 0x78, 0xfe, 0xed, 0xfa, 0xce, 0x0b, 0xad, 0xda, 0xce, 0x34, 0x56, 0x78, 0xfe, 0xed, 0xfa, 0xce, 0x0b, 0xad, 0xda, 0xce, 0x34, 0x56, 0x78, 0xfe, 0xed, 0xfa, 0xce, 0x0b, 0xad, 0xda, 0xce };
    hexDump("--- test_ints ---", test_ints, sizeof(test_ints));
    printf("All is fine\n");

    return 0;
}