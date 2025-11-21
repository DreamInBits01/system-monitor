#include "core/cpu/helpers.h"
// void parse_cpu_data(char *line, void *data) {

// };
void parse_cpuinfo_line(char *line, void *data)
{
    CPUData *cpu_data = (CPUData *)data;
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
        cpu_data->avg_mhz = avg_mhz;
    }
    if (strncmp("processor", line, 9) == 0)
    {
        cpu_data->logical_cpus++;
    }
    // model name
    if (strncmp("model name", line, 10) == 0 && strlen(cpu_data->model_name) == 0)
    {
        strncpy(cpu_data->model_name, line + 13, 128);
    }
    // pyhsical cores
    if (strncmp("cpu cores", line, 9) == 0 && cpu_data->physical_cores == 0)
    {
        cpu_data->physical_cores = atoi(line + 12);
    }
}
void parse_cpu_cores_count(char *line, size_t *output)
{
    if (strncmp(line, "cpu", 3) == 0)
        *output = *output + 1;
}