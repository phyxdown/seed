#include <stdio.h>
#include <stdlib.h>

#include "time.h"

void main() {
    Time tm_now = timeNow();
    char* ts_now = timeFormat(tm_now);
    printf("%s\n", ts_now);
    free(ts_now);
    free(tm_now);
}

