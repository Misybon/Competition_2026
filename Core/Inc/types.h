#pragma once

typedef enum
{
    STBY = 0,
    TRACK = 1,
    CORNER = 2,
    THROW_PREPARE = 3,
    THROW_WAIT = 4,
    STOP_PREPARE = 5,
} TRACK_STATUS;

struct Track_Speed
{
    int32_t vx;
    int32_t vy;
    int32_t vz;
};
