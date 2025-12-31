#ifndef SPRITE_STATE_H_
#define SPRITE_STATE_H_

#include <tuple>


#define ENUM_FOREACH(var, enum_type, enum_class) \
    for (int var{static_cast<int>(enum_class::FIRST_##enum_type)}; \
        var < static_cast<int>(enum_class::LAST_##enum_type); ++var)

enum class MotionType {
    FIRST_MOTION_TYPE,
    STANDING = FIRST_MOTION_TYPE,
    INTERACTING,
    WALKING,
    JUMPING,
    FALLING,
    LAST_MOTION_TYPE
};

enum class HorizontalFacing {
    FIRST_HORIZONTAL_TYPE,
    LEFT = FIRST_HORIZONTAL_TYPE,
    RIGHT,
    LAST_HORIZONTAL_TYPE
};

enum class VerticalFacing{
    FIRST_VERTICAL_TYPE,
    UP = FIRST_VERTICAL_TYPE,
    DOWN,
    HORIZONTAL,
    LAST_VERTICAL_TYPE
};

#endif