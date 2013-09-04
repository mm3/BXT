#define FUSE_USE_VERSION  26
   
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

static const char* bxt_version = "0.0.1";
   
static const char *bxt_str = "Hello World!\n";
static const char *bxt_path = "/hello";

struct bxt_config {
     char *bxt_mountpoint;
};

enum {
     KEY_HELP,
     KEY_VERSION,
};

#define BXT_OPT(t, p, v) { t, offsetof(struct bxt_config, p), v }

static struct fuse_opt bxt_opts[] = {
     //BXT_OPT("-i %s",  bxt_mountpoint, 0),
     //BXT_OPT("in=%s",   bxt_mountpoint, 0),

     FUSE_OPT_KEY("-V",             KEY_VERSION),
     FUSE_OPT_KEY("--version",      KEY_VERSION),
     FUSE_OPT_KEY("-h",             KEY_HELP),
     FUSE_OPT_KEY("--help",         KEY_HELP),
     FUSE_OPT_END
};
  
static int bxt_getattr(const char *path, struct stat *stbuf)
{
    int res = 0;
    memset(stbuf, 0, sizeof(struct stat));
    if(strcmp(path, "/") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
    }
    else if(strcmp(path, bxt_path) == 0) {
        stbuf->st_mode = S_IFREG | 0444;
        stbuf->st_nlink = 1;
        stbuf->st_size = strlen(bxt_str);
    }
    else
        res = -ENOENT;

    return res;
}
  
static int bxt_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                         off_t offset, struct fuse_file_info *fi)
{
    (void) offset;
    (void) fi;

    if(strcmp(path, "/") != 0)
        return -ENOENT;

    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);
    filler(buf, bxt_path + 1, NULL, 0);

    return 0;
}

static int bxt_open(const char *path, struct fuse_file_info *fi)
{
    if(strcmp(path, bxt_path) != 0)
        return -ENOENT;

    if((fi->flags & 3) != O_RDONLY)
        return -EACCES;

    return 0;
}

static int bxt_read(const char *path, char *buf, size_t size, off_t offset,
                      struct fuse_file_info *fi)
{
    size_t len;
    (void) fi;
    if(strcmp(path, bxt_path) != 0)
        return -ENOENT;

    len = strlen(bxt_str);
    if (offset < len) {
        if (offset + size > len)
            size = len - offset;
        memcpy(buf, bxt_str + offset, size);
    } else
        size = 0;

    return size;
}


  
static struct fuse_operations bxt_oper = {
    .getattr   = bxt_getattr,
    .readdir = bxt_readdir,
    .open   = bxt_open,
    .read   = bxt_read,
};
  
static int bxt_opt_proc(void *data, const char *arg, int key, struct fuse_args *outargs)
{
     switch (key) {
     case KEY_HELP:
             fprintf(stderr,
                     "usage: %s mountpoint [options]\n"
                     "\n"
                     "general options:\n"
                     "    -o opt,[opt...]  mount options\n"
                     "    -h   --help      print help\n"
                     "    -V   --version   print version\n"
                     "\n"
                     , outargs->argv[0]);
             fuse_opt_add_arg(outargs, "-ho");
             fuse_main(outargs->argc, outargs->argv, &bxt_oper, NULL);
             exit(1);

     case KEY_VERSION:
             fprintf(stderr, "BXT version %s\n", bxt_version);
             fuse_opt_add_arg(outargs, "--version");
             fuse_main(outargs->argc, outargs->argv, &bxt_oper, NULL);
             exit(0);
     }
     return 1;
}

int main(int argc, char *argv[])
{
    struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
    struct bxt_config conf;
    fuse_opt_parse(&args, &conf, bxt_opts, bxt_opt_proc);

    return fuse_main(argc, argv, &bxt_oper, NULL);
}

