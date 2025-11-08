#include "utils.h"
// int proc_read_buffer_size()
// {
//     int cores = sysconf(_SC_NPROCESSORS_ONLN);
//     int page_size = sysconf(_SC_PAGESIZE);

// }
int find_proc_file_fd(ProcFile *files, char *key)
{
    for (size_t i = 0; i < CACHED_PROC_FILES_NUMBER; i++)
    {
        if (strncmp(files[i].key, key, sizeof(files[i].key)) == 0)
        {
            return files[i].fd;
        }
    }
    return -1;
}
DIR *find_proc_dir_fd(ProcFile *files, char *key)
{
    for (size_t i = 0; i < CACHED_PROC_FILES_NUMBER; i++)
    {
        if (files[i].is_directory && strncmp(files[i].key, key, sizeof(files[i].key)) == 0)
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
bool proc_file_read_and_parse(int fd, void (*callback)(char *line, void *output), void *output)
{
    if (fd < 0)
    {
        perror("Invalid file descriptor");
        return false;
    }
    // start at the beginning of the file
    if (lseek(fd, 0, SEEK_SET) < 0)
    {
        perror("Lseek error");
        return false;
    }
    char buffer[sysconf(_SC_PAGESIZE) * 2];
    //-1 is put to reserve a place for '\0'
    ssize_t bytes = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes < 0)
    {
        perror("Error while parsing proc\n");
        return false;
    }
    // empty file
    if (bytes == 0)
        return true;
    // null-terminate buffer
    buffer[bytes] = '\0';
    char *line = strtok(buffer, "\n");
    while (line != NULL)
    {
        if (*line == '\0')
            continue;
        callback(line, output);
        line = strtok(NULL, "\n");
    }
    return true;
}
void show_process_information(Process *process, Window *window, WINDOW *virtual_pad, int y)
{
    if (process->exe_path == NULL || process->name == NULL)
        return;
    mvwprintw(virtual_pad, y, 2, "%d", process->pid);
    mvwprintw(virtual_pad, y, (window->width * .16), "%s", process->name);
    mvwprintw(virtual_pad, y, (window->width * .5), "%.2f", process->cpu_usage);
    mvwprintw(virtual_pad, y, (window->width * .6), "%s", process->exe_path);
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
        &data->window,
        data->virtual_pad.itself,
        data->virtual_pad.y);
}
void highlight_process(ProcessesBlock *data)
{
    if (data->selected_process != NULL && !data->get_process_faild)
    {
        wattron(data->virtual_pad.itself, COLOR_PAIR(1) | A_REVERSE | A_BOLD);
        show_process_information(
            data->selected_process,
            &data->window, data->virtual_pad.itself, data->virtual_pad.y);
        wattroff(data->virtual_pad.itself, COLOR_PAIR(1) | A_REVERSE | A_BOLD);
    }
}
void refresh_pad(ProcessesBlock *data, unsigned processes_count)
{
    if (data->virtual_pad.y >= processes_count)
    {
        data->virtual_pad.y = processes_count - 1;
    }
    wclear(data->window.itself);
    wattron(data->window.itself, COLOR_PAIR(4));
    box(data->window.itself, 0, 0);
    wattroff(data->window.itself, COLOR_PAIR(4));
    // add it after boxing
    wattron(data->window.itself, A_BOLD);
    mvwaddstr(data->window.itself, 0, 2, "Processes");
    mvwprintw(
        data->window.itself,
        data->window.height - 1,
        2,
        "%d/%d", data->virtual_pad.y,
        processes_count);
    mvwprintw(data->window.itself, 2, 2, "PID");
    mvwprintw(data->window.itself, 2, data->window.width * .16, "Name");
    mvwprintw(data->window.itself, 2, data->window.width * .5, "CPU");
    mvwprintw(data->window.itself, 2, data->window.width * .6, "Path");
    wattroff(data->window.itself, A_BOLD);
    wrefresh(data->window.itself);
    prefresh(data->virtual_pad.itself,
             data->virtual_pad.y, data->virtual_pad.x + 2,
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
        mvwprintw(ctx->processes_block->window.itself, 1, 15, "Scrolled:%.0f%%", (float)ctx->processes_block->virtual_pad.y / (ctx->processes_block->processes_count - 1) * 100);

        // refresh
        refresh_pad(ctx->processes_block, ctx->processes_block->processes_count);

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