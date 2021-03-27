
#include <glib.h>
#include <gio/gio.h>


gboolean
TestFuncion(gpointer data){
    g_print("TestFunction\n");
    return FALSE;
}

gboolean
TestCallback(gpointer data){
    pthread_t self = pthread_self();
    g_print("%s.. %u\n", __func__, self);
    sleep(5);

    g_print("TestCallback\n");
    return FALSE;
}
void
DestroyNoti(gpointer data){
    g_print("DestroyNoti\n");
    return;
}
void * thread(void *data){
    pthread_t self = pthread_self();
    g_print("%s.. %u\n", __func__, self);
    GMainContext * ctx = g_main_context_new();
    GMainLoop * loop = g_main_loop_new(ctx, FALSE);
    GSource * src2 = g_idle_source_new();
    g_source_set_callback(src2, TestCallback, NULL, NULL);
    guint ret = g_source_attach(src2, ctx);
    g_idle_add(TestFuncion, NULL);

    g_main_loop_run(loop);

    return NULL;
}

void * thread_2(void *data){
    pthread_t self = pthread_self();
    g_print("%s.. %u\n", __func__, self);
    GMainLoop * loop = g_main_loop_new(NULL, FALSE);
    GMainContext * ctx = g_main_loop_get_context(loop); // 이렇게 선언하면 mainloop에 속해짐.
    GSource * src2 = g_idle_source_new();
    g_source_set_callback(src2, TestCallback, NULL, NULL);
    guint ret = g_source_attach(src2, ctx);
    g_idle_add(TestFuncion, NULL);

    g_main_loop_run(loop);

    return NULL;
}


int 
main(int argc, char* argv[]) {

    pthread_t self = pthread_self();
    g_print("%s.. %u\n", __func__, self);

    // context 가 NULL인경우 main loop, 이다.
    GMainLoop * mainloop = g_main_loop_new(NULL, FALSE);
    GMainContext * context1 = g_main_loop_get_context(mainloop);
    GSource * src = g_idle_source_new();
    g_source_set_callback(src, TestCallback, NULL, DestroyNoti);
    guint ret = g_source_attach(src, context1);
    g_idle_add(TestFuncion, NULL);

    g_thread_new(NULL, thread, NULL);
    g_thread_new(NULL, thread_2, NULL);

    
    

    g_main_loop_run(mainloop);

    return 0;
}
