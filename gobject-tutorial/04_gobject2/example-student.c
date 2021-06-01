#include "example-student.h"


struct _ExampleStudent{
    GObject parent;
    char * name;
    int id;
};

enum {
    PROP_0 = 0,
    PROP_NAME,
    PROP_ID,
    LAST_PROP,
};

enum {
    SIGNAL_NAME_CHANGED = 0,
    LAST_SIGNAL,
};

guint signals[LAST_SIGNAL];

static GParamSpec * properties[LAST_PROP];


G_DEFINE_TYPE(ExampleStudent, example_student, G_TYPE_OBJECT)



static void
set_property(
    GObject        *object,
    guint           property_id,
    const GValue   *value,
    GParamSpec     *pspec){
    ExampleStudent * self = EXAMPLE_STUDENT(object);

    switch(property_id){
        case PROP_NAME:
            if(g_strcmp0(self->name, g_value_get_string(value)) != 0) {
                g_free(self->name);
                self->name = g_strdup(g_value_get_string(value));
                g_print("g_signal_emit start???\n");
                g_signal_emit(self, signals[SIGNAL_NAME_CHANGED], 0);
                g_print("g_signal_emit end???\n");
            }
            break;
        case PROP_ID:
            self->id = g_value_get_int(value);
            break;
        default:
            break;
    }
}

static void
get_property(
    GObject        *object,
    guint           property_id,
    GValue         *value,
    GParamSpec     *pspec) {

    ExampleStudent * self = EXAMPLE_STUDENT(object);

    switch(property_id){
        case PROP_NAME:
            g_value_set_string(value, self->name);
            break;
        case PROP_ID:
            g_value_set_int(value, self->id);
        default:
            break;
    }

}

static void
example_student_init(ExampleStudent * self){
    g_print("example student init is called\n");
}

static void
example_student_class_init(ExampleStudentClass * klass){

    GObjectClass * objClass = G_OBJECT_CLASS(klass);

    objClass->set_property = set_property;
    objClass->get_property = get_property;

    properties[PROP_NAME] = 
    g_param_spec_string(
        "name",
        "name",
        "student name",
        "Mr. A",
        G_PARAM_READWRITE);
    properties[PROP_ID] = 
    g_param_spec_int(
        "id",
        "id",
        "student id",
        0, 
        10000,
        0,
        G_PARAM_READWRITE);

    g_object_class_install_properties(objClass, LAST_PROP, properties);

    signals[SIGNAL_NAME_CHANGED] = 
    g_signal_new(
        "changed",
        G_TYPE_FROM_CLASS(klass),
        G_SIGNAL_RUN_FIRST,
        0, 
        NULL,
        NULL,
        NULL,
        G_TYPE_NONE,
        0,
        NULL);
}