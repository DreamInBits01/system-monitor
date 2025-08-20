#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include "memory.h"
#include "ncurses.h"
typedef struct
{
    int logical_cpus;
    int physical_cores;
    int mhz_occurrence;
    float total_mhz;
    char model_name[128];

} CpuInfo;
typedef struct
{
    // gb
    int buffers;
    int cached;
    float total_memory;
    float free_memory;
} MemoryInfo;
int is_numeric(char *name)
{
    while (*name)
    {
        if (!isdigit(*name))
            return 0;
        name++;
    };
    return 1;
}
CpuInfo read_cpu_info()
{
    FILE *cpu_info_file = fopen("/proc/cpuinfo", "r");
    if (cpu_info_file == NULL)
    {
        printf("Error while reading cpu_info_file\n");
        exit(1);
    }
    char line[256];
    CpuInfo cpu_info = {0};
    while (fgets(line, sizeof(line), cpu_info_file))
    {
        // logical CPUs
        if (strncmp("processor", line, 9) == 0)
        {
            cpu_info.logical_cpus++;
        }
        // model name
        if (strncmp("model name", line, 10) == 0 && strlen(cpu_info.model_name) == 0)
        {
            strncpy(cpu_info.model_name, line + 13, 128);
            printf("CPU:%s", cpu_info.model_name);
        }
        // pyhsical cores
        if (strncmp("cpu cores", line, 9) == 0 && cpu_info.physical_cores == 0)
        {
            cpu_info.physical_cores = atoi(line + 12);
            printf("CPU cores:%d\n", cpu_info.physical_cores);
        }
        // mhz
        if (strncmp("cpu MHz", line, 7) == 0)
        {
            float current_mhz;
            if (sscanf(line, "cpu MHz : %f", &current_mhz) == 1)
            {
                cpu_info.total_mhz += current_mhz;
                cpu_info.mhz_occurrence += 1;
            }
        };
    };
    printf("Logical CPUs:%d\n", cpu_info.logical_cpus);
    if (cpu_info.mhz_occurrence > 0)
    {
        float avg_mhz = cpu_info.total_mhz / cpu_info.mhz_occurrence;
        printf("Average CPU frequency: %.2f MHz\n", avg_mhz);
    }
    fclose(cpu_info_file);
    return cpu_info;
}
MemoryInfo read_memory_info()
{
    FILE *memory_info_file = fopen("/proc/meminfo", "r");
    if (memory_info_file == NULL)
    {
        printf("Error while reading memory_info_file\n");
        exit(1);
    }
    char line[256];
    MemoryInfo memory_info = {0};
    while (fgets(line, sizeof(line), memory_info_file))
    {
        if (strncmp("MemTotal:", line, 9) == 0)
        {
            float kb;
            sscanf(line, "MemTotal: %f", &kb);
            memory_info.total_memory = KB_TO_GB(kb);
        }
        if (strncmp("MemFree:", line, 8) == 0)
        {
            float kb;
            sscanf(line, "MemFree: %f", &kb);
            memory_info.free_memory = KB_TO_GB(kb);
        }
    };
    return memory_info;
}
struct dirent *get_processes()
{
    DIR *directory = opendir("/proc/");
    if (directory == NULL)
    {
        printf("Error while opening a directory!\n");
    };
    struct dirent *ep;
    struct dirent *processes = malloc(10 * sizeof(struct dirent));
    if (processes == NULL)
    {
        printf("Error while allocating processes\n");
        exit(1);
    }
    int processes_index = 0;
    while ((ep = readdir(directory)) != NULL && processes_index < 10)
    {
        if (is_numeric(ep->d_name))
        {
            processes[processes_index] = *ep;
            // printf("A process:%s\n", ep->d_name);
            processes_index++;
        }
    };
    closedir(directory);
    return processes;
}
void build_loadbar(int fill, int bar_width, int y, int x)
{
    mvaddch(y, x, '[');
    for (size_t i = 0; i < bar_width; i++)
    {
        if (i < fill)
        {
            addch('#');
        }
        else
        {
            addch(' ');
        };
    }
    addch(']');
}
int main()
{
    slk_init(1);
    initscr();
    noecho();
    cbreak();

    slk_set(1, "Help", 1);
    slk_set(2, "Save", 1);
    slk_set(3, "Load", 1);
    slk_set(4, "Quit", 1);
    slk_refresh();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    int bar_width = COLS / 4;
    MemoryInfo memory_info = read_memory_info();
    while (1)
    {
        memory_info = read_memory_info();
        double used_percent = (memory_info.total_memory - memory_info.free_memory) / memory_info.total_memory * 100;
        // get a fraction and multiply it by the width
        int fill = used_percent / 100 * bar_width;
        mvprintw(0, 0, "Task manager (press q to quit)\n");
        mvprintw(1, 0, "Memory total: %f\n", memory_info.total_memory);
        mvprintw(2, 0, "Memory free: %f\n", memory_info.free_memory);
        mvprintw(3, 0, "Buffers: %d\n", memory_info.buffers);
        mvprintw(4, 0, "Used memory:%f", used_percent);
        build_loadbar(fill, bar_width, 5, 0);
        refresh();
        int ch = getch();
        if (ch == KEY_F(4))
        {
            break;
        }
        sleep(1.5);
    }
    endwin();
    return 0;
}