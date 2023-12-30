#pragma once

#ifndef TPS_RELOC_VERSION
    #define TPS_RELOC_VERSION "1.10"
#endif

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
