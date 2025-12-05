#include "context/helpers.h"

// CACHED FILE DESCRIPTORS
void initialize_fds(ProcFile *destination)
{
    // define it first here to easily put them inside destination
    //*destination = files wouldn't work as that would change just the first element of the context's array
    // a double pointer would require dynamiclly allocated memory which increases complexity
    const ProcFile files[CACHED_PROC_FILES_NUMBER] = {
        // PATH,KEY,READ MODE, FD,DIR*, is_directory
        {"/proc/meminfo", "meminfo", "r", NULL, NULL, false},
        {"/proc/cpuinfo", "cpuinfo", "r", NULL, NULL, false},
        {"/proc/stat", "procstat", "r", NULL, NULL, false},
        {"/proc/uptime", "uptime", "r", NULL, NULL, false},
        {"/proc/", "processes", 0, NULL, NULL, true},
    };
    for (size_t i = 0; i < CACHED_PROC_FILES_NUMBER; i++)
    {
        if (files[i].is_directory)
        {
            destination[i].dir = opendir(files[i].path);

            if (destination[i].dir == NULL)
            {
                perror(destination[i].path);
                cleanup_cached_fds(destination, i);
                exit(1);
            }
        }
        else
        {
            destination[i].fd = fopen(files[i].path, files[i].read_mode);
            if (destination[i].fd == NULL)
            {
                perror(destination[i].path);
                cleanup_cached_fds(destination, i);
                exit(1);
            }
        }
        // Copy metadata
        destination[i].path = files[i].path;
        destination[i].key = files[i].key;
        destination[i].read_mode = files[i].read_mode;
        destination[i].is_directory = files[i].is_directory;
    }
}
void cleanup_cached_fds(ProcFile *files, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        if (files[i].is_directory)
        {
            closedir(files[i].dir);
        }
        else
        {
            fclose(files[i].fd);
        }
    }
}

// CPU
bool initialize_cpu_context(CPUBlock **data, FILE *fd, int max_rows, int max_cols)
{
    *data = malloc(sizeof(CPUBlock));
    if (*data == NULL)
        return false;
    memset(*data, 0, sizeof(CPUBlock));
    (*data)->window.height = max_rows * .28;
    (*data)->window.width = max_cols;
    (*data)->window.itself = newwin(
        (*data)->window.height,
        (*data)->window.width,
        (*data)->window.y,
        (*data)->window.x);
    (*data)->data.cpu_cores_count = cpu_cores_count(fd);
    (*data)->data.cores = malloc(((*data)->data.cpu_cores_count + 1) * sizeof(CPUCore));
    return true;
}
void cleanup_cpu_context(CPUBlock *data)
{
    free(data->data.cores);
    delwin(data->window.itself);
    free(data);
}

// MEMORY
bool initialize_memory_context(MemoryBlock **data, int max_rows, int max_cols)
{
    *data = malloc(sizeof(MemoryBlock));
    if (*data == NULL)
        return false;
    memset(*data, 0, sizeof(MemoryBlock));
    (*data)->window.y = (int)(max_rows * .28);
    (*data)->window.x = (int)(max_cols * .55);
    (*data)->window.height = (int)(.35 * max_rows);
    (*data)->window.width = (int)(.46 * max_cols);
    (*data)->window.itself = newwin(
        (*data)->window.height,
        (*data)->window.width,
        (*data)->window.y,
        (*data)->window.x);
    (*data)->bar_width = (*data)->window.width / 5;
    return true;
}
void cleanup_memory_context(MemoryBlock *data)
{
    delwin(data->window.itself);
    free(data);
}

// PROCESSES
bool initialize_processes_context(ProcessesBlock **data, int max_rows, int max_cols)
{
    *data = malloc(sizeof(ProcessesBlock));
    if ((*data) == NULL)
        return false;
    memset(*data, 0, sizeof(ProcessesBlock));
    (*data)->y_to_pid = NULL;
    (*data)->processes = NULL;
    // window
    (*data)->window.height = (int)(.7 * max_rows);
    (*data)->window.width = (int)(.55 * max_cols);
    (*data)->window.y = (int)(max_rows * .28);
    (*data)->window.x = 0;
    (*data)->window.itself = newwin(
        (*data)->window.height,
        (*data)->window.width,
        (*data)->window.y,
        (*data)->window.x);
    // virtual pad
    (*data)->virtual_pad.y = 0;
    (*data)->virtual_pad.height = 500;
    (*data)->virtual_pad.width = 200;
    (*data)->virtual_pad.itself = newpad(
        (*data)->virtual_pad.height,
        (*data)->virtual_pad.width);
    //-5 to exclude the first element from the next list
    (*data)->virtual_pad.viewport_bottom = (*data)->window.height - INITIAL_VIEWPORT_BOTTOM_ALIGNMENT;
    (*data)->virtual_pad.viewport_top = 0;

    // Sorting
    (*data)->sort_options[0] = DEFAULT_SORT;
    (*data)->sort_options[1] = SORT_BY_CPU;
    // (*data)->sort_options[2] = SORT_BY_MEM;
    return true;
}
void cleanup_processes_context(ProcessesBlock *data)
{
    if (data->processes != NULL)
    {
        cleanup_processes(&data->processes);
        free(data->processes);
    }
    if (data->y_to_pid != NULL)
    {
        cleanup_y_to_pid(&data->y_to_pid);
        free(data->y_to_pid);
    }
    delwin(data->window.itself);
    delwin(data->virtual_pad.itself);
    free(data);
}

// THREADS
bool initialize_threads_context(AppContext *ctx)
{
    struct timespec interactivity_delay = {
        .tv_sec = 0,
        .tv_nsec = 25000000};
    struct timespec render_delay = {
        .tv_sec = 2,
        .tv_nsec = 500000000};
    ctx->interactivity_delay = interactivity_delay;
    ctx->render_delay = render_delay;
    pthread_mutex_init(&ctx->mutex, NULL);
    return true;
}
void cleanup_threads_context(AppContext *ctx)
{
    pthread_mutex_destroy(&ctx->mutex);
    // pthread_cond_destroy(&ctx->cv);
}