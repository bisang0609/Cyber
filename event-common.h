#pragma once
// strcpy->strncpy
#define SAFE_STRCPY(dest, src) do { \
    strncpy(dest, src, sizeof(dest) - 1); \
    (dest)[sizeof(dest) - 1] = '\0'; \
} while(0)