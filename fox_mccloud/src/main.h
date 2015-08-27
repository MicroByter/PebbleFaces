#pragma once

#include "pebble.h"

static const GPathInfo MINUTE_HAND_POINTS = {
  4,
  (GPoint []) {
    { 2.5, 10 },
    { -2.5, 10 },
    { -2.5, -60 },
    { 2.5, -60 }
  }
};

static const GPathInfo SECOND_HAND_POINTS = {
  4,
  (GPoint []) {
    { 1, 0 },
    { -1, 0 },
    { -1, -67 },
    { 1, -67 }
  }
};

static const GPathInfo SECOND_HAND_POINTS2 = {
  4,
  (GPoint []) {
    { 2, 18 },
    { -2, 18 },
    { -2, 0 },
    { 2, 0 }
  }
};



static const GPathInfo HOUR_HAND_POINTS = {
  4, (GPoint []){
    { 3, 8},
    {-3, 8},
    {-3, -45}, 
    {3, -45}
  }
};
