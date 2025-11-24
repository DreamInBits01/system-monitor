#include "core/system/index.h"

void read_system_uptime(FILE *fd, SystemStats *data)
{
    fscanf(fd, "%lf %lf", &data->uptime, &data->idle_time);
}
void read_local_time(SystemStats *data)
{
    time_t current_time;
    time(&current_time);
    data->local_time = localtime(&current_time);
}
void show_local_time(Window *window, struct tm *local_time)
{
    char *time = asctime(local_time);
    mvwprintw(window->itself, 0, 0, "%s", time);
}
