#include <glib.h>
#include "example-student.h"


void cb(ExampleStudent * self){
    char * name = NULL;
    g_object_get(self, "name", &name, NULL);
    g_print("my name in callback is %s\n", name);
}

int main(int argc, char * argv[]){

    ExampleStudent * jane = NULL;

    jane = g_object_new(EXAMPLE_TYPE_STUDENT, NULL);

    g_signal_connect(jane, "changed", (GCallback)cb, NULL);
    g_object_set(jane, "name", "Jane", NULL);

    gchar * name = NULL;
    g_object_get(jane, "name", &name, NULL);

    g_print("My Name is %s\n", name);
    return 0;
}