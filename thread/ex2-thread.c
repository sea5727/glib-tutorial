#include <glib.h>

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