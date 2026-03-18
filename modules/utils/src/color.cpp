#include "color.h"

namespace color {
    const QColor RED_TEAM(255, 102, 102);    // 红色
    const QColor BLUE_TEAM(102, 178, 255);   // 蓝色
    const QColor NEUTRAL_COLOR(245, 245, 220);     // 米色
    const QColor DEATH_COLOR(64, 64, 64);          // 深灰色
    const QColor UNKNOWN_COLOR(200, 200, 200);     // 浅灰色
    const QColor getRandomColor() {
        return QColor(
            60 + (rand() % 161),  // 60-220
            60 + (rand() % 161),  // 60-220
            60 + (rand() % 161)   // 60-220
        );
    }
}