#include "cpu.h"

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
        }
        // pyhsical cores
        if (strncmp("cpu cores", line, 9) == 0 && cpu_info.physical_cores == 0)
        {
            cpu_info.physical_cores = atoi(line + 12);
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
    if (cpu_info.mhz_occurrence > 0)
    {
        float avg_mhz = cpu_info.total_mhz / cpu_info.mhz_occurrence;
        cpu_info.avg_mhz = avg_mhz;
    }
    fclose(cpu_info_file);
    return cpu_info;
}
void show_cpu_info(CpuInfo *cpu_info)
{
    int x = COLS / 2;
    mvprintw(0, x, "Model name: %s\n", cpu_info->model_name);
    mvprintw(1, x, "Logical CPUs: %d\n", cpu_info->logical_cpus);
    mvprintw(2, x, "Pyhiscal cores: %d\n", cpu_info->physical_cores);
    mvprintw(3, x, "Mhz:%f", cpu_info->total_mhz / cpu_info->mhz_occurrence);
}