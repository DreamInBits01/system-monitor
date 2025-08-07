#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include "memory.h"
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
    int total_memory;
    int free_memory;
    int buffers;
    int cached;
} MemoryInfo;
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
            int kb;
            sscanf(line, "MemTotal: %d", &kb);
            printf("MemTotal:%dgb\n", KB_TO_GB(kb));
        }
    };
    return memory_info;
}
int main()
{
    read_cpu_info();
    printf("----------\n");
    read_memory_info();
}