#ifndef INTERACTIVITY_H
#define INTERACTIVITY_H
#include <signal.h>
#include <unistd.h>
#include "ui/render.h"
#include "context/index.h"
#include "utils.h"
#include "init.h"
void *interactivity_routine(void *data);
#endif