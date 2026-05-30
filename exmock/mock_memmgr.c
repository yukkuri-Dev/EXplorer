#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

static int memmgr_initialized = 0;

void memmgr_init() {
    memmgr_initialized = 1;
}

void* memmgr_alloc(size_t size) {
    if (!memmgr_initialized) {
        fprintf(stderr, "[ERR] memmgr_alloc called before init_memmgr!\n");
        raise(SIGSEGV);
    }
    return malloc(size);
}

void memmgr_free(void* ptr) {
    if (!memmgr_initialized) {
        raise(SIGSEGV);
    }
    free(ptr);
}
