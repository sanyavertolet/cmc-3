#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/mman.h>


enum
{
    FILE_SIZE = 1024 * 64,
    PAGE_BITS = 9,
    PAGE_SIZE = 1 << PAGE_BITS,
    PAGE_OFFSET_MASK = PAGE_SIZE - 1,
    PAGE_ID_SHIFT = 9,
    HEX_BASE = 16,
    ARG_PATH = 1,
    ARG_ADR = 2,
    ARGC_SUCCESS = 3
};

int
main(int argc, char *argv[])
{
    if(argc != ARGC_SUCCESS) {
        fprintf(stderr, "error: two arguments should be passed.");
        return 1;
    }
    char *path = argv[ARG_PATH];

    unsigned page_tbl_offset = strtoul(argv[ARG_ADR], NULL, HEX_BASE);

    int fd = open(path, O_RDONLY);
    if(fd < 0) {
        fprintf(stderr, "open(%s): %s\n", path, strerror(errno));
        exit(1);
    }

    size_t size = FILE_SIZE;
    uint8_t *phis_mem = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
    if(phis_mem == MAP_FAILED) {
        fprintf(stderr, "mmap(%s,%ld): %s\n", path, size, strerror(errno));
        exit(1);
    }
    close(fd);

    uint16_t *page_tbl = (uint16_t*) &phis_mem[page_tbl_offset];

    int virt_addr;
    int phis_page_offset;
    int virt_page_idx;
    int phis_page_idx;
    int phys_addr;
    unsigned value;

    while(scanf("%x", &virt_addr) == 1) {
        phis_page_offset = virt_addr & PAGE_OFFSET_MASK;
        virt_page_idx = virt_addr >> PAGE_BITS;
        phis_page_idx = page_tbl[virt_page_idx] >> PAGE_ID_SHIFT;
        phys_addr = phis_page_idx * PAGE_SIZE + phis_page_offset;
        value = * ((uint16_t*) &phis_mem[phys_addr]);
        printf("%u\n", value);
    }

    munmap(phis_mem, size);
    return 0;
}
