#pragma once

#include "pebble.h"

static const GPathInfo MINUTE_HAND_POINTS = {
  4,
  (GPoint []) {
    {2, -10 },
    { -2, -10 },
    { -2, -62 },
    { 2, -62 }
  }
};


static const GPathInfo HOUR_HAND_POINTS = {
  4, (GPoint []){
    { 3, -10},
    {-3, -10},
    {-3, -38}, 
    {3, -38}
  }
};

