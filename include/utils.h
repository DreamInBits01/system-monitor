#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>
#include "context/structs.h"
#include "core/processes/structs.h"
/**
 * find_proc_file_fd - Find cached FILE descriptor by key
 * @files: Array of cached proc files to search
 * @key: String identifier for the desired file
 *
 * Searches the cached proc files array for a non-directory entry matching
 * the given key. Only searches up to CACHED_PROC_FILES_NUMBER entries.
 *
 * Return: FILE pointer if found, NULL if not found or key matches a directory
 */
FILE *find_proc_file_fd(ProcFile *files, char *key);
/**
 * find_proc_dir_fd - Find cached directory descriptor by key
 * @files: Array of cached proc files to search
 * @key: String identifier for the desired directory
 *
 * Searches the cached proc files array for a directory entry matching
 * the given key. Only searches up to CACHED_PROC_FILES_NUMBER entries.
 *
 * Return: DIR pointer if found, NULL if not found or key matches a regular file
 */
DIR *find_proc_dir_fd(ProcFile *files, char *key);
/**
 * proc_dir_read_and_parse - Read directory entries and apply callback to each
 * @fd: Open directory stream to read from
 * @callback: Function to call for each directory entry, receives dirent and output
 * @output: User-defined data pointer passed to callback for collecting results
 *
 * Rewinds the directory to the beginning and iterates through all entries,
 * calling the provided callback function for each one. The callback can use
 * the output pointer to accumulate data or perform side effects.
 *
 * Return: true on success, false if fd is NULL
 */
bool proc_dir_read_and_parse(DIR *fd, void (*callback)(struct dirent *ep, void *output), void *output);
/**
 * proc_file_read_and_parse - Read file line-by-line and apply callback to each
 * @fd: Open file stream to read from
 * @callback: Function to call for each non-empty line, receives line and output
 * @output: User-defined data pointer passed to callback for collecting results
 *
 * Seeks to the beginning of the file and reads it line-by-line (max 256 chars
 * per line). Empty lines are skipped. For each non-empty line, the callback
 * is invoked with the line content and output pointer.
 *
 * Return: true on success, false if fd is NULL or seek fails
 */
bool proc_file_read_and_parse(FILE *fd, void (*callback)(char *line, void *output), void *output, size_t lines_to_skip);
#endif
