#include <glib.h>
#include "point.h"

typedef struct _PointPrivate PointPriavte;

G_DEFINE_TYPE (Point, point, G_TYPE_OBJECT);

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
