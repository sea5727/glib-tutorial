#pragma once

#include <glib-object.h>

typedef struct _TutorialMedia TutorialMedia;
typedef struct _TutorialMediaClass TutorialMediaClass;

#define TUTORIAL_TYPE_MEDIA             (tutorial_media_get_type ())
#define TUTORIAL_MEDIA(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj), TUTORIAL_TYPE_MEDIA, TutorialMedia))
#define TUTORIAL_IS_MEDIA(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj), TUTORIAL_TYPE_MEDIA))
#define TUTORIAL_MEDIA_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass), TUTORIAL_TYPE_MEDIA, TutorialMediaClass))
#define TUTORIAL_IS_MEDIA_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass), TUTORIAL_TYPE_MEDIA))
#define TUTORIAL_MEDI_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj), TUTORIAL_TYPE_MEDIA, TutorialMediaClass)))

struct _TutorialMedia {
    GObject parent_instance;
    gchar * media_name;
    guint inv_nr;
    GString * location;
    GString * title;
    gboolean orig_pacakge;
};

struct _TutorialMediaClass{
    GObjectClass parent_class;
    void (*unpacked) (TutorialMedia * media);
    void (*throw_out) (TutorialMedia * media, gboolean permanent);
};



TutorialMedia * media_new ();
void media_print (TutorialMedia * self);
void meep_meep ();


