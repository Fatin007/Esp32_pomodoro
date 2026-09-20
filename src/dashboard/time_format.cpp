#include "time_format.h"

String formatTime(unsigned long totalSeconds) {

    unsigned long hours = totalSeconds / 3600;
    unsigned long minutes = (totalSeconds % 3600) / 60;
    unsigned long seconds = totalSeconds % 60;

    char buffer[12];

    if (hours > 0) {
        sprintf(buffer, "%02lu:%02lu:%02lu", hours, minutes, seconds);
    } else {
        sprintf(buffer, "%02lu:%02lu", minutes, seconds);
    }

    return String(buffer);
}
