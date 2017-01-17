#include <Gfile.h>

#ifdef _WIN32
# include <Evil.h>
#endif

#include <Eina.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/file.h>

#ifdef _WIN32
char * gfile_strndup(const char *s, size_t n);
#endif

