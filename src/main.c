#include "main.h"
/*
-Render thread
-Interactivity thread
*/

int main()
{
    initialize_ncurses();
    AppContext ctx = {0};
    initialize_context(&ctx);
    pthread_t interactivity_thread_id;
    pthread_t render_thread_id;

    // create threads
    pthread_create(&interactivity_thread_id, NULL, interactivity_routine, &ctx);
    pthread_create(&render_thread_id, NULL, render_routine, &ctx);

    // join thread
    pthread_join(interactivity_thread_id, NULL);
    pthread_join(render_thread_id, NULL);
    return 0;
}
