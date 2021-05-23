# declaring class

``` c
// point.h
#include <glib-object.h>

typedef struct _Point Point;
typedef struct _PointClass PointClass;

struct _Point{
    GObject parent_instance;
    gint a;
};

struct _PointClass {
    GObjectClass parent_class;
    void (*print) (Point * self);
};
// point.c
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

```

same with this code
``` c++
class Point{
public:
    Point() {
        a = 100;
    }
    gint a;
    void print() { 
        //proc a;
    }
};
```