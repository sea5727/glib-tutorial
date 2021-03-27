
#include <stdio.h>
#include <glib.h>
#include <gio/gio.h>

gboolean
test_callback(gpointer data){

    pthread_t self = pthread_self();
    g_print("%s : %u\n", __func__, self);

    GIOChannel * channel = (GIOChannel *)data;

    const gchar * encoding = g_io_channel_get_encoding(channel);
    g_print("test_callback]channel encoding:%s\n", encoding);

    gchar buf[1024];
    gsize bytes_read;
    GError *error = NULL;
    gchar * str_return = NULL;
    
    
    // ensure_socket_condition (recv_socket, G_IO_IN, cancellable);
    GIOStatus status = g_io_channel_read_chars(channel, buf, sizeof(buf), &bytes_read, &error);

    g_print("status:%d, err:%p\n", status, error);
    if(error) {
        g_warning("Error in serve_input_handler: %s", error->message);
        return FALSE;
    }


    g_print("buf:%s, bytes_read:%u\n", buf, bytes_read);

    // g_print("str_return:%s, length:%u\n", str_return, length);

    if(status == G_IO_STATUS_ERROR){
        
    } else if(status == G_IO_STATUS_NORMAL){
        
    } else if(status == G_IO_STATUS_EOF){
        
    } else if(status == G_IO_STATUS_AGAIN){

    }


    sleep(1);

    return TRUE;
}

GSocket * MakeUdp(guint16 udp_port){
    GSocket * s_udp;
    GError * err;
    int idIdle = -1, dataI = 0;
    GSocketAddress * gsockAddr = 
        G_SOCKET_ADDRESS(g_inet_socket_address_new(g_inet_address_new_any(G_SOCKET_FAMILY_IPV4), 
            udp_port));
    s_udp = g_socket_new(G_SOCKET_FAMILY_IPV4, G_SOCKET_TYPE_DATAGRAM, G_SOCKET_PROTOCOL_UDP, &err);

    if(err != NULL){
        g_print("code:%d, domain:%d, message:%s\n", err->code, err->domain, err->message);
        g_free(err);
        return NULL;
    }
    
    if(g_socket_bind(s_udp, gsockAddr, TRUE, NULL) == FALSE){
        g_print("Error bind\n");
        return NULL;
    }

    return s_udp;
}

GIOChannel *
MakeChannel(int fd, GMainContext * ctx){
    GIOChannel * channel = g_io_channel_unix_new(fd);
    GError *error = NULL;
    const gchar * encoding = g_io_channel_get_encoding(channel);
    g_print("channel encoding:%s\n", encoding);
    g_io_channel_set_encoding(channel, NULL, &error);

    const gchar * now = g_io_channel_get_encoding(channel);
    g_print("channel encoding now :%s, error:%p\n", now, error);
    
    // channel

    GSource * src = g_io_create_watch(channel, G_IO_IN);
    g_source_set_callback(src, test_callback, channel, NULL);
    guint ret = g_source_attach(src, ctx);

    return channel;
}


void *thread(void *data){
    pthread_t self = pthread_self();
    g_print("%s : %u\n", __func__, self);

    GSocket * udp1 = MakeUdp(3000);
    GSocket * udp2 = MakeUdp(3004);

    GMainContext * ctx= g_main_context_new();
    GMainLoop * loop = g_main_loop_new(ctx, FALSE);

    GIOChannel * channel1 = MakeChannel( g_socket_get_fd(udp1), ctx);
    GIOChannel * channel2 = MakeChannel( g_socket_get_fd(udp2), ctx);
    g_main_loop_run(loop);

    g_io_channel_unref(channel1);
    g_io_channel_unref(channel2);
    return NULL;    
}


int main(int argc, char *argv[]) {
    pthread_t self = pthread_self();
    g_print("%s : %u\n", __func__, self);
    GError *error = NULL;
    GThread *t;

    t = g_thread_new("test", thread, NULL);

    GMainLoop *loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(loop);

    g_thread_join(t);
	return 0;
}