
#include <glib.h>
#include <gio/gio.h>


gboolean
TestFuncion(gpointer data){
    g_print("TestFunction\n");
    return FALSE;
}

gboolean
TestCallback(gpointer data){
    g_print("TestCallback\n");
    return FALSE;
}
void
DestroyNoti(gpointer data){
    g_print("DestroyNoti\n");
    return;
}
int 
main(int argc, char* argv[]) {

    

    GMainLoop * mainloop = g_main_loop_new(NULL, FALSE);
    g_print("mainloop:%p\n", mainloop);

    GMainLoop * mainloop2 = g_main_loop_new(NULL, FALSE);
    g_print("mainloop2:%p\n", mainloop2);


    gboolean isrunning = g_main_loop_is_running(mainloop);
    g_print("isrunning:%d\n", isrunning);

    GMainContext * context1 = g_main_loop_get_context(mainloop);
    g_print("context1:%p\n", context1);

    GMainContext * context2 = g_main_loop_get_context(mainloop);
    g_print("context2:%p\n", context2);

    GMainContext * context3 = g_main_context_default ();
    g_print("context3:%p\n", context3);

    GSource * src = g_idle_source_new();
    
    g_source_set_callback(src, TestCallback, NULL, DestroyNoti);
    guint ret = g_source_attach(src, context1);
    
    g_print("src:%p, ret:%d\n", src, ret);

    g_idle_add(TestFuncion, NULL);

    g_main_loop_run(mainloop);

    return 0;
}
