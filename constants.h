#pragma once

#ifndef TPS_RELOC_VERSION
    #define TPS_RELOC_VERSION "1.16"
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
    FULL_NAME=0,
    LOCATION,
    DEPARTMENT,
    TEAM,
    COMPONENT,
    ROLE,
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

typedef enum class SQLiteOrder {
    SURNAME = 0,
    NAME,
    LOCATION,
    DEPARTMENT,
    TEAM,
    ROLE,
    COMPONENT,
    ROOM
} SQLitePeopleColumns;

