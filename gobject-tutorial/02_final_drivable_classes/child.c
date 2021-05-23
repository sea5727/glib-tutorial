
#include "child.h"


G_DEFINE_TYPE (TutorialChild, tutorial_child, TUTORIAL_TYPE_PARENT)

//class init
//member init

static void
tutorial_child_class_init(TutorialChildClass * klass){
    TutorialParentClass * parent_class = TUTORIAL_PARENT_CLASS(klass);
}

static void
tutorial_child_init(TutorialChild * self){
    TutorialParent * parent = TUTORIAL_PARENT(self);
}

