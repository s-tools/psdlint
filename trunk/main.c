
#include "libpsd.h"
#include <stdio.h>

int main(){
    psd_context context;
    psd_context* pc = &context;
    psd_image_load(&pc, "./10.psd");

    printf("%ld\n", (long)context.stream.current_pos);

    return 0;
}
