#pragma once

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

