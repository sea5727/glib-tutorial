
#include <glib-object.h>
#include <glib.h>

#include "tutorial-media.h"


G_DEFINE_TYPE (TutorialMedia, tutorial_media, G_TYPE_OBJECT);

enum {
    PROP_INV_NR = 1,
    PROP_ORIG_PACKAGE,
    LAST_PROP,
};

enum {
    SIGNAL_UNPACKED = 0,
    SIGNAL_THROW_OUT,
    SIGNAL_SET_PROP,
    SIGNAL_LAST,
};

guint signals[SIGNAL_LAST];

//set property
//get property
//class init
//member init

static void set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);
static void get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
tutorial_media_class_init (TutorialMediaClass * klass) {
    GObjectClass * object_class = G_OBJECT_CLASS(klass);

    object_class->set_property;
    object_class->get_property;

    GParamSpec* inv_nr_param = g_param_spec_uint(
        "inventory-id",
        "inventory number",
        "number on inventory label",
        0,
        UINT_MAX,
        0,
        G_PARAM_READWRITE
    );

    GParamSpec* org_package_param = g_param_spec_boolean(
        "orig-package",
        "original package?",
        "is item in its original package?",
        FALSE,
        G_PARAM_READWRITE
    );

    g_object_class_install_property(
        object_class,
        PROP_INV_NR,
        inv_nr_param);

    g_object_class_install_property(
        object_class,
        PROP_ORIG_PACKAGE,
        org_package_param
    );


    signals[SIGNAL_UNPACKED] = g_signal_new(
        "unpacked",
        G_TYPE_FROM_CLASS(klass),
        G_SIGNAL_RUN_FIRST,
        0,
        NULL,
        NULL,
        NULL,
        G_TYPE_NONE,
        0,
        NULL);

    signals[SIGNAL_THROW_OUT] = g_signal_new(
        "throw_out",
        G_TYPE_FROM_CLASS(klass),
        G_SIGNAL_RUN_FIRST,
        0,
        NULL,
        NULL,
        NULL,
        G_TYPE_NONE,
        0,
        NULL);

    signals[SIGNAL_SET_PROP] = g_signal_new(
        "set_prop",
        G_TYPE_FROM_CLASS(klass),
        G_SIGNAL_RUN_FIRST,
        0,
        NULL,
        NULL,
        NULL,
        G_TYPE_NONE,
        0,
        NULL);

    
    // signals[SIGNAL_UNPACKED] = 
    
    // klass->throw_out;
    // klass->unpacked;

    
}

static void
tutorial_media_init (TutorialMedia * self){

}


static void 
set_property (
    GObject *object, 
    guint property_id, 
    const GValue *value, 
    GParamSpec *pspec) {
    
    TutorialMedia * self = TUTORIAL_MEDIA(object);

    switch(property_id) {
        case PROP_INV_NR :
            self->inv_nr = g_value_get_uint(value);
            break;
        case PROP_ORIG_PACKAGE:
            self->orig_pacakge = g_value_get_boolean(value);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
            break;
    }
    g_signal_emit(self, SIGNAL_SET_PROP, 0);
}


static void 
get_property (
    GObject *object, 
    guint property_id, 
    GValue *value, 
    GParamSpec *pspec) {

    TutorialMedia * self = TUTORIAL_MEDIA(object);

    switch(property_id){
        case PROP_INV_NR:
            g_value_set_uint(value, self->inv_nr);
            break;
        case PROP_ORIG_PACKAGE:
            g_value_set_boolean(value, self->orig_pacakge);
            break;
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}

GType media_get_type (void) {
    static GType type = 0;
    if (type) return type;

    static const GTypeInfo media_info = {
        sizeof (TutorialMediaClass),                /* class structure size */
        NULL,                               /* base class initializer */
        NULL,                               /* base class finalizer */
        (GClassInitFunc) tutorial_media_class_init,  /* class initializer */
        NULL,                               /* class finalizer */
        NULL,                               /* class data */
        sizeof (TutorialMedia),                     /* instance struct size */
        0,                                  /* preallocated instances */
        NULL,                               /* instance initializer */
        NULL                                /* function table */
    };

    type = g_type_register_static(
        G_TYPE_OBJECT, /* parent class */
        "Media",       /* type name */
        &media_info,   /* GtypeInfo struct */
        0);            /* flags */

    // const GInterfaceInfo cleanable_info = {media_cleanable_init, NULL, NULL};
    
    // g_type_add_interface_static(type, TYPE_CLEANABLE, &cleanable_info);

    return type;
}