#pragma once

#include <glib-object.h>
#include "parent.h"

G_BEGIN_DECLS

#define TUTORIAL_CHILD_TYPE tutorial_child_get_type ()
G_DECLARE_FINAL_TYPE (TutorialChild, tutorial_child, TUTORIAL, CHILD, TutorialParent)

struct _TutorialChild {
    TutorialParent parent;
};

struct _TutorialChildClass {
    TutorialParentClass parent_class;
};


G_END_DECLS