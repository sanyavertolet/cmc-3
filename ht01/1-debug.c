#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/mman.h>

/*
 * Общий объём памяти - 64кб: 1024*64=65536
 * размер страницы - 512 байт,
 * число страниц = 1024*64/512 = 128
 * диапазон номеров страниц: 0-127
 * Скорее всего, в таблице страниц для хранения индекса страницы используется весь старший байт,
 * хотя, кто его знает....
 *
 * https://github.com/cmc-prak/cmc-os/blob/master/2020-2021/sem06/20-memmng.pdf
 *
 * V - количество бит виртуального адреса (16)
 * P - количество бит на смещение в странице:
 *     размер страницы - 512 байтов, смещение 0-511 - 9 битов
 * V-P = 16-9 = 7 - это количество битов на номер страницы
 *     у нас всего 128 страниц, семи битов как раз хватит (0-127)
 * Таблица страниц имеет следующий вид:
 * - младший байт uint32_t не используется
 * - старший байт содержит номер физической страницы
 * Придётся для получения интекса физической страницы сдвигать вправо
 * Либо можно трактовать таблицу страниц как байтовый массив
 * и брать всегда старший байт пары: byte_ptr[pg_idx*2+1]
 * Я предпочитаю сдвигать.
 *
 */

enum
{
    FILE_SIZE = 1024 * 64, PAGE_BITS = 9, //0-511: 01 1111 1111b
    PAGE_SIZE = 1 << PAGE_BITS,
    PAGE_OFFSET_MASK = PAGE_SIZE - 1,
    PAGE_ID_SHIFT = 8
};

int
create_test(char *fname)
{
    int fd = open(fname, O_RDWR | O_CREAT, 0664);
    if(fd < 0) {
        fprintf(stderr, "open(%s): %s\n", fname, strerror(errno));
        return -1;
    }
    if(ftruncate(fd, FILE_SIZE) < 0) {
        fprintf(stderr, "ftruncate(%s,%d): %s\n", fname, FILE_SIZE, strerror(errno));
Errx:
        close(fd);
        return -1;
    }

    uint8_t *phis_mem = mmap(NULL, FILE_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if(phis_mem == MAP_FAILED) {
        fprintf(stderr, "mmap(%s,%d): %s\n", fname, FILE_SIZE, strerror(errno));
        goto Errx;
    }
    close(fd);

    unsigned page_tbl_offset = 0x100;

    //Таблица страниц адресуется словами, поэтому её тип uint16_t
    uint16_t *page_tbl = (uint16_t*) &phis_mem[page_tbl_offset];

    //Сделаем 5 страниц. В каждой - по 512 байтов, перемешаем в шахматном порядке
    //всего 5*512 = 2560 байтов, 1280 слов
    page_tbl[0] = ((0x200/PAGE_SIZE)+0) <<PAGE_ID_SHIFT;
    page_tbl[1] = ((0x200/PAGE_SIZE)+2) <<PAGE_ID_SHIFT;
    page_tbl[2] = ((0x200/PAGE_SIZE)+1) <<PAGE_ID_SHIFT;
    page_tbl[3] = ((0x200/PAGE_SIZE)+3) <<PAGE_ID_SHIFT;
    page_tbl[4] = ((0x200/PAGE_SIZE)+4) <<PAGE_ID_SHIFT;

    int virt_addr;
    int phys_page_offset;
    int virt_page_idx;
    int phys_page_idx;
    unsigned value;

    for(virt_addr = 0 ; virt_addr < 2560; virt_addr+=2 ) {
        phys_page_offset = virt_addr & PAGE_OFFSET_MASK; //Байтовое смещение в странице физической памяти
        virt_page_idx = virt_addr >> PAGE_BITS;
        phys_page_idx = page_tbl[virt_page_idx] >> PAGE_ID_SHIFT;
        value = virt_addr/2+1; //Последовательные числа от 1 ...
        *((uint16_t*) &phis_mem[phys_page_idx * PAGE_SIZE + phys_page_offset]) = value;
 //       msync(phis_mem,FILE_SIZE,MS_SYNC); //Сбрасывает буфер phis_mem[0...FILE_SIZE-1] на диск
    }

    if(munmap(phis_mem, FILE_SIZE) == -1) {
        fprintf(stderr, "munmap(%s,%d): %s\n", fname, FILE_SIZE, strerror(errno));
    }

    return 0;
}

int
main(int argc, char *argv[])
{
    //ОТЛАДКА!!!!
    //char *path = argv[1];
    char *path = "test.bin";

    //ОТЛАДКА!!!!
    //unsigned page_tbl_offset = strtoul(argv[2], NULL, 16);
    unsigned page_tbl_offset = 0x100;

    create_test(path);

    int fd = open(path, O_RDONLY);
    if(fd < 0) {
        fprintf(stderr, "open(%s): %s\n", path, strerror(errno));
        exit(1);
    }

    size_t size = FILE_SIZE;
    /*
     * Физическая память адрессуется побайтно,
     * поэтому базовый тип для всей памяти uint8_t,
     * можно unsigned char
     */
    uint8_t *phis_mem = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
    if(phis_mem == MAP_FAILED) {
        fprintf(stderr, "mmap(%s,%ld): %s\n", path, size, strerror(errno));
        exit(1);
    }
    close(fd);

    //Таблица страниц адресуется словами, поэтому её тип uint16_t
    uint16_t *page_tbl = (uint16_t*) &phis_mem[page_tbl_offset];

    int virt_addr;
    int phis_page_offset;
    int virt_page_idx;
    int phis_page_idx;
    int phys_addr;
    unsigned value;

    //ОТЛАДКА!!!
    //while(scanf("%x", &virt_addr) == 1) {
    for(virt_addr = 0; virt_addr < 2560; virt_addr +=2) {
        if(virt_addr%2) {
            fprintf(stderr, "odd address (%04X = %d) not allowed!\n", virt_addr, virt_addr );
            continue;
        }
        phis_page_offset = virt_addr & PAGE_OFFSET_MASK; //Байтовое смещение в странице физической памяти
        virt_page_idx = virt_addr >> PAGE_BITS;
        phis_page_idx = page_tbl[virt_page_idx] >> PAGE_ID_SHIFT;
        phys_addr = phis_page_idx * PAGE_SIZE + phis_page_offset;
        value = * ((uint16_t*) &phis_mem[phys_addr]);
        printf("%u\n", value);
    }

    munmap(phis_mem, size);
    return 0;
}
