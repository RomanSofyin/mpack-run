#include <stdio.h>

#include "hexDump.h"
#include "mpack.h"

int main(int c, char **v)
{
    // encode to memory buffer
    char* data;
    size_t size;
    mpack_writer_t writer;
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
        return -1;
    }

    // use the data
    hexDump("--- MPACKed data ---", data, size);
    free(data);

    printf("All is fine\n");
    return 0;
}
