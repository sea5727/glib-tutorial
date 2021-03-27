
#include <glib.h>
#include <gio/gio.h>


gboolean
gio_read_socket(
    GIOChannel * channel,
    GIOCondition condition,
    gpointer data) {
    pthread_t self = pthread_self();
    g_print("gio_read_socket self:%d\n", self);

    char buf[1024];
    gsize bytes_read;
    GError *error = NULL;

    if (condition & G_IO_HUP) return FALSE; /* this channel is done */

    g_io_channel_read_chars (channel, buf, sizeof (buf), &bytes_read,
            &error);
    g_assert (error == NULL);

    buf[bytes_read] = '\0';

    g_print ("%s", buf);

    int *a = data;
    *a = *a + 1;

    return TRUE;
}

gboolean
idleCpt (gpointer user_data){

   int *a = user_data;
   g_print("%d\n", *a);
   sleep(1);

   return TRUE;
}

int 
main(int argc, char* argv[]) {
    pthread_t self = pthread_self();
    g_print("self:%d\n", self);
    
    GSocket * s_udp;
    GError * err;
    int idIdle = -1, dataI = 0;
    guint16 udp_port = 3000;
    GSocketAddress * gsockAddr = 
        G_SOCKET_ADDRESS(g_inet_socket_address_new(g_inet_address_new_any(G_SOCKET_FAMILY_IPV4), 
            udp_port));
    s_udp = g_socket_new(G_SOCKET_FAMILY_IPV4, G_SOCKET_TYPE_DATAGRAM, G_SOCKET_PROTOCOL_UDP, &err);

    if(err != NULL){
        g_print("code:%d, domain:%d, message:%s\n", err->code, err->domain, err->message);
        g_free(err);
        return 0;
    }
    
    if(g_socket_bind(s_udp, gsockAddr, TRUE, NULL) == FALSE){
        g_print("Error bind\n");
        return 0;
    }

    int fd = g_socket_get_fd(s_udp);

    GIOChannel * channel = g_io_channel_unix_new(fd);
    guint source = g_io_add_watch(channel, G_IO_IN, gio_read_socket, &dataI);
    g_io_channel_unref(channel);

   GMainLoop *loop = g_main_loop_new(NULL, FALSE);
//    idIdle = g_idle_add(idleCpt, &dataI);
   g_main_loop_run(loop);

    return 0;
}
