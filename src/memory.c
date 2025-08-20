#include "memory.h"

MemoryInfo read_memory_info()
{
    FILE *memory_info_file = fopen("/proc/meminfo", "r");
    if (memory_info_file == NULL)
    {
        printf("Error while reading memory_info_file\n");
        endwin();
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
            memory_info.total = KB_TO_GB(kb);
        }
        if (strncmp("MemFree:", line, 8) == 0)
        {
            float kb;
            sscanf(line, "MemFree: %f", &kb);
            memory_info.free = KB_TO_GB(kb);
        }
        if (strncmp("Buffers:", line, 8) == 0)
        {
            unsigned kb;
            sscanf(line, "Buffers: %d", &kb);
            memory_info.buffers = kb;
        }
    };
    return memory_info;
}