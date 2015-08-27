#pragma once

#include "pebble.h"

static const GPathInfo MINUTE_HAND_POINTS = {
  4,
  (GPoint []) {
    { 3, -38 },
    { -3, -38 },
    { -3, -52 },
    { 3, -52 }
  }
};

static const GPathInfo SECOND_HAND_POINTS = {
  4,
  (GPoint []) {
    { 1, -38 },
    { -1, -38 },
    { -1, -52 },
    { 1, -52 }
  }
};


static const GPathInfo HOUR_HAND_POINTS = {
  4, (GPoint []){
    { 1, -38},
    {-1, -38},
    {-1, -52}, 
    {1, -52}
  }
};
