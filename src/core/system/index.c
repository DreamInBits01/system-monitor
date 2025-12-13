#include "core/system/index.h"

void read_system_uptime(FILE *fd, SystemStats *data)
{
    if (fd == NULL)
        return;
    fscanf(fd, "%lf %lf", &data->uptime_in_seconds, &data->idle_time);
    data->uptime_in_minutes = (unsigned)data->uptime_in_seconds / 60;
    data->uptime_in_hours = (unsigned)data->uptime_in_minutes / 60;
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
