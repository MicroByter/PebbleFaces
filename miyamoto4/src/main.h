#pragma once

#include "pebble.h"

static const GPathInfo MINUTE_HAND_POINTS = {
  4,
  (GPoint []) {
    { 2, 4 },
    { -2, 4 },
    { -2, -30 },
    { 2, -30 }
  }
};


static const GPathInfo HOUR_HAND_POINTS = {
  4, (GPoint []){
    { 2.5, 4},
    {-2.5, 4},
    {-2.5, -20}, 
    {2.5, -20}
  }
};
/*static const GPathInfo MINUTE_HAND_POINTS = {
  3,
  (GPoint []) {
    { -8, 20 },
    { 8, 20 },
    { 0, -80 }
  }
};


static const GPathInfo HOUR_HAND_POINTS = {
  3, (GPoint []){
    {-6, 20},
    {6, 20},
    {0, -60}
  }
};
*/