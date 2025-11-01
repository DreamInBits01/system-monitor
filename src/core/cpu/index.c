#include "core/cpu/index.h"

void read_dynamic_cpu_data(DynamicCpuData *dynamic_cpu_data)
{
    FILE *cpu_data_file = fopen("/proc/cpuinfo", "r");
    if (cpu_data_file == NULL)
    {
        printf("Error while reading cpu_data_file\n");
        exit(1);
    }
    char line[256];
    float total_mhz = 0;
    int mhz_occurrence = 0;
    while (fgets(line, sizeof(line), cpu_data_file))
    {
        if (strncmp("cpu MHz", line, 7) == 0)
        {
            float current_mhz;
            if (sscanf(line, "cpu MHz : %f", &current_mhz) == 1)
            {
                total_mhz += current_mhz;
                mhz_occurrence += 1;
            }
        };
    };
    if (mhz_occurrence > 0)
    {
        float avg_mhz = (float)total_mhz / mhz_occurrence;
        dynamic_cpu_data->avg_mhz = avg_mhz;
    }
    fclose(cpu_data_file);
}
void show_dynamic_cpu_data(DynamicCpuData *dynamic_cpu_data, int max_cols)
{
    int x = max_cols / 2;
    mvprintw(3, x, "Avg Mhz:%.2f", dynamic_cpu_data->avg_mhz);
}
void read_static_cpu_data(StaticCpuData *static_cpu_data)
{
    FILE *cpu_data_file = fopen("/proc/cpuinfo", "r");
    if (cpu_data_file == NULL)
    {
        printf("Error while reading cpu_data_file\n");
        exit(1);
    }
    char line[256];
    static_cpu_data->logical_cpus = 0;
    while (fgets(line, sizeof(line), cpu_data_file))
    {
        // logical CPUs
        if (strncmp("processor", line, 9) == 0)
        {
            static_cpu_data->logical_cpus++;
        }
        // model name
        if (strncmp("model name", line, 10) == 0 && strlen(static_cpu_data->model_name) == 0)
        {
            strncpy(static_cpu_data->model_name, line + 13, 128);
        }
        // pyhsical cores
        if (strncmp("cpu cores", line, 9) == 0 && static_cpu_data->physical_cores == 0)
        {
            static_cpu_data->physical_cores = atoi(line + 12);
        }
    };
    fclose(cpu_data_file);
}
void show_static_cpu_data(StaticCpuData *static_cpu_data, int max_cols)
{
    int x = max_cols / 2;
    mvprintw(0, x, "Model name: %s", static_cpu_data->model_name);
    mvprintw(1, x, "Logical CPUs: %d", static_cpu_data->logical_cpus);
    mvprintw(2, x, "Pyhiscal cores: %d", static_cpu_data->physical_cores);
}
