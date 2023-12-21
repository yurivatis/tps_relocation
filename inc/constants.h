#pragma once

typedef enum class Column {
    DEPARTMENT=0,
    TEAM,
    COMPONENT,
    COLOR,
    REMOVE,
    TOTAL_COLUMNS
} Columns;

typedef enum class Orientation {
    DOWN = 0,
    LEFT,
    UP,
    RIGHT,
    CENTER
} RoomOrientation;
