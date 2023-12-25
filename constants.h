#pragma once

typedef enum class Column {
    DEPARTMENT=0,
    TEAM,
    COMPONENT,
    COLOR,
    REMOVE,
    TOTAL_COLUMNS
} Columns;

typedef enum class MemberColumn {
    NAME=0,
    ROOM,
    TOTAL_COLUMNS
} MemberColumns;


typedef enum class Orientation {
    DOWN = 0,
    LEFT,
    UP,
    RIGHT,
    CENTER
} RoomNrOrientation;

