#include <unistd.h>
#include <glib.h>
#include <stdio.h>

int fd[2];

void *pipe_send_thread(void *data) {
	// call sleep so that the main loop source is not ready immediately
    g_print("pipe_send_thread\n");
	sleep(5);
	
	write(fd[1],"GIOChannel main loop example",29);
	return NULL;
}

gboolean my_callback(
    GIOChannel *source,
    GIOCondition condition,
    gpointer data) {

    g_print("main loop callback: ");

	char buf[100];	
	
	read(fd[0],buf,sizeof(buf));
	g_print("%s",buf);
	
	g_main_loop_quit((GMainLoop *)data);
	
	return FALSE;
}

int main() {
	GMainLoop *loop;
	GIOChannel *channel;
	
	pipe(fd);

    g_thread_new("pipe", pipe_send_thread, NULL);
	
	channel = g_io_channel_unix_new(fd[0]);
	loop = g_main_loop_new(NULL,FALSE);

	g_io_add_watch(channel, G_IO_IN | G_IO_HUP | G_IO_ERR,(GIOFunc)my_callback,loop);

	g_main_loop_run(loop);
	
	g_io_channel_shutdown(channel,TRUE,NULL);

	return 0;
}