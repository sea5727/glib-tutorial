#include <stdio.h>
#include <glib.h>
#include <gio/gio.h>

static gboolean
cb_stdin (GIOChannel *source, GIOCondition cond,
    gpointer data) {
    pthread_t self = pthread_self();

    gchar *line = NULL;
    GIOStatus status;
    status = g_io_channel_read_line(source, &line, NULL, NULL, NULL);
    if(status == G_IO_STATUS_NORMAL) {
        g_print("%lu]RECV:%s", self, line);
        g_free(line);
    } else if(status == G_IO_STATUS_ERROR) {
        g_print("%lu]G_IO_STATUS_ERROR\n", self);
    } else if(status == G_IO_STATUS_EOF) {
        g_print("%lu]G_IO_STATUS_EOF\n", self);
    } else if(status == G_IO_STATUS_AGAIN) {
        g_print("%lu]G_IO_STATUS_AGAIN\n", self);
    }
    return TRUE; // FALSE : once, TRUE : infinite
}

static gboolean
cb_test (GIOChannel *source, GIOCondition cond,
    gpointer data) {

    g_print("cb_test start\n");
    
    return TRUE;
}


static gboolean
main_cb_test (gpointer user_data) {

    g_print("main_cb_test start\n");
    
    return TRUE;
}

gboolean 
func(gpointer user_data){

}

int main(int argc, char *argv[]){
    pthread_t self = pthread_self();
    g_print("%lu] =====> start <======= \n", self);

    GMainLoop * gloop = NULL;
    GIOChannel * io_stdin = NULL;

    gloop = g_main_loop_new(NULL, FALSE);
    
    io_stdin = g_io_channel_unix_new(fileno(stdin));
    GIOChannel a;
    g_io_channel_new_file;
    g_io_add_watch(io_stdin, G_IO_IN, cb_stdin, NULL);
    // g_idle_add(main_cb_test, NULL);
    g_timeout_add(1000, main_cb_test, NULL);
    // g_timeout_add
    // guint source = g_io_add_watch(io_stdin, G_IO_IN, cb_test, NULL);


    GMainContext * gmain_context = g_main_context_new();

    // GSource *src = g_idle_source_new ();
    // g_source_set_callback (src, func, NULL, NULL);
    // g_source_attach (src, g_main_loop_get_context (my_loop));
    // g_source_unref (src);

    g_print("g_main_loop_new:%p\n", gloop);
    g_print("gmain_context:%p\n", gmain_context);
    g_main_loop_run (gloop);
    g_io_channel_unref (io_stdin);
    return 0;
}