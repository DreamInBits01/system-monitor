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

// void *render_routine(void *data)
// {
//     while (1)
//     {
//         TaskManagerContext *ctx = (TaskManagerContext *)data;
//         read_memory_info(ctx->memory_info);
//         show_memory_info(ctx->memory_info, ctx->bar_width);
//         read_cpu_info(ctx->cpu_info);
//         show_cpu_info(ctx->cpu_info);
//         read_processes(&ctx->processes, ctx->processes_count);

//         attron(A_BOLD);
//         mvprintw(6, 0, "Processes count:%ld", *ctx->processes_count);
//         mvprintw(6, 25, "Scrolled:%.1f%%", (float)*ctx->pad_config.y / (*ctx->processes_count - ctx->pad_config.pad_view.height) * 100);
//         attroff(A_BOLD);
//         refresh();

//         werase(ctx->pad_config.itself);
//         show_processes(&ctx->processes, ctx->pad_config.itself, ctx->pad_config.height, *ctx->pad_config.y);
//         prefresh(ctx->pad_config.itself,
//                  *ctx->pad_config.y, ctx->pad_config.x,
//                  ctx->pad_config.pad_view.y,
//                  ctx->pad_config.pad_view.x,
//                  ctx->pad_config.pad_view.y + ctx->pad_config.pad_view.height - 1,
//                  ctx->pad_config.pad_view.x + ctx->pad_config.pad_view.width - 1);
//         getch();
//         sleep(2);
//     }
// }
void initialize_task_manager(TaskManagerContext *ctx)
{
    slk_init(1);
    initscr();
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    noecho();
    cbreak();
    slk_set(1, "Help", 1);
    slk_set(2, "Save", 1);
    slk_set(3, "Load", 1);
    slk_set(4, "Quit", 1);
    slk_refresh();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    ctx->processes = NULL;
    ctx->processes_count = malloc(sizeof(size_t));
    *ctx->processes_count = 0;
    //
    ctx->memory_info = malloc(sizeof(MemoryInfo));
    ctx->bar_width = COLS / 4;
    memset(ctx->memory_info, 0, sizeof(MemoryInfo));
    //
    ctx->cpu_info = malloc(sizeof(CpuInfo));
    memset(ctx->cpu_info, 0, sizeof(CpuInfo));
    //
    ctx->pad_config.height = 1000;
    ctx->pad_config.width = 200;
    ctx->pad_config.y = malloc(sizeof(int));
    *ctx->pad_config.y = 0;
    ctx->pad_config.itself = newpad(ctx->pad_config.height, ctx->pad_config.width);
    //
    ctx->pad_config.pad_view.height = (int)(.7 * LINES);
    ctx->pad_config.pad_view.width = COLS;
    ctx->pad_config.pad_view.y = 8;
    ctx->pad_config.pad_view.x = 0;

    ctx->pad_config.pad_view.itself = newwin(ctx->pad_config.pad_view.height, ctx->pad_config.pad_view.width, *ctx->pad_config.y, ctx->pad_config.pad_view.x);

    box(ctx->pad_config.pad_view.itself, 0, 0);
    wrefresh(ctx->pad_config.pad_view.itself);
}
int main()
{
    TaskManagerContext ctx = {0};
    initialize_task_manager(&ctx);
    while (1)
    {
        read_memory_info(ctx.memory_info);
        show_memory_info(ctx.memory_info, ctx.bar_width);
        read_cpu_info(ctx.cpu_info);
        show_cpu_info(ctx.cpu_info);
        read_processes(&ctx.processes, ctx.processes_count);

        attron(A_BOLD);
        mvprintw(6, 0, "Processes count:%ld", *ctx.processes_count);
        mvprintw(6, 25, "Scrolled:%.1f%%", (float)*ctx.pad_config.y / (*ctx.processes_count - ctx.pad_config.pad_view.height) * 100);
        attroff(A_BOLD);
        refresh();

        werase(ctx.pad_config.itself);
        show_processes(&ctx.processes, ctx.pad_config.itself, ctx.pad_config.height, *ctx.pad_config.y);
        prefresh(ctx.pad_config.itself,
                 *ctx.pad_config.y, ctx.pad_config.x,
                 ctx.pad_config.pad_view.y,
                 ctx.pad_config.pad_view.x,
                 ctx.pad_config.pad_view.y + ctx.pad_config.pad_view.height - 1,
                 ctx.pad_config.pad_view.x + ctx.pad_config.pad_view.width - 1);
        int ch = getch();
        switch (ch)
        {
        case KEY_F(4):
            goto cleanup;
        case KEY_UP:
            if (*ctx.pad_config.y > 0)
                *ctx.pad_config.y -= 1;
            break;
        case KEY_DOWN:
            if (*ctx.pad_config.y < *ctx.processes_count - ctx.pad_config.pad_view.height)
                *ctx.pad_config.y += 1;
            break;
        case KEY_HOME:
            *ctx.pad_config.y = 0;
            break;
        case KEY_END:
            *ctx.pad_config.y = ctx.pad_config.height - ctx.pad_config.pad_view.height;
            break;
        }
        sleep(2);
    }
cleanup:
    delwin(ctx.pad_config.itself);
    delwin(ctx.pad_config.pad_view.itself);
    cleanup_processes(&ctx.processes);
    endwin();
}