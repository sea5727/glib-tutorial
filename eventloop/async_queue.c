#include <glib.h>

#define PUSH_POP_COUNT 5000

typedef struct _MyData MyData;
typedef struct _MyQueues MyQueues;
struct _MyData {
    int a;
    int b;
};

struct _MyQueues{
    GAsyncQueue * push_queue;
    GAsyncQueue * wait_queue;
};

gint count1 = 0;

gpointer push_thread1(gpointer data){
    pthread_t self = pthread_self();
    g_print("%lu] %s start\n", self, __func__);
    
    MyQueues * q = (MyQueues *)data;
    while(count1 != PUSH_POP_COUNT){
        gpointer push_data = g_new0(MyData, 1);
        if(push_data == NULL) g_print("new data is NULL\n");
        g_async_queue_push(q->push_queue, push_data);
        count1 += 1;
    }

    gint len = g_async_queue_length(q->push_queue);
    g_print("%lu] %s end.. len:%d\n", self, __func__, len);
    

    return NULL;
}

gint count2 = 0;
gpointer push_thread2(gpointer data){
    pthread_t self = pthread_self();
    g_print("%lu] %s start\n", self, __func__);

    MyQueues * q = (MyQueues *)data;
    while(count2 != PUSH_POP_COUNT){
        gpointer push_data = g_new0(MyData, 1);
        if(push_data == NULL) g_print("new data is NULL\n");
        g_async_queue_push(q->push_queue, push_data);
        count2 += 1;
    }

    gint len = g_async_queue_length(q->push_queue);
    g_print("%lu] %s end.. len:%d\n", self, __func__, len);
    

    return NULL;
}

gint last_pop_count = 0;


gpointer pop_thread1(gpointer data){
    pthread_t self = pthread_self();
    g_print("%lu] %s start\n", self, __func__);
    
    MyQueues * q = (MyQueues *)data;
    gint pop_count = 0;
    gpointer pop_data = NULL;
    g_usleep(100);
    
    while(1) {
        gpointer pop_data = g_async_queue_try_pop(q->push_queue);
        if(pop_data == NULL) {
            gint cnt = g_atomic_int_get(&last_pop_count);
            if(cnt == PUSH_POP_COUNT + PUSH_POP_COUNT){
                break;
            }
        } else {
            g_atomic_int_inc (&last_pop_count);
        }
    }
    

    gint len = g_async_queue_length(q->push_queue);
    g_print("%lu] %s end.. pop_count:%d, len:%d\n", self, __func__, pop_count, len);
    

    return NULL;
}
gpointer pop_thread2(gpointer data){
    pthread_t self = pthread_self();
    g_print("%lu] %s start\n", self, __func__);
    
    MyQueues * q = (MyQueues *)data;
    gint pop_count = 0;
    gpointer pop_data = NULL;
    g_usleep(100);
    
    while(1) {
        gpointer pop_data = g_async_queue_try_pop(q->push_queue);
        if(pop_data == NULL) {
            gint cnt = g_atomic_int_get(&last_pop_count);
            if(cnt == PUSH_POP_COUNT + PUSH_POP_COUNT){
                break;
            }
        } else {
            g_atomic_int_inc (&last_pop_count);
        }
    }
    gint len = g_async_queue_length(q->push_queue);
    g_print("%lu] %s end.. pop_count:%d, len:%d\n", self, __func__, pop_count, len);
    

    return NULL;
}


int main(int argc, char * argv[]){

    gpointer data = g_new0(MyData, 1);
    MyQueues q = { 0, };
    q.push_queue = g_async_queue_new();
    q.wait_queue = g_async_queue_new();

    GMainContext * ctx = g_main_context_new();
    GMainLoop * loop = g_main_loop_new(ctx, FALSE);

    GThread * th1 = g_thread_new("push_thread", push_thread1, &q);
    GThread * th2 = g_thread_new("push_thread", push_thread2, &q);
    GThread * th3 = g_thread_new("pop_thread", pop_thread1, &q);
    GThread * th4 = g_thread_new("pop_thread", pop_thread2, &q);

    g_main_loop_run(loop);

    g_thread_join(th1);
    g_thread_join(th2);
    g_thread_join(th3);
    g_thread_join(th4);

    return 0;
}
