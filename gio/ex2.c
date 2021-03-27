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
        g_print("%u RECV ]%s", self, line);
        g_free(line);
    } else if(status == G_IO_STATUS_ERROR) {
        g_print("%u G_IO_STATUS_ERROR] \n", self);
    } else if(status == G_IO_STATUS_EOF) {
        g_print("%u G_IO_STATUS_EOF] \n", self);
    } else if(status == G_IO_STATUS_AGAIN) {
        g_print("%u G_IO_STATUS_AGAIN] \n", self);
    }
    return TRUE; // FALSE : once, TRUE : infinite
}

static gboolean
cb_test (GIOChannel *source, GIOCondition cond,
    gpointer data) {

    g_print("cb_test start\n");
    
    return TRUE;
}
int main(int argc, char *argv[]){
    pthread_t self = pthread_self();
    g_print("%u]main loop start\n", self);

    GMainLoop * gloop = NULL;
    GIOChannel * io_stdin = NULL;

    gloop = g_main_loop_new(NULL, FALSE);
    io_stdin = g_io_channel_unix_new(fileno(stdin));

    g_io_add_watch(io_stdin, G_IO_IN, cb_stdin, NULL);
    guint source = g_io_add_watch(io_stdin, G_IO_IN, cb_test, NULL);


    g_main_loop_run (gloop);
    g_io_channel_unref (io_stdin);
    return 0;
}