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
