#include <stdio.h>

#include "hexDump.h"
#include "mpack.h"

int msgpack_homepage_example()
{
    // encode to memory buffer
    char *data = NULL;
    size_t size = 0;

    {
        /* Encode the data using Write API */
        mpack_writer_t writer;
        mpack_writer_init_growable(&writer, &data, &size);

        // write the example on the msgpack homepage
        mpack_build_map(&writer);
        mpack_write_cstr(&writer, "compact");
        mpack_write_bool(&writer, false);
        mpack_write_cstr(&writer, "schema");
        mpack_write_uint(&writer, 123);
        mpack_complete_map(&writer);

        // finish writing
        if (mpack_writer_destroy(&writer) != mpack_ok)
        {
            fprintf(stderr, "An error occurred encoding the data!\n");
            return -1;
        }
    }

    // Print out the data MPACKed
    hexDump("--- MPACKed data ---", data, size);

    {
        /* Decode the data MPACK'ed using Node API */
        bool compact;
        int schema;
        mpack_tree_t tree;
        mpack_tree_init_data(&tree, data, size);
        mpack_tree_parse(&tree);

        mpack_node_t root = mpack_tree_root(&tree);
        compact = mpack_node_bool(mpack_node_map_cstr(root, "compact"));
        schema = mpack_node_int(mpack_node_map_cstr(root, "schema"));
        printf("Data read: compact=%s, schema=%d\n", compact ? "TRUE" : "FALSE", schema);

        if (mpack_tree_destroy(&tree) != mpack_ok)
        {
            fprintf(stderr, "An error occurred decoding the data!\n");
            free(data);     // If ptr is NULL, no operation is performed.
            return -2;
        }
    }

    free(data);
    return 0;
}

int main(int c, char **v)
{
    msgpack_homepage_example();
    printf("All is fine\n");
    return 0;
}
