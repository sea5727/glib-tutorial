
#include <glib.h>
#include <gio/gio.h>


gboolean
gio_read_socket(
    GIOChannel * channel,
    GIOCondition condition,
    gpointer data){
    pthread_t self = pthread_self();
    g_print("%s/%lu] gio_read_socket condition:%d\n", __func__, self, condition);


    char buf[1024];
    gsize bytes_read = 0;
    GError *error = NULL;

    if (condition & G_IO_HUP) {
        return FALSE; /* this channel is done */
    }

    g_io_channel_read_chars (channel, buf, sizeof (buf), &bytes_read, &error);

    g_assert (error == NULL);

    g_print ("%s", buf);

    return TRUE;

}

gboolean
gio_accept_socket(
    GIOChannel * channel,
    GIOCondition condition,
    gpointer data) {
    pthread_t self = pthread_self();
    g_print("%s/%lu] gio_accept_socket condition:%d\n", __func__, self, condition);

    GSocket * sock = (GSocket *)data;
    GError * err = NULL;

    GSocket * client_sock = g_socket_accept(sock, NULL, &err);
    int client_fd = g_socket_get_fd(client_sock);
    g_print("%s/%lu] client fd:%d\n", __func__, self, client_fd);


    GIOChannel * client_chan = g_io_channel_unix_new(client_fd);
    g_io_channel_set_encoding(client_chan, NULL, NULL);
    guint source = g_io_add_watch(client_chan, G_IO_IN|G_IO_ERR, gio_read_socket, sock);
    g_io_channel_unref(client_chan);



    return TRUE;
}

gboolean
gio_interval_timer (gpointer user_data){
    pthread_t self = pthread_self();
    int *a = user_data;
    g_print("gio_interval_timer self:%lu, data:%d\n", self, *a);

   return TRUE;
}


gboolean
gio_just_one_proc_immediately (gpointer user_data){
    pthread_t self = pthread_self();
    int *a = user_data;
    g_print("gio_just_one_proc_immediately self:%lu, data:%d\n", self, *a);

    return FALSE;  // end

    //return TRUE; // retry
}


int 
main(int argc, char* argv[]) {
    pthread_t self = pthread_self();
    g_print("main self:%lu\n", self);

    GSocketService * service = g_threaded_socket_service_new(10);
    GSocketAddress * server_addr = g_inet_socket_address_new(g_inet_address_new_any(G_SOCKET_FAMILY_IPV4), 3000);

    
    if(g_socket_listener_add_address(G_SOCKET_LISTENER(service), server_addr, G_SOCKET_TYPE_STREAM, G_SOCKET_PROTOCOL_TCP, NULL, NULL, NULL) == FALSE){
        return 0;
    }

    
    
    
    GSocket * sock = NULL;
    GError * err = NULL;
    int idIdle = -1, dataI = 0;
    guint16 tcp_server_port = 3000;
    GSocketAddress * gsockAddr = 
        G_SOCKET_ADDRESS(g_inet_socket_address_new(g_inet_address_new_any(G_SOCKET_FAMILY_IPV4), 
            tcp_server_port));
    sock = g_socket_new(G_SOCKET_FAMILY_IPV4, G_SOCKET_TYPE_STREAM, G_SOCKET_PROTOCOL_TCP, &err);

    if(err != NULL){
        g_print("code:%d, domain:%u, message:%s\n", err->code, err->domain, err->message);
        g_error_free(err);
        return 0;
    }
    
    if(g_socket_bind(sock, gsockAddr, TRUE, &err) == FALSE){
        g_print("g_socket_bind error code:%d, domain:%u, message:%s\n", err->code, err->domain, err->message);
        g_error_free(err);
        return 0;
    }
    
    g_object_unref(gsockAddr);

    if(g_socket_listen(sock, &err) == FALSE){
        g_print("g_socket_bind error code:%d, domain:%u, message:%s\n", err->code, err->domain, err->message);
        g_error_free(err);
        return 0;
    }



    int fd = g_socket_get_fd(sock);

    GIOChannel * channel = g_io_channel_unix_new(fd);
    guint source = g_io_add_watch(channel, G_IO_IN|G_IO_ERR, gio_accept_socket, sock);
    g_io_channel_unref(channel);

   GMainLoop *loop = g_main_loop_new(NULL, FALSE);
   idIdle = g_idle_add(gio_just_one_proc_immediately, &dataI);
   idIdle = g_timeout_add(1000, gio_interval_timer, &dataI);
   g_main_loop_run(loop);

    return 0;
}
