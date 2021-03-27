#include <glib.h>

static
char * texas_1, *texas_2;

void
key_destroyed(gpointer data){
    g_print("Got a key destroy call for %s\n", data == texas_1 ? "texas_1" : "texas_2");
}

int main(int argc, char *argv[]){
    char * value;
    GHashTable *hash = g_hash_table_new_full(g_str_hash, g_str_equal, key_destroyed, NULL);

    texas_1 = g_strdup("Texas");
    texas_2 = g_strdup("Texas");

    g_hash_table_insert(hash, texas_1, "Austin");
    g_print("Calling insert with the texas_2 key\n");

    value = (char *)g_hash_table_lookup (hash, texas_1);
    g_print("value:%s\n", value);
    
    g_hash_table_insert(hash, texas_2, "Houston");
    g_print("Calling insert with the texas_2 key\n");

    value = (char *)g_hash_table_lookup (hash, texas_1);
    g_print("value:%s\n", value);

    g_hash_table_replace(hash, texas_2, "Third");

    value = (char *)g_hash_table_lookup (hash, texas_1);
    g_print("value:%s\n", value);

    g_print("Destroying hash, so gooybye texas_2\n");
    g_hash_table_destroy(hash);
    g_free(texas_1);
    g_free(texas_2);
    return 0;
}
