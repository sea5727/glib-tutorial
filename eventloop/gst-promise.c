#include <glib.h>
#include <gst/gst.h>


#define N_QUEUES 3

struct event_queue
{
    GMutex lock;
    GCond cond;
//   GThread *thread;
    GMainContext *main_context;
    GMainLoop *main_loop;
    gpointer user_data;
};


struct stress_queues {
    GAsyncQueue *push_queue;
    GAsyncQueue *wait_queue;
    guint64 push_count;
};

struct stress_item
{
    struct event_queue *q;
  GstPromise *promise;
  GstPromiseResult result;
};



gpointer 
promise_thread(gpointer data){
    pthread_t self = pthread_self();
    g_print("%lu] %s start\n", self, __func__);
    struct event_queue * q = data;
    
    q->main_context = g_main_context_new();
    q->main_loop = g_main_loop_new(q->main_context, FALSE);

    g_cond_broadcast(&q->cond);
    g_main_loop_run(q->main_loop);

    g_main_loop_unref(q->main_loop);
    g_main_context_unref(q->main_context);
    
    g_print("%lu] %s end??\n", self, __func__);

    return NULL;
}

static struct event_queue *
event_queue_new(void){
    pthread_t self = pthread_self();
    g_print("%lu] %s start\n", self, __func__);


    struct event_queue * q = g_new0(struct event_queue, 1);
    g_cond_init(&q->cond);
    g_mutex_init(&q->lock);
    g_mutex_lock(&q->lock);
    GThread * th_promise = g_thread_new("promise-thread", promise_thread, q);

    while(q->main_context == NULL){
        g_cond_wait(&q->cond, &q->lock);
    }
    
    g_mutex_unlock(&q->lock);

    g_print("%lu] %s end??\n", self, __func__);

    return q;
}

static void
enqueue_task(const char * name, struct event_queue *q, GSourceFunc func, gpointer data, GDestroyNotify notify){
    GSource *source;
    pthread_t self = pthread_self();
    struct stress_queues *s_q = q->user_data;

    source = g_idle_source_new ();
    g_source_set_priority (source, G_PRIORITY_DEFAULT);
    g_source_set_callback (source, (GSourceFunc) func, data, notify);
    g_source_attach (source, q->main_context);
    g_source_unref (source);
}

static gboolean
_push_random_promise (struct event_queue * q){
    pthread_t self = pthread_self();
    struct stress_queues *s_q = q->user_data;
    struct stress_item *item;

    item = g_new0 (struct stress_item, 1);
    item->promise = gst_promise_new ();
    
    while (item->result == GST_PROMISE_RESULT_PENDING){
        item->result = g_random_int () % 4;
    }

    g_print("push s_q->wait_queue:%p, item:%p, promise:%p \n", s_q->wait_queue, item, item->promise);
    g_async_queue_push (s_q->wait_queue, item);
    g_async_queue_push (s_q->push_queue, item);

    s_q->push_count++;

    g_print("%lu] _push_random_promise G_SOURCE_CONTINUE\n", self);
    return FALSE;
}



static gboolean
_wait_promise (struct event_queue *q) {
    
    pthread_t self = pthread_self();
    g_print("%lu]_wait_promise start \n", self);
    
    struct stress_queues *s_q = q->user_data;
    struct stress_item *item;
    struct stress_item *item2;
    struct stress_item *item3;

    item = g_async_queue_pop (s_q->wait_queue);
    
    if (item == (void *) 1){
        return G_SOURCE_REMOVE;
    }
        
    GstPromiseResult ret = gst_promise_wait (item->promise);
    g_print("%lu]gst_promise_wait?%d\n", self, ret);
    g_assert_true (ret == item->result);

    gst_promise_unref (item->promise);
    g_free (item);

    return FALSE;
}

static void
stress_reply (struct stress_item *item)
{
    pthread_t self = pthread_self();
    
    switch (item->result) {
        case GST_PROMISE_RESULT_INTERRUPTED:
            g_print("%lu]gst_promise_interrupt start \n", self);
            gst_promise_interrupt (item->promise);
            break;
        case GST_PROMISE_RESULT_REPLIED:
            g_print("%lu]gst_promise_reply start \n", self);
            gst_promise_reply (item->promise, NULL);
            break;
        case GST_PROMISE_RESULT_EXPIRED:
            g_print("%lu]gst_promise_expire start \n", self);
            gst_promise_expire (item->promise);
            break;
        default:
            g_assert_not_reached ();
    }
}

static gboolean
_pop_promise (struct event_queue *q)
{
    
    pthread_t self = pthread_self();
    
    
    struct stress_queues *s_q = q->user_data;
    struct stress_item *item;

    g_print("%lu]_pop_promise start \n", self);

    item = g_async_queue_pop (s_q->push_queue);

    if (item == (void *) 1)
        return G_SOURCE_REMOVE;

    stress_reply (item);

    return FALSE;
}


gboolean 
test_func(gpointer data){
    pthread_t self = pthread_self();
    g_print("%lu] test_func start\n", self);
    return TRUE;
}

gpointer 
worker_thread(gpointer data){
    pthread_t self = pthread_self();
    g_print("%lu] %s start\n", self, __func__);
    struct stress_queues q = {0, };

    struct event_queue *pushers[N_QUEUES];
    struct event_queue *poppers[N_QUEUES];
    struct event_queue *waiters[N_QUEUES];

    q.push_queue = g_async_queue_new();
    q.wait_queue = g_async_queue_new();

    for(int i = 0 ; i < N_QUEUES; i ++){
        pushers[i] = event_queue_new();
        pushers[i]->user_data = &q;
        enqueue_task ("pusher", pushers[i], (GSourceFunc) _push_random_promise, pushers[i], NULL);
        poppers[i] = event_queue_new();
        poppers[i]->user_data = &q;
        enqueue_task ("poppers", poppers[i], (GSourceFunc) _wait_promise, poppers[i], NULL);
        waiters[i] = event_queue_new();
        waiters[i]->user_data = &q;
        enqueue_task ("waiters", waiters[i], (GSourceFunc) _pop_promise, waiters[i], NULL);
        g_usleep(1000 * 1000);
        break;
    }

    return NULL;
}


int 
main(int argc, char * argv[]){
    pthread_t self = pthread_self();
    g_print("%lu] %s start\n", self, __func__);

    // GMainContext * ctx = g_main_context_new();
    GMainLoop * loop = g_main_loop_new(NULL, FALSE);

    GThread * th_worker = g_thread_new("worker", worker_thread, NULL);

    g_main_loop_run(loop);

    g_thread_join(th_worker);
    g_main_loop_unref(loop);
    // g_main_context_unref(ctx);
    

    return 0;
}