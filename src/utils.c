#include "utils.h"
// int proc_read_buffer_size()
// {
//     int cores = sysconf(_SC_NPROCESSORS_ONLN);
//     int page_size = sysconf(_SC_PAGESIZE);

// }
FILE *find_proc_file_fd(ProcFile *files, char *key)
{
    for (size_t i = 0; i < CACHED_PROC_FILES_NUMBER; i++)
    {
        if (!files[i].is_directory && strcmp(files[i].key, key) == 0)
        {
            return files[i].fd;
        }
    }
    return NULL;
}
DIR *find_proc_dir_fd(ProcFile *files, char *key)
{
    for (size_t i = 0; i < CACHED_PROC_FILES_NUMBER; i++)
    {
        if (files[i].is_directory && strcmp(files[i].key, key) == 0)
        {
            return files[i].dir;
        }
    }
    return NULL;
}
bool proc_dir_read_and_parse(DIR *fd, void (*callback)(struct dirent *ep, void *output), void *output)
{
    if (fd == NULL)
    {
        perror("Invalid file descriptor");
        return false;
    }
    // start at the beginning of the directory
    rewinddir(fd);
    struct dirent *ep;
    while ((ep = readdir(fd)) != NULL)
    {
        callback(ep, output);
    }
    return true;
}
bool proc_file_read_and_parse(FILE *fd, void (*callback)(char *line, void *output), void *output)
{
    if (fd == NULL)
    {
        perror("Invalid file descriptor");
        return false;
    }
    // start at the beginning of the file
    if (fseek(fd, 0, SEEK_SET) < 0)
    {
        perror("lseek");
        return false;
    }
    char line[256];
    while (fgets(line, sizeof(line), fd))
    {
        if (*line == '\0')
            continue;
        callback(line, output);
    }
    return true;
}
