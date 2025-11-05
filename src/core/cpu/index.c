#include "core/cpu/index.h"

void read_dynamic_cpu_data(DynamicCpuData *data)
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
        data->avg_mhz = avg_mhz;
    }
    fclose(cpu_data_file);
}
void show_dynamic_cpu_data(CPUBlock *data)
{
    mvwprintw(data->window.itself, 4, 1, "Avg Mhz:%.2f", data->dynamic_data.avg_mhz);
}
void read_static_cpu_data(StaticCpuData *data)
{
    FILE *cpu_data_file = fopen("/proc/cpuinfo", "r");
    if (cpu_data_file == NULL)
    {
        printf("Error while reading cpu_data_file\n");
        exit(1);
    }
    char line[256];
    data->logical_cpus = 0;
    while (fgets(line, sizeof(line), cpu_data_file))
    {
        // logical CPUs
        if (strncmp("processor", line, 9) == 0)
        {
            data->logical_cpus++;
        }
        // model name
        if (strncmp("model name", line, 10) == 0 && strlen(data->model_name) == 0)
        {
            strncpy(data->model_name, line + 13, 128);
        }
        // pyhsical cores
        if (strncmp("cpu cores", line, 9) == 0 && data->physical_cores == 0)
        {
            data->physical_cores = atoi(line + 12);
        }
    };
    fclose(cpu_data_file);
}
void show_static_cpu_data(CPUBlock *data)
{
    mvwprintw(data->window.itself, 1, 1, "Model name: %s", data->static_data.model_name);
    mvwprintw(data->window.itself, 2, 1, "Logical CPUs: %d", data->static_data.logical_cpus);
    mvwprintw(data->window.itself, 3, 1, "Pyhiscal cores: %d", data->static_data.physical_cores);
}
void update_cpu_block(CPUBlock *data)
{
    show_static_cpu_data(data);
    show_dynamic_cpu_data(data);
    wattron(data->window.itself, COLOR_PAIR(4));
    box(data->window.itself, 0, 0);
    wattroff(data->window.itself, COLOR_PAIR(4));
    wattron(data->window.itself, A_BOLD);
    mvwaddstr(data->window.itself, 0, 2, "CPU");
    wattroff(data->window.itself, A_BOLD);
    wrefresh(data->window.itself);
}