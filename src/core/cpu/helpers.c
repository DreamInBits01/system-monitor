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
        char *token = strtok(line + 13, " ");
        while (token != NULL)
        {
            if (strncmp(token, "Gen", 3) != 0 && strncmp(token, "Intel(R)", 8) != 0 && strncmp(token, "Core(TM)", 8) != 0)
            {
                strcat(cpu_data->model_name, token);
                strcat(cpu_data->model_name, " ");
            }
            token = strtok(NULL, " ");
        }
        //-1 for starting from zero and -1 for last added " "
        cpu_data->model_name[strlen(cpu_data->model_name) - 2] = '\0';
    }
    // pyhsical cores
    if (strncmp("cpu cores", line, 9) == 0 && cpu_data->physical_cores == 0)
    {
        cpu_data->physical_cores = atoi(line + 12);
    }
}
void parse_procstat_cpu_line(char *line, void *data)
{
    CPUData *cpu_data = (CPUData *)data;
    if (strncmp("cpu", line, 3) == 0)
    {
        int index;
        sscanf(line, "cpu%d", &index);
        cpu_data->cores[index].usage = index;
    }
}
void parse_cpu_cores_count(char *line, void *output)
{
    size_t *count = (size_t *)output;
    if (strncmp(line, "cpu", 3) == 0)
        *count = *count + 1;
}