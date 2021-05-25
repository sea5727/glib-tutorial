#include <glib.h>
#include <gio/gio.h>


typedef struct binary_data{
    int a;
    int b;
    char c[20];
}DATA;

const guint sender_port = 4000;
const guint receiver_port = 3000;



GSocket * 
make_udp(guint16 udp_port){
    GSocket * sock = NULL;
    GError * err = NULL;
    int idIdle = -1, dataI = 0;
    GSocketAddress * gsockAddr = 
        G_SOCKET_ADDRESS(g_inet_socket_address_new(g_inet_address_new_any(G_SOCKET_FAMILY_IPV4), 
            udp_port));
    sock = 
    g_socket_new(
        G_SOCKET_FAMILY_IPV4, 
        G_SOCKET_TYPE_DATAGRAM, 
        G_SOCKET_PROTOCOL_UDP, 
        &err);

    if(err != NULL){
        g_print("code:%d, domain:%d, message:%s\n", err->code, err->domain, err->message);
        g_free(err);
        return NULL;
    }
    
    if(g_socket_bind(sock, gsockAddr, TRUE, NULL) == FALSE){
        g_print("Error bind\n");
        return NULL;
    }

    return sock;
}


GIOChannel *
make_channel(int fd, GMainContext * ctx){
    GError *error = NULL;
    GIOChannel * channel = g_io_channel_unix_new(fd);
    const gchar * encoding = g_io_channel_get_encoding(channel);
    g_io_channel_set_encoding(channel, NULL, &error); // if you want to send a binary data
    
    return channel;
}



gboolean 
recv_callback (gpointer user_data){
    pthread_t self = pthread_self();
    g_print("%s/%lu] recv_callback\n", __func__, self);

    GIOChannel * chan = (GIOChannel *)user_data;
    DATA data;
    gsize len = 0;
    GError * err = NULL;
    gchar buffer[1024] = {0,};
    GIOStatus ret = g_io_channel_read_chars(chan, buffer, sizeof(buffer), &len, &err);
    switch(ret){
        case G_IO_STATUS_ERROR:
            break;
        case G_IO_STATUS_NORMAL:            
            memcpy(&data, buffer, len);
            g_print("%s/%lu] readlen:%lu, a:%d, b:%d, c:%s\n", __func__, self, len, data.a, data.b, data.c);
            break;
        case G_IO_STATUS_EOF:
            break;
        case G_IO_STATUS_AGAIN:
            break;
        default:
            break;
    }

    return TRUE;
}

gboolean 
timeout_callback (gpointer user_data){
    static guint times = 0;
    
    pthread_t self = pthread_self();
    g_print("%s/%lu] timeout_callback -> try send\n", __func__, self);
    GSocket * sock = (GSocket *)user_data;
    GSocketAddress * send_to_addr = 
        G_SOCKET_ADDRESS(g_inet_socket_address_new(g_inet_address_new_from_string("127.0.0.1"), receiver_port));
    GError * err = NULL;

    
    DATA data;
    memset(&data, 0, sizeof(data));
    data.a = 100;
    data.b = 99;
    strcpy(data.c, "Hello World");
    gssize sendlen = g_socket_send_to (sock, send_to_addr, (const gchar * )&data, sizeof(data), NULL, &err);
    g_print("%s/%lu] send len:%ld\n",  __func__, self, sendlen);
    if(sendlen > 0) times += 1;
    if(times == 10){
        g_socket_close(sock, &err);
        if(err){
            g_print("error code:%d, domain:%u, message:%s\n", err->code, err->domain, err->message);
        }
        g_object_unref(sock);
        return FALSE;
    }

    return TRUE;
}

gboolean 
timeout_for_log (gpointer user_data){
    
    pthread_t self = pthread_self();
    g_print("%s/%lu] timeout_callback -> try send\n", __func__, self);
    return TRUE;
}


void * send_thread(void *data){
    pthread_t self = pthread_self();
    g_print("%s/%lu] send_thread start \n", __func__, self);
    
    GMainContext * ctx = g_main_context_new();
    GMainLoop * loop = g_main_loop_new(ctx, FALSE);

    GSocket * sock = make_udp(sender_port);
    GIOChannel * chan = make_channel(g_socket_get_fd(sock), ctx);

    GSource * src = g_io_create_watch(chan, G_IO_IN|G_IO_ERR);
    g_source_set_callback(src, recv_callback, chan, NULL);
    g_source_attach(src, ctx);

    GSource * timeout = g_timeout_source_new(1000);
    g_source_set_callback(timeout, timeout_callback, sock, NULL);
    g_source_attach(timeout, ctx);
    
    g_main_loop_run(loop);
    return NULL;
}

void * recv_thread(void *data){
    pthread_t self = pthread_self();
    g_print("%s/%lu] recv_thread start \n", __func__, self);
    
    GMainContext * ctx = g_main_context_new();
    GMainLoop * loop = g_main_loop_new(ctx, FALSE);

    GSocket * sock = make_udp(receiver_port);
    GIOChannel * chan = make_channel(g_socket_get_fd(sock), ctx);

    GSource * src = g_io_create_watch(chan, G_IO_IN|G_IO_ERR);
    g_source_set_callback(src, recv_callback, chan, NULL);
    g_source_attach(src, ctx);

    // GSource * timeout = g_timeout_source_new(1000);
    // g_source_set_callback(timeout, timeout_for_log, sock, NULL);
    // g_source_attach(timeout, ctx);

    g_main_loop_run(loop);
    return NULL;
}

int main(int argc, char * argv[]){

    pthread_t self = pthread_self();
    g_print("%s/%lu] main start \n", __func__, self);
    GError *error = NULL;
    GThread *sender, *receiver;

    GMainLoop *loop = g_main_loop_new(NULL, FALSE);

    sender = g_thread_new("sender", send_thread, NULL);
    receiver = g_thread_new("receiver", recv_thread, NULL);

    
    g_main_loop_run(loop);

    g_thread_join(sender);
    g_thread_join(receiver);


    return 0;
}