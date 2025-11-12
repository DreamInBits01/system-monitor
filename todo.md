## Interactivity

- remove deleted process from processes list when killed
- to redraw the pad efficiently, just shift every y after the deleted process by -1
- Add viewports to processes's virtual pad

## Run options

- add refresh argument
- add help commmand to explain program

## Render

- Create a function to handle render when having static and dynamic data
  - cpu
- There's an issue with the process selection function, i think it's because the pad is starting at x + 3
  - The issue is happening because I'm refreshing the pad which creates new y for each process
  - I'm assigning the y only in the show_processes
  - So the used y is redundent
- need to delete the line height of the unseen process:DONE
- I think the issue with the selection is that there's a mismatch between the virtual.y and the pad's starting y (window.y + 3).
  - The y is starting from zero, and the pad is starting from window.y + 3
  - When i find search for a process, the line height and virtual.y are both starting from zero so it's okay
  - However, when i render the items, the y that's going to be used for the first time to show the item is zero, where in fact it should be 3
  - that creates an issue when i use the y that's starting from zero to show processes that are starting with a pad offest of 3
  - the solution is to match the search y with with the offest

## Memory management

- Create a block for each part of the ui (memory, cpu, processes): DONE
- Create an initialization function for each ui block
- Create a cleanup function for each ui block

## OPTIMIZATIONS

- cache proc files fds at the beginning:DONE
  - clean up opened files after:DONE
- write proc_file_read_and_parse && proc_dir_read_and_parse:DONE
  - refactor reading functions to use proc_file_read_and_parse && proc_dir_read_and_parse:DONE
- Cache uptime file and refactor it

## Systems considerations

## Refactor

- Refactor utils file
  - Most of the functions belong to the processes feature
