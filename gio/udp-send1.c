
#include <stdio.h>
#include <glib.h>
#include <gio/gio.h>

gboolean
test_callback(gpointer data){

    GIOChannel * channel = (GIOChannel *)data;

    const gchar * encoding = g_io_channel_get_encoding(channel);
    g_print("test_callback]channel encoding:%s\n", encoding);

    gchar buf[1024];
    gsize bytes_read;
    GError *error = NULL;
    gchar * str_return = NULL;
    
    
    // GIOStatus status = g_io_channel_read_to_end(channel, &buf, &bytes_read, &error);
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


gboolean TestCallback(gpointer data){
    pthread_t self = pthread_self();
    g_print("TestCallback : %u\n", self);

    gchar buffer[1024] = {0,};
    gsize len = g_strlcpy(buffer, "testdata1\ntestdata2", sizeof(buffer));
    g_strlcpy(buffer + len + 1, "testdata3\ntestdata4", sizeof(buffer) - len - 1);

    g_print("buffer:%s\n", buffer);
    
    GError * error = NULL;
    GSocket * socket = (GSocket *)data;
    GSocketAddress * gsockAddr = 
        G_SOCKET_ADDRESS(g_inet_socket_address_new(g_inet_address_new_from_string("192.168.0.34"),  3000));
    GCancellable * cancellable = g_cancellable_new();
	gsize size = g_socket_send_to (socket, gsockAddr,
		       buffer, 30,
		       cancellable, &error);
    g_print("size:%u\n", size);
    return FALSE;
}

void *thread(void *data){
    pthread_t self = pthread_self();
    g_print("thread : %u\n", self);

    GSocket * udp1 = MakeUdp(5000);


    GMainContext * ctx = g_main_context_new();
    GMainLoop * loop = g_main_loop_new(ctx, FALSE);
    
    GSource * timeoutsrc = g_timeout_source_new(1000);
    g_source_set_callback(timeoutsrc, TestCallback, udp1, NULL);
    guint ret = g_source_attach(timeoutsrc, ctx);

    g_print("thread run start: %u\n", self);
    g_main_loop_run(loop);
    g_print("thread run end: %u\n", self);
    return NULL;    
}


int main(int argc, char *argv[]) {
    pthread_t self = pthread_self();
    g_print("main : %u\n", self);

    GError *error = NULL;
    GThread *t;

    t = g_thread_new("test", thread, NULL);

    GMainLoop *loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(loop);

    g_thread_join(t);
	return 0;
}