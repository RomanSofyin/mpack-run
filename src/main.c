#include <stdio.h>

#include "hexDump.h"
#include "mpack.h"

enum e_enum
{
    VAL0 = 0,
    VAL1,
    VAL01 = VAL1,
    VAL2,
    VAL3,
    VAL4
};

typedef struct s_subentry
{
    double d;
    enum e_enum e;
    float f;
} t_subentry;
#define SUBENTRY_FIELD_COUNT (3) // double, int, fload

typedef struct s_entry
{
    int n;
    char *s;
    t_subentry se;
    bool b;
} t_entry;
#define ENTRY_FIELD_COUNT (4) // int, utf8_cstr, array, bool

#define define_test_arr                                                                               \
    t_entry arr[] = {                                                                                 \
        {.n = 1, .s = "string 001", .se = {.d = 1.111, .e = VAL1, .f = 1.112f}, .b = true},           \
        {.n = 0x23456789, .s = "string 002", .se = {.d = 2.221, .e = VAL2, .f = 2.222f}, .b = false}, \
        {.n = 0, .s = "", .se = {.d = 0, .e = VAL0, .f = 0}, .b = 0},                                 \
        {.n = 3, .s = "string 003", .se = {.d = 3.331, .e = VAL3, .f = 3.332f}, .b = true},           \
    }

int msgpack_homepage_example()
{
    // encode to memory buffer
    char *data = NULL;
    size_t size = 0;

    {
        /* Encode the data using Write API */
        printf("The data gonna be encoded is %ld in length\n", strlen("compact") + sizeof(bool) + strlen("schema") + sizeof(uint64_t));
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
            free(data); // If ptr is NULL, no operation is performed.
            return -2;
        }
    }

    free(data);
    return 0;
}

int custom_data_blind_test()
{
    define_test_arr;
    // encode to memory buffer
    char *data = NULL;
    size_t size = 0;

    printf("The data gonna be encoded is %ld in length\n", sizeof(arr));
    /* Encode the data using Write API */
    mpack_writer_t writer;
    mpack_writer_init_growable(&writer, &data, &size);
    mpack_start_array(&writer, sizeof(arr));

    for (size_t i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
    {
        mpack_start_bin(&writer, sizeof(arr[0]));
        mpack_write_bytes(&writer, (const char *)&arr[i], sizeof(arr[0]));
        mpack_finish_bin(&writer);
    }

    mpack_finish_array(&writer);

    // finish writing
    if (mpack_writer_destroy(&writer) != mpack_ok)
    {
        fprintf(stderr, "An error occurred encoding the data!\n");
        return (-1);
    }

    hexDump("--- MPACKed data ---", data, size);

    free(data);
    return 0;
}

int custom_data_diligent_test()
{
    // encode to memory buffer
    char *data = NULL;
    size_t size = 0;
    define_test_arr;

    {
        /* Encode the data using Write API */
        printf("The data gonna be encoded is %ld in length\n", sizeof(arr));
        mpack_writer_t writer;
        mpack_writer_init_growable(&writer, &data, &size);

        /* write out `arr` as "array" */
        mpack_start_array(&writer, sizeof(arr) / sizeof(arr[0]));
        for (size_t i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
        {
            printf("--- writing element %lu ---\n", i);

            /* write out `t_entry` instance as "array" */
            mpack_start_array(&writer, ENTRY_FIELD_COUNT);

            t_entry *p_e = arr + i;
            mpack_write_int(&writer, p_e->n);
            mpack_write_utf8_cstr(&writer, p_e->s);

            /* write out `t_subentry` instance as "array" */
            t_subentry *p_se = &p_e->se;
            mpack_start_array(&writer, SUBENTRY_FIELD_COUNT);
            mpack_write_double(&writer, p_se->d);
            mpack_write_int(&writer, p_se->e);
            mpack_write_float(&writer, p_se->f);
            /* finish writing `t_subentry` instance */
            mpack_finish_array(&writer);

            mpack_write_bool(&writer, p_e->b);
            /* finish writing `t_entry` instance */
            mpack_finish_array(&writer);
        }
        /* finish writing `arr` */
        mpack_finish_array(&writer);

        // finish writing
        if (mpack_writer_destroy(&writer) != mpack_ok)
        {
            fprintf(stderr, "An error occurred encoding the data!\n");
            return (-1);
        }
    }

    hexDump("--- MPACKed data ---", data, size);

    {
        /* Decode the data MPACK'ed using Node API */
        mpack_tree_t tree;
        mpack_tree_init_data(&tree, data, size);
        mpack_tree_parse(&tree);

        mpack_node_t root = mpack_tree_root(&tree);
        char tag_descr[128];
        mpack_tag_debug_describe(mpack_node_tag(root), tag_descr, sizeof(tag_descr));
        size_t arr_len = mpack_node_array_length(root);
        mpack_node_t node_1 = mpack_node_array_at(root, 1);
        printf("Data read-: root=%p, tag_descr=%s, arr_len=%ld, node_1=%p\n", &root, tag_descr, arr_len, &node_1);

        if (mpack_tree_destroy(&tree) != mpack_ok)
        {
            fprintf(stderr, "An error occurred decoding the data!\n");
            free(data); // If ptr is NULL, no operation is performed.
            return -2;
        }
    }

    free(data);
    return 0;
}

int main(int c, char **v)
{
    //msgpack_homepage_example();
    //custom_data_blind_test();
    custom_data_diligent_test();
    printf("All is fine\n");
    return 0;
}
