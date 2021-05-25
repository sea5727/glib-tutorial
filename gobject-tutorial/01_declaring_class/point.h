#pragma once

#include <glib-object.h>


typedef struct _Point Point;
typedef struct _PointClass PointClass;
typedef struct _PointPrivate PointPrivate;

#define TYPE_POINT	(point_get_type())


struct _Point{
    GObject parent_instance;
    gint a;
    PointPrivate * priv;
};

struct _PointClass {
    GObjectClass parent_class;
    void (*print) (Point * self);
};


Point * point_new(void);