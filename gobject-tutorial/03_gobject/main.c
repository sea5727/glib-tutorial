#include <glib.h>
#include "tutorial-media.h"

void cb(TutorialMedia * self){
    g_print("g_signal_connect\n");
}
int main(int argc, char * argv[]){

    // g_object_new()

    TutorialMedia * media;
    media = g_object_new(TUTORIAL_TYPE_MEDIA, NULL);

    g_signal_connect(media, "set_prop", (GCallback) cb, NULL);
    
    return 0;
}
