#pragma once

#ifndef TPS_RELOC_MAJOR
    #define TPS_RELOC_MAJOR 2
#endif
#ifndef TPS_RELOC_MINOR
    #define TPS_RELOC_MINOR 1
#endif

#ifndef CSV_SEPARATOR
    #define CSV_SEPARATOR ";"
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

typedef enum class RoomColumn {
    NUMBER=0,
    CAPACITY,
    TOTAL_COLUMNS
} RoomColumns;

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
    ROOM,
    TOTAL_COLUMNS
} SQLitePeopleColumns;

