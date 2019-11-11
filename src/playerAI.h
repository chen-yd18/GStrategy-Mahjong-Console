#ifndef PLAYERAI_H
#define PLAYERAI_H

#include "basicSettings.h"

Operation PlayerRedAI(Player plr,Map M);//红方AI（值传递，防止选手修改地图）
Operation PlayerBlueAI(Player plr,Map M);//蓝方AI

#endif