#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "debug.h"
#include "hunt.h"

struct arguments {
    int fd;
    uint8_t *file;
    size_t file_len;
    uint8_t *flag;
    size_t flag_len;
};

static bool close_file(int fd, uint8_t *file, size_t len);
static bool open_file(struct arguments *args, char *path);
static bool parse_args(struct arguments *args, int argc, char **argv);
static bool parse_flag(struct arguments *args, char *flag);
static void print_flag_offset(size_t offset, void *arg);
static void print_usage(char *prog);

int main(int argc, char **argv)
{
    struct arguments args;

    if (!parse_args(&args, argc, argv))
        return 1;

    hunt(args.file, args.file_len, args.flag, args.flag_len, print_flag_offset, NULL);

    if (!close_file(args.fd, args.file, args.file_len))
        return 1;

    return 0;
}

static bool close_file(int fd, uint8_t *file, size_t len)
{
    if (file) {
        if (munmap(file, len) != 0) {
            error("Failed to unmap file from memory: %s\n", strerror(errno));
            (void) close(fd);
        }
    }

    if (close(fd) != 0) {
        error("Failed to close file: %s\n", strerror(errno));
        return false;
    }

    return true;
}

static bool open_file(struct arguments *args, char *path)
{
    int fd;
    struct stat statbuf;
    uint8_t *file;
    size_t file_len;

    if ((fd = open(path, O_RDONLY)) == -1) {
        error("Failed to open file: %s\n", strerror(errno));
        return false;
    }

    if (fstat(fd, &statbuf) != 0) {
        error("Failed to stat file: %s\n", strerror(errno));
        (void) close_file(fd, NULL, 0);
        return false;
    }

    file_len = statbuf.st_size;

    if ((file = mmap(NULL, file_len, PROT_READ, MAP_SHARED, fd, 0)) == MAP_FAILED) {
        error("Failed to map file to memory: %s\n", strerror(errno));
        (void) close_file(fd, NULL, 0);
        return false;
    }

    args->fd = fd;
    args->file = file;
    args->file_len = file_len;
    return true;
}

static bool parse_args(struct arguments *args, int argc, char **argv)
{
    char *prog = *argv;
    int c;

    while ((c = getopt(argc, argv, "h")) != -1) {
        switch (c) {
            case 'h':
                print_usage(prog);
                return true;
            default:
                return false;
        }
    }

    argc -= optind;
    argv += optind;

    if (argc == 0) {
        print_usage(prog);
        return true;
    }

    if (argc != 2) {
        error("flag and file arguments required\n");
        return false;
    }

    if (!parse_flag(args, argv[0]))
        return false;

    if (!open_file(args, argv[1]))
        return false;

    return true;
}

static bool parse_flag(struct arguments *args, char *flag)
{
    args->flag = (uint8_t *)flag;
    args->flag_len = strlen(flag);
    return true;
}

static void print_flag_offset(size_t offset, void *arg)
{
    (void) arg;
    printf("%016zX\n", offset);
}

static void print_usage(char *prog)
{
    printf("usage: %s [-h] flag file\n\n", prog);
    printf("Hunt down flags within files\n\n");
    printf("Arguments:\n");
    printf("  flag      flag for which to hunt\n");
    printf("  file      file in which to hunt\n");
    printf("\nOptions:\n");
    printf("  -h        show this message and exit\n");
}
