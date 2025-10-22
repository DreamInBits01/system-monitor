#include "utils.h"
void update_interactivity_status(PadConfig *pad_config, int processes_count)
{
    move(6, 25);
    clrtoeol();
    attron(A_BOLD);
    mvprintw(6, 25, "Scrolled:%.0f%%", (float)pad_config->y / (processes_count - 1) * 100);
    if (pad_config->selected_process != NULL)
    {
        mvprintw(6, 45, "Selected process:%d, y:%d", pad_config->selected_process->pid, pad_config->y);
    }
    attroff(A_BOLD);
    refresh();
}
void remove_process_highlight(PadConfig *pad_config)
{
    if (pad_config->selected_process == NULL)
        return NULL;
    mvwprintw(pad_config->itself, pad_config->selected_process->y, 0, "Process: %d, %s, cpu_usage:%.2f%%",
              pad_config->selected_process->pid,
              pad_config->selected_process->exe_path,
              pad_config->selected_process->cpu_usage);
}
void highlight_process(PadConfig *pad_config)
{
    wattron(pad_config->itself, COLOR_PAIR(1) | A_REVERSE | A_BOLD);
    mvwprintw(pad_config->itself, pad_config->y, 0, "Process: %d, %s, cpu_usage:%.2f%%", pad_config->selected_process->pid, pad_config->selected_process->exe_path, pad_config->selected_process->cpu_usage);
    wattroff(pad_config->itself, COLOR_PAIR(1) | A_REVERSE | A_BOLD);
}
void refresh_pad(PadConfig *pad_config, unsigned processes_count)
{
    if (pad_config->y >= processes_count)
    {
        pad_config->y = processes_count - 1;
    }
    prefresh(pad_config->itself,
             pad_config->y, pad_config->x,
             pad_config->pad_view.y,
             pad_config->pad_view.x,
             pad_config->pad_view.y + pad_config->pad_view.height - 1,
             pad_config->pad_view.x + pad_config->pad_view.width - 1);
}
void handle_manual_process_selection(AppContext *ctx)
{
    // get new process
    get_selected_process(
        &ctx->processes,
        &ctx->y_to_pid,
        &ctx->pad_config.selected_process,
        &ctx->pad_config.get_process_faild,
        ctx->pad_config.selected_process_y);
    // add highlighting if successful
    if (!ctx->pad_config.get_process_faild)
    {
        ctx->pad_config.y = ctx->pad_config.selected_process_y;
        highlight_process(&ctx->pad_config);
        refresh_pad(&ctx->pad_config, ctx->processes_count);
        update_interactivity_status(&ctx->pad_config, ctx->processes_count);
    }
}
