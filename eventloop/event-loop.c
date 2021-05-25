
#include <glib.h>
#include <gio/gio.h>


// gboolean
// TestFuncion(gpointer data){
//     g_print("TestFunction\n");
//     return FALSE;
// }

gboolean
callback1(gpointer data){
    pthread_t self = pthread_self();
    g_print("%s/%lu] this is callback \n", __func__, self);
    return TRUE;
}
void
callback1_destory(gpointer data){
    pthread_t self = pthread_self();
    g_print("%s/%lu] DESTORY \n", __func__, self);
    return;
}

void * thread_1(void *data){
    pthread_t self = pthread_self();
    g_print("%s/%lu] new thread1 start \n", __func__, self);
    
    
    GMainContext * ctx = g_main_context_new();
    GMainLoop * loop = g_main_loop_new(ctx, FALSE);

    // GSource * src = g_idle_source_new();
    GSource * src = g_timeout_source_new(1000);
    g_source_set_callback(src, callback1, NULL, NULL);
    guint ret = g_source_attach(src, ctx);

    g_main_loop_run(loop);

    return NULL;
}

void * thread_2(void *data){
    pthread_t self = pthread_self();
    g_print("%s/%lu] new thread2 start \n", __func__, self);

    
    GMainContext * ctx = g_main_context_new();
    GMainLoop * loop = g_main_loop_new(ctx, FALSE);
    // GMainContext * ctx = g_main_loop_get_context(loop); 

    // GSource * src = g_idle_source_new();
    GSource * src = g_timeout_source_new(1000);
    g_source_set_callback(src, callback1, NULL, NULL);
    guint ret = g_source_attach(src, ctx);

    g_main_loop_run(loop);

    return NULL;
}


int 
main(int argc, char* argv[]) {

    pthread_t self = pthread_self();
    g_print("%s/%lu] main start \n", __func__, self);

    // context 가 NULL인경우 main loop, 이다.
    GMainLoop * mainloop = g_main_loop_new(NULL, FALSE);
    GMainContext * context1 = g_main_loop_get_context(mainloop);
    // GSource * src = g_idle_source_new();
    GSource * src = g_timeout_source_new(1000);
    g_source_set_callback(src, callback1, NULL, callback1_destory);
    guint ret = g_source_attach(src, context1);

    // g_idle_add(TestFuncion, NULL);
    g_thread_new(NULL, thread_1, NULL);
    g_thread_new(NULL, thread_2, NULL);

    g_main_loop_run(mainloop);

    return 0;
}
