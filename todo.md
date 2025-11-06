## Interactivity

- remove deleted process from processes list when killed
- to redraw the pad efficiently, just shift every y after the deleted process by -1

## Run options

- add refresh argument
- add help commmand to explain program

## Render

- Create a function to handle render when having static and dynamic data
  - cpu

## Memory management

- Create a block for each part of the ui (memory, cpu, processes): DONE
- Create an initialization function for each ui block
- Create a cleanup function for each ui block

## OPTIMIZATIONS

- open used files once and store their fds at the beginning
  - clean up opened files after
