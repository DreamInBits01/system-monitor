#include "core/cpu/helpers.h"

void parse_dynamic_cpu_data(char *line, void *data)
{
    DynamicCpuData *dynamic_cpu_data = (DynamicCpuData *)data;
    float total_mhz = 0;
    int mhz_occurrence = 0;
    if (strncmp("cpu MHz", line, 7) == 0)
    {
        float current_mhz;
        if (sscanf(line, "cpu MHz : %f", &current_mhz) == 1)
        {
            total_mhz += current_mhz;
            mhz_occurrence += 1;
        }
    };
    if (mhz_occurrence > 0)
    {
        float avg_mhz = (float)total_mhz / mhz_occurrence;
        dynamic_cpu_data->avg_mhz = avg_mhz;
    }
}
void parse_static_cpu_data(char *line, void *data)
{
    StaticCpuData *static_cpu_data = (StaticCpuData *)data;
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
}