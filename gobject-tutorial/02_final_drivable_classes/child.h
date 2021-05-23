#pragma once

#include "parent.h"

typedef struct _TutorialChild TutorialChild;
typedef struct _TutorialChildClass TutorialChildClass;

#define TUTORIAL_CHILD_TYPE tutorial_child_get_type ()
G_DECLARE_FINAL_TYPE (TutorialChild, tutorial_chgild, TUTORIAL, CHILD, TutorialParent)

struct _TutorialChild {
    TutorialParent parent;
};

struct _TutorialChildClass {
    TutorialParentClass parent_class;
};

