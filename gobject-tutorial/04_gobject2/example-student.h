#ifndef __EXAMPLE_STUDENT_H__
#define __EXAMPLE_STUDENT_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define EXAMPLE_TYPE_STUDENT (example_student_get_type())

G_DECLARE_FINAL_TYPE(ExampleStudent, example_student, EXAMPLE, STUDENT, GObject)




G_END_DECLS


#endif