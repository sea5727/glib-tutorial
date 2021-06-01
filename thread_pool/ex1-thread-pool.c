#include <glib.h>
#include <glib/gprintf.h>
#include <unistd.h>
#include <sys/syscall.h>
//#include <pthread.h>
 
 //Used to sort the task list
gint compare_data (gconstpointer a, gconstpointer b, gpointer user_data)
{
	 if(user_data == NULL)//From small to large
	{
		if(*((gint *)a) > *((gint *)b))
			return 1;
		else
			return -1;
	}
	 else//From big to small
	{
		if(*((gint *)a) < *((gint *)b))
			return 1;
		else
			return -1;
	}
}
 
 //The thread executes the function, prints the data after entering, and sleeps for 5 seconds
void thread_execute (gpointer data, gpointer user_data)
{
	g_printf("thread_execute %ld in\n", syscall(__NR_gettid));
	g_printf("thread_execute %ld data is : %d\n", syscall(__NR_gettid), *((gint *)data));
	g_usleep (5000000);
	g_printf("thread_execute %ld out\n", syscall(__NR_gettid));
}
 
gint data[10];
int main(int argc, char **argv)
{
	g_printf ("main in\n");
 
	gint count;
	GError *gerr = NULL;
	GThreadPool *gpool = NULL;
	 //A maximum of 2 threads are executed at the same time. Since the exclusive is set to FALSE, no error will occur, and the error is set to NULL
	gpool = g_thread_pool_new (thread_execute, NULL, 2, FALSE, NULL);
 
	g_printf("thread pool max threads is %d\n", g_thread_pool_get_max_threads (gpool));
	g_printf("thread pool num threads is %d\n", g_thread_pool_get_num_threads (gpool));
	
	 //The maximum number of threads allowed in the thread pool is 3
	if(!g_thread_pool_set_max_threads (gpool, 3, &gerr))
		g_printf("g_thread_pool_set_max_threads is error: %s\n", gerr->message);
 
	g_printf("thread pool max threads is %d\n", g_thread_pool_get_max_threads (gpool));
	g_printf("thread pool num threads is %d\n", g_thread_pool_get_num_threads (gpool));
	
	/** 
	 * There are up to 3 threads running simultaneously in the actual thread pool 
	*/  

	for (count = 0; count < (sizeof(data)/sizeof(gint)); count++)
	{
		data[count] = count;
		g_thread_pool_push(gpool, (gpointer)(&(data[count])), &gerr);
		if(gerr != NULL)
		{
			g_printf("g_thread_pool_push is error: %s\n", gerr->message);
		}
	}

 
	g_usleep (100000);
	g_printf("thread pool num threads is %d\n", g_thread_pool_get_num_threads (gpool));
	g_printf("thread pool unprocessed num is %d\n", g_thread_pool_unprocessed (gpool));
	g_printf("thread pool num unused threads is %d\n", g_thread_pool_get_num_unused_threads ());
	
	 //Insert a new task with high priority
	if(g_thread_pool_move_to_front (gpool, (gpointer)(&(data[9]))))
		g_printf("g_thread_pool_move_to_front is 9\n");
	
	 //Release immediately, do not continue to execute the task queue
	//g_thread_pool_free (gpool, TRUE, TRUE);
	 //Wait for all tasks in the task queue to be completed
	g_thread_pool_free (gpool, FALSE, TRUE);
	g_printf("thread pool num unused threads is %d\n", g_thread_pool_get_num_unused_threads ());
	
	 //Set the maximum idle time 2s
	g_thread_pool_set_max_idle_time (2000);
	
	g_usleep (5000000);
	g_printf("thread pool num unused threads is %d\n", g_thread_pool_get_num_unused_threads ());
	
	
	 //Maximum execution of 1 thread at the same time, because exclusive is set to FALSE, so no error will occur, error is set to NULL
	gpool = g_thread_pool_new (thread_execute, NULL, 1, FALSE, NULL);
	
	/** 
	 * There are up to 3 threads running simultaneously in the actual thread pool 
	*/  
	for (count = 0; count < (sizeof(data)/sizeof(gint)); count++)
	{
		data[count] = count;
		g_thread_pool_push(gpool, (gpointer)(&(data[count])), &gerr);
		if(gerr != NULL)
		{
			g_printf("g_thread_pool_push is error: %s\n", gerr->message);
		}
	}
	
	 	//From small to large 
	//g_thread_pool_set_sort_function (gpool, compare_data, NULL);
	 //From big to small
	g_thread_pool_set_sort_function (gpool, compare_data, (gpointer)(1));
	
	 //Wait for all tasks in the task queue to be completed
	g_thread_pool_free (gpool, FALSE, TRUE);
	//while(1);
	g_printf ("main out\n");
    return 0;

}