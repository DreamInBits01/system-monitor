#include "context/index.h"

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
void cleanup_context(AppContext *ctx)
{
    ctx->running = 0;
    // fds
    cleanup_cached_fds(ctx->proc_files, CACHED_PROC_FILES_NUMBER);
    if (ctx->processes_block != NULL)
    {
        if (ctx->processes_block->processes != NULL)
        {
            cleanup_processes(&ctx->processes_block->processes);
            free(ctx->processes_block->processes);
        }
        if (ctx->processes_block->y_to_pid != NULL)
        {
            cleanup_y_to_pid(&ctx->processes_block->y_to_pid);
            free(ctx->processes_block->y_to_pid);
        }
        delwin(ctx->processes_block->window.itself);
        delwin(ctx->processes_block->virtual_pad.itself);
        free(ctx->processes_block);
    }

    // memory
    if (ctx->memory_block != NULL)
        cleanup_memory_context(ctx->memory_block);

    // CPU cleanup
    if (ctx->cpu_block != NULL)
        cleanup_cpu_context(ctx->cpu_block);
    // sort
    delwin(ctx->sort_menu.window);
    del_panel(ctx->sort_menu.panel);
    // ctx
    pthread_mutex_destroy(&ctx->mutex);
    // pthread_cond_destroy(&ctx->cv);
    free(ctx);
    endwin();
}
void initialize_fds(ProcFile *destination)
{
    // define it first here to easily put them inside destination
    //*destination = files wouldn't work as that would change just the first element of the context's array
    // a double pointer would require dynamiclly allocated memory which increases complexity
    ProcFile files[CACHED_PROC_FILES_NUMBER] = {
        // PATH,KEY,READ MODE, FD,DIR*, is_directory
        {"/proc/meminfo", "meminfo", "r", NULL, NULL, false},
        {"/proc/cpuinfo", "cpuinfo", "r", NULL, NULL, false},
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
void initialize_context(AppContext *ctx)
{
    // screen
    getmaxyx(stdscr, ctx->max_rows, ctx->max_cols);
    initialize_fds(ctx->proc_files);
    // Memory conf
    if (!initialize_memory_context(&ctx->memory_block, ctx->max_rows, ctx->max_cols))
    {
        cleanup_context(ctx);
        exit(1);
    }
    // CPU config
    if (!initialize_cpu_context(&ctx->cpu_block, ctx->max_rows, ctx->max_cols))
    {
        cleanup_context(ctx);
        exit(1);
    }
    // Processes block
    ctx->processes_block = malloc(sizeof(ProcessesBlock));
    if (ctx->processes_block == NULL)
    {
        cleanup_context(ctx);
        exit(1);
    }
    ctx->processes_block->y_to_pid = NULL;
    ctx->processes_block->processes = NULL;
    // window
    ctx->processes_block->window.height = (int)(.7 * ctx->max_rows);
    ctx->processes_block->window.width = (int)(.55 * ctx->max_cols);
    ctx->processes_block->window.y = (int)(ctx->max_rows * .28);
    ctx->processes_block->window.x = 0;
    ctx->processes_block->window.itself = newwin(
        ctx->processes_block->window.height,
        ctx->processes_block->window.width,
        ctx->processes_block->window.y,
        ctx->processes_block->window.x);
    // virtual pad
    ctx->processes_block->virtual_pad.y = 0;
    ctx->processes_block->virtual_pad.height = 500;
    ctx->processes_block->virtual_pad.width = 200;
    ctx->processes_block->virtual_pad.itself = newpad(
        ctx->processes_block->virtual_pad.height,
        ctx->processes_block->virtual_pad.width);
    //-5 to exclude the first element from the next list
    ctx->processes_block->virtual_pad.viewport_bottom = ctx->processes_block->window.height - INITIAL_VIEWPORT_BOTTOM_ALIGNMENT;
    ctx->processes_block->virtual_pad.viewport_top = 0;
    // Sort menu
    ctx->sort_menu.window = newwin(15, 50, (ctx->max_rows - 15) / 2, (ctx->max_cols - 50) / 2);
    box(ctx->sort_menu.window, 0, 0);
    mvwaddstr(ctx->sort_menu.window, 1, 1, "Sort");
    // sort panel
    ctx->sort_menu.panel = new_panel(ctx->sort_menu.window);
    ctx->sort_menu.visible = false;
    // ctx mutex
    ctx->running = 1;
    pthread_mutex_init(&ctx->mutex, NULL);
    struct timespec interactivity_delay = {
        .tv_sec = 0,
        .tv_nsec = 25000000};
    struct timespec render_delay = {
        .tv_sec = 2,
        .tv_nsec = 500000000};
    ctx->interactivity_delay = interactivity_delay;
    ctx->render_delay = render_delay;
    // pthread_cond_init(&ctx->cv, NULL);
}