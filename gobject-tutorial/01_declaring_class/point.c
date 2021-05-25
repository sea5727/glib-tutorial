#include <glib.h>
#include "point.h"



struct _PointPrivate{
    gint private_value;
};

G_DEFINE_TYPE_WITH_PRIVATE (Point, point, G_TYPE_OBJECT);



static void point_print(Point * self);

static void
point_class_init(PointClass * klass){
    klass->print = point_print;
    
}

static void
point_init (Point * self){
    self->a = 100;
}

static void
point_print(Point * self){
    //proc self->a; 
}

Point * point_new(void) {
    return g_object_new(TYPE_POINT, NULL);
}

gint
point_get_private_int(Point * self){
    return self->priv->private_value;
}