#include <glib.h>

void *thread_f(void *data){
    return NULL;
}
int main(int argc, char *argv[]){

    pthread_t self = pthread_self();
    g_print("main:%u\n", self);

    GThread *thread;
    GError *err;

    thread = g_thread_new("name", thread_f, NULL);

    

    g_thread_join(thread);


    return 0;
}