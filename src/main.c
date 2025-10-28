#include "main.h"
/*
-Render thread
-Interactivity thread
*/

int main(int argc, char **argv)
{
    initialize_ncurses();
    AppContext *ctx = malloc(sizeof(AppContext));
    memset(ctx, 0, sizeof(AppContext));
    initialize_context(ctx);
    pthread_t interactivity_thread_id;
    pthread_t render_thread_id;

    // create threads
    pthread_create(&render_thread_id, NULL, render_routine, ctx);
    pthread_create(&interactivity_thread_id, NULL, interactivity_routine, ctx);

    // join thread
    pthread_join(interactivity_thread_id, NULL);
    pthread_join(render_thread_id, NULL);

    // cleanup
    cleanup_context(ctx);
    cleanup_ncurses();
    return 0;
}
