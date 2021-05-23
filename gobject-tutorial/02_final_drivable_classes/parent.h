#pragma once

#include <glib-object.h>

typedef struct _TutorialParent TutorialParent;
typedef struct _TutorialParentClass TutorialParentClass;

#define TUTORIAL_TYPE_PARENT tutorial_parent_get_type()
G_DECLARE_DERIVABLE_TYPE (TutorialParent, tutorial_parent, TUTORIAL, PARENT, GObject)

typedef struct _TutorialParentClass {
    GObjectClass parent_class;  
};

GType tutorial_parent_get_type(void);