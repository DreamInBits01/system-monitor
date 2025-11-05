#include "utils.h"
void show_process_information(Process *process, WINDOW *virtual_pad, int y)
{
    if (process->exe_path == NULL || process->name == NULL)
        return;
    mvwprintw(virtual_pad, y, 0, "%d, %s, %s, cpu_usage:%.2f%%",
              process->pid,
              process->exe_path,
              process->name,
              process->cpu_usage);
}
void update_interactivity_metadata(ProcessesBlock *data, int processes_count)
{
    mvwprintw(data->window.itself, 1, 2, "Count:%d", processes_count);
    mvwprintw(data->window.itself, 1, 25, "Scrolled:%.0f%%", (float)data->virtual_pad.y / (processes_count - 1) * 100);
}
void remove_process_highlight(ProcessesBlock *data)
{
    if (data->selected_process == NULL || data->get_process_faild)
        return;
    show_process_information(
        data->selected_process,
        data->virtual_pad.itself,
        data->virtual_pad.y);
}
void highlight_process(ProcessesBlock *data)
{
    if (data->selected_process != NULL && !data->get_process_faild)
    {
        wattron(data->virtual_pad.itself, COLOR_PAIR(1) | A_REVERSE | A_BOLD);
        show_process_information(data->selected_process, data->virtual_pad.itself, data->virtual_pad.y);
        wattroff(data->virtual_pad.itself, COLOR_PAIR(1) | A_REVERSE | A_BOLD);
    }
}
void refresh_pad(ProcessesBlock *data, unsigned processes_count)
{
    if (data->virtual_pad.y >= processes_count)
    {
        data->virtual_pad.y = processes_count - 1;
    }
    wattron(data->window.itself, COLOR_PAIR(4));
    box(data->window.itself, 0, 0);
    wattroff(data->window.itself, COLOR_PAIR(4));
    // add it after boxing
    wattron(data->window.itself, A_BOLD);
    mvwaddstr(data->window.itself, 0, 2, "Processes");
    wattroff(data->window.itself, A_BOLD);
    wrefresh(data->window.itself);
    prefresh(data->virtual_pad.itself,
             data->virtual_pad.y, data->virtual_pad.x,
             data->window.y + 3,                       // move the pad 2 units down and show borders
             data->window.x + 2,                       // move the pad 2 units right and show borders
             data->window.y + data->window.height - 2, // move the pad 2 units up and show borders,
             data->window.x + data->window.width - 2   // move the pad 2 units left and show borders
    );
}
void handle_manual_process_selection(AppContext *ctx)
{
    // get new selected process
    get_selected_process(
        &ctx->processes_block->processes,
        &ctx->processes_block->y_to_pid,
        &ctx->processes_block->selected_process,
        &ctx->processes_block->get_process_faild,
        ctx->processes_block->selected_process_y);
    // add highlighting if successful
    if (!ctx->processes_block->get_process_faild)
    {
        ctx->processes_block->virtual_pad.y = ctx->processes_block->selected_process_y;
        highlight_process(ctx->processes_block);
        mvwprintw(ctx->processes_block->window.itself, 1, 15, "Scrolled:%.0f%%", (float)ctx->processes_block->virtual_pad.y / (ctx->processes_count - 1) * 100);

        // refresh
        refresh_pad(ctx->processes_block, ctx->processes_count);

        // visible elements are in the range of pad view height
        // create a viewport_top (cursor position, starts at 0), viewport_bottom (viewtop+pad_view.height)
        // if y is outside of the boundary, refresh update viewport to show the new y and refresh

        // if (ctx->pad_config.y % ctx->pad_config.pad_view.height + 1 == 0)
        // {
        //     refresh_pad(&ctx->pad_config, ctx->processes_count);
        // }
        // else
        // {
        //     prefresh(ctx->pad_config.itself,
        //              ctx->pad_config.y - (ctx->pad_config.y % ctx->pad_config.pad_view.height), // Keep current viewport start
        //              ctx->pad_config.x,
        //              ctx->pad_config.pad_view.y,
        //              ctx->pad_config.pad_view.x,
        //              ctx->pad_config.pad_view.y + ctx->pad_config.pad_view.height - 1,
        //              ctx->pad_config.pad_view.x + ctx->pad_config.pad_view.width - 1);
        // }
    }
}