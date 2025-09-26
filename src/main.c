#include "main.h"
// main thread for handling rendering
//  worker thread for handling interactivity
// Create a Consumer, producer pattern for threads,
// one thread generates the action (producer), the other executes it (consumer)

/*
-Refreshing happens on the rendering worker
-The switch case responsible for interactivity works on another thread
-the pad variables are shared between these two threads



*/
void cleanup_context(TaskManagerContext *ctx)
{
    // processes
    free(ctx->processes_count);
    cleanup_processes(&ctx->processes);
    // memory
    free(ctx->memory_info);
    // cpu
    free(ctx->cpu_info);
    // pad
    free(ctx->pad_config.y);
    delwin(ctx->pad_config.itself);
    // pad view
    delwin(ctx->pad_config.pad_view.itself);
    ctx->running = 0;
    pthread_mutex_destroy(&ctx->pad_config.mutex);
    pthread_mutex_destroy(&ctx->render_mutex);
}
void *render_routine(void *data)
{
    TaskManagerContext *ctx = (TaskManagerContext *)data;
    while (ctx->running)
    {
        pthread_mutex_lock(&ctx->render_mutex);
        clear();
        read_memory_info(ctx->memory_info);
        show_memory_info(ctx->memory_info, ctx->bar_width);
        read_cpu_info(ctx->cpu_info);
        show_cpu_info(ctx->cpu_info);
        read_processes(&ctx->processes, ctx->processes_count);

        attron(A_BOLD);
        mvprintw(6, 0, "Processes count:%ld", *ctx->processes_count);
        mvprintw(6, 25, "Scrolled:%.1f%%", (float)*ctx->pad_config.y / (*ctx->processes_count - ctx->pad_config.pad_view.height) * 100);
        attroff(A_BOLD);
        refresh();
        werase(ctx->pad_config.itself);
        show_processes(&ctx->processes, ctx->pad_config.itself, ctx->pad_config.height, *ctx->pad_config.y);
        pthread_mutex_lock(&ctx->pad_config.mutex);
        prefresh(ctx->pad_config.itself,
                 *ctx->pad_config.y, ctx->pad_config.x,
                 ctx->pad_config.pad_view.y,
                 ctx->pad_config.pad_view.x,
                 ctx->pad_config.pad_view.y + ctx->pad_config.pad_view.height - 1,
                 ctx->pad_config.pad_view.x + ctx->pad_config.pad_view.width - 1);
        pthread_mutex_unlock(&ctx->pad_config.mutex);
        pthread_mutex_unlock(&ctx->render_mutex);
        struct timespec sleep_time = {2, 200000000};
        nanosleep(&sleep_time, NULL);
    }
    return NULL;
}

void *interactivity_routine(void *data)
{
    TaskManagerContext *ctx = (TaskManagerContext *)data;
    while (ctx->running)
    {
        pthread_mutex_lock(&ctx->render_mutex);
        int ch = getch();
        switch (ch)
        {
        case KEY_F(4):
            goto cleanup;
        case KEY_UP:
            if (*ctx->pad_config.y > 0)
            {
                pthread_mutex_lock(&ctx->pad_config.mutex);
                *ctx->pad_config.y -= 1;
                pthread_mutex_unlock(&ctx->pad_config.mutex);
                prefresh(ctx->pad_config.itself,
                         *ctx->pad_config.y, ctx->pad_config.x,
                         ctx->pad_config.pad_view.y,
                         ctx->pad_config.pad_view.x,
                         ctx->pad_config.pad_view.y + ctx->pad_config.pad_view.height - 1,
                         ctx->pad_config.pad_view.x + ctx->pad_config.pad_view.width - 1);
            }
            break;
        case KEY_DOWN:
            if (*ctx->pad_config.y < *ctx->processes_count - ctx->pad_config.pad_view.height)
            {
                pthread_mutex_lock(&ctx->pad_config.mutex);
                *ctx->pad_config.y += 1;
                pthread_mutex_unlock(&ctx->pad_config.mutex);
                prefresh(ctx->pad_config.itself,
                         *ctx->pad_config.y, ctx->pad_config.x,
                         ctx->pad_config.pad_view.y,
                         ctx->pad_config.pad_view.x,
                         ctx->pad_config.pad_view.y + ctx->pad_config.pad_view.height - 1,
                         ctx->pad_config.pad_view.x + ctx->pad_config.pad_view.width - 1);
            };
            break;
        case KEY_HOME:
            pthread_mutex_lock(&ctx->pad_config.mutex);
            *ctx->pad_config.y = 0;
            pthread_mutex_unlock(&ctx->pad_config.mutex);

            prefresh(ctx->pad_config.itself,
                     *ctx->pad_config.y, ctx->pad_config.x,
                     ctx->pad_config.pad_view.y,
                     ctx->pad_config.pad_view.x,
                     ctx->pad_config.pad_view.y + ctx->pad_config.pad_view.height - 1,
                     ctx->pad_config.pad_view.x + ctx->pad_config.pad_view.width - 1);
            break;
        case KEY_END:
            pthread_mutex_lock(&ctx->pad_config.mutex);
            *ctx->pad_config.y = ctx->pad_config.height - ctx->pad_config.pad_view.height;
            pthread_mutex_unlock(&ctx->pad_config.mutex);
            prefresh(ctx->pad_config.itself,
                     *ctx->pad_config.y, ctx->pad_config.x,
                     ctx->pad_config.pad_view.y,
                     ctx->pad_config.pad_view.x,
                     ctx->pad_config.pad_view.y + ctx->pad_config.pad_view.height - 1,
                     ctx->pad_config.pad_view.x + ctx->pad_config.pad_view.width - 1);
            break;
        }
        pthread_mutex_unlock(&ctx->render_mutex);
    }
cleanup:
    cleanup_context(ctx);
    endwin();
    return NULL;
}
void initialize_ncurses()
{
    // Ncurses config
    slk_init(1);
    initscr();
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    noecho();
    cbreak();
    curs_set(0);
    slk_set(1, "Help", 1);
    slk_set(2, "Save", 1);
    slk_set(3, "Load", 1);
    slk_set(4, "Quit", 1);
    slk_refresh();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
}
void initialize_task_manager(TaskManagerContext *ctx)
{
    //*Todo, add error handling for malloc
    // ctx mutex
    ctx->running = 1;
    pthread_mutex_init(&ctx->render_mutex, NULL);
    // Processes config
    ctx->processes = NULL;
    ctx->processes_count = malloc(sizeof(size_t));
    *ctx->processes_count = 0;
    // Memory config
    ctx->memory_info = malloc(sizeof(MemoryInfo));
    ctx->bar_width = COLS / 4;
    memset(ctx->memory_info, 0, sizeof(MemoryInfo));
    // Cpu config
    ctx->cpu_info = malloc(sizeof(CpuInfo));
    memset(ctx->cpu_info, 0, sizeof(CpuInfo));
    // Pad config
    ctx->pad_config.height = 1000;
    ctx->pad_config.width = 200;
    ctx->pad_config.y = malloc(sizeof(int));
    *ctx->pad_config.y = 0;
    ctx->pad_config.itself = newpad(ctx->pad_config.height, ctx->pad_config.width);
    pthread_mutex_init(&ctx->pad_config.mutex, NULL);
    // Pad view config
    ctx->pad_config.pad_view.height = (int)(.7 * LINES);
    ctx->pad_config.pad_view.width = COLS;
    ctx->pad_config.pad_view.y = 8;
    ctx->pad_config.pad_view.x = 0;
    ctx->pad_config.pad_view.itself = newwin(ctx->pad_config.pad_view.height, ctx->pad_config.pad_view.width, *ctx->pad_config.y, ctx->pad_config.pad_view.x);
    // Rendering the pad
    box(ctx->pad_config.pad_view.itself, 0, 0);
    wrefresh(ctx->pad_config.pad_view.itself);
}

int main()
{
    initialize_ncurses();
    TaskManagerContext ctx = {0};
    initialize_task_manager(&ctx);
    pthread_t interactivity_thread_id;
    pthread_t render_thread_id;
    // create threads
    pthread_create(&interactivity_thread_id, NULL, interactivity_routine, &ctx);
    pthread_create(&render_thread_id, NULL, render_routine, &ctx);

    // join thread
    pthread_join(interactivity_thread_id, NULL);
    pthread_join(render_thread_id, NULL);
    return 0;
}
