- remove deleted process from processes list when killed
- to redraw the pad efficiently, just shift every y after the deleted process by -1
- open used files once and store their fds at the beginning
- clean up opened files after
- Read process Name, and change read_process_destination name
- create one function that reads process stat,
  create sub functions to read different stats,
  feed sub functions the fd of the opened process to reduce overhead
