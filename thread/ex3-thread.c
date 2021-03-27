#include <stdio.h>
#include <glib.h>
#include <gio/gio.h>

static gboolean
cb_test (GIOChannel *source, GIOCondition cond,
    gpointer data) {

    g_print("cb_test start\n");
    
    return TRUE;
}

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


void notify(gpointer data) {
    pthread_t self = pthread_self();
    g_print("notify:%u\n", self);

    g_thread_self();
    g_main_loop_quit((GMainLoop *)data);
}

gboolean func(gpointer data) {
    pthread_t self = pthread_self();
    g_print("func:%u\n", self);

    static gint i = 0;
    g_message("%d", i++);
    return (i < 10) ? TRUE : FALSE;
}

gpointer thread(gpointer data) {
    pthread_t self = pthread_self();
    g_print("thread:%u\n", self);

    GMainContext *c;
    GMainContext *d;
    GMainLoop *l;
    GSource *s;

    c = g_main_context_new();
    d = g_main_context_default();

    g_message("local: %p", c);
    g_message("default: %p", d);

#if 1
    l = g_main_loop_new(c, FALSE);
    s = g_timeout_source_new(100);
    g_source_set_callback(s, func, l, notify);
    g_source_attach(s, c);
    g_source_unref(s);
#else
    l = g_main_loop_new(d, FALSE);
    g_timeout_add_full(G_PRIORITY_DEFAULT, 100, func, l, notify);
#endif

    GIOChannel * io_stdin = g_io_channel_unix_new(fileno(stdin));
    g_io_add_watch(io_stdin, G_IO_IN, cb_stdin, NULL);
    guint source = g_io_add_watch(io_stdin, G_IO_IN, cb_test, NULL);

    g_main_loop_run(l);
    g_message("done");

    return NULL;
}

int main(int argc, char *argv[]) {

    pthread_t self = pthread_self();
    g_print("main:%u\n", self);

    GError *error = NULL;
    GThread *t;

    t = g_thread_new("test", thread, NULL);

   GMainLoop *loop = g_main_loop_new(NULL, FALSE);
   g_main_loop_run(loop);

   g_thread_join(t);

    return 0;
}