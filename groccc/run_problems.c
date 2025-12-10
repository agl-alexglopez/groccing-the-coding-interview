/** Author: Alexander Lopez
===========================
The problem harness is meant to be run on the problems/ folder, not specific
problems For running specific problems, you can run the binaries directly in the
build/[debug]/bin/problems/ folder.

Point the executable at the problems as follows:

.build/[path to]/run_problems build/[debug]/bin/problems/

The much easier way is to run the Makefile command:

In debug mode:

make debug-problems

In release mode:

make release-problems

The path will be different to the run_problems executable depending on the build
being build/debug/bin or build/bin.

This program runs the problems as a child process so that we can also accept a
report from the problem program on its own determination of success. Each child
will return a problem status as its exit code. A pass is 0 and failure is
non-zero, currently set as 1 to be POSIX compliant.

Running children also gives us a chance to catch surprise crashes or segfaults
while still being able to run subsequent problems. Most programmer errors will
trigger some sort of OS level failure that we can handle as the parent. This way
we get as much information from all problems on their failures as possible.

See the test generation framework for what API the problems agree to use. */
#ifdef __linux__
#    include <linux/limits.h>
#    define FILESYS_MAX_PATH PATH_MAX
#endif
#ifdef __APPLE__
#    include <sys/syslimits.h>
#    define FILESYS_MAX_PATH NAME_MAX
#endif

#include <dirent.h>
#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "str_view/str_view.h"

#include "loggers.h"
#include "test_case_generator.h"

struct Path_bin
{
    SV_Str_view path;
    SV_Str_view bin;
};

static char const *const pass_mark = "⬤";
static char const *const fail_mark = "X";
static char const *const err_message
    = "problem process was unexpectedly killed.";
static char const *const unclassified_message
    = "problem process exited with status outside of normal range.";

static enum TCG_Tests_status run(SV_Str_view);
static enum TCG_Tests_status run_problem_process(struct Path_bin);
static DIR *open_problem_dir(SV_Str_view);
static bool fill_path(char[FILESYS_MAX_PATH], SV_Str_view, SV_Str_view);

int
main(int argc, char **argv)
{
    if (argc == 1)
    {
        return 0;
    }
    SV_Str_view arg_view = SV_from_terminated(argv[1]);
    return run(arg_view);
}

static enum TCG_Tests_status
run(SV_Str_view const problems_dir)
{
    DIR *dir_pointer = open_problem_dir(problems_dir);
    if (!dir_pointer)
    {
        return TCG_TESTS_ERROR;
    }
    char absolute_path[FILESYS_MAX_PATH];
    size_t problems_ran = 0;
    size_t problems_passed = 0;
    struct dirent const *d;
    while ((d = readdir(dir_pointer)))
    {
        SV_Str_view const entry = SV_from_terminated(d->d_name);
        if (SV_starts_with(entry, SV_from("."))
            || SV_starts_with(entry, SV_from("..")))
        {
            continue;
        }
        fill_path(absolute_path, problems_dir, entry);
        printf("%s(%s%s", LOG_CYAN, SV_begin(entry), LOG_NONE);
        (void)fflush(stdout);
        enum TCG_Tests_status const res = run_problem_process((struct Path_bin){
            SV_from_terminated(absolute_path),
            entry,
        });
        switch (res)
        {
            case TCG_TESTS_ERROR:
                logerr("\n%s%s%s\n%s %s%s%s)%s\n", LOG_RED, err_message,
                       LOG_CYAN, SV_begin(entry), LOG_RED, fail_mark, LOG_CYAN,
                       LOG_NONE);
                break;
            case TCG_TESTS_PASS:
                logout(" %s%s%s)%s\n", LOG_GREEN, pass_mark, LOG_CYAN,
                       LOG_NONE);
                break;
            case TCG_TESTS_FAIL:
                logout("\n%s%s%s)%s\n", LOG_RED, fail_mark, LOG_CYAN, LOG_NONE);
                break;
            default:
                logerr("\n%s%s%s\n%s %s%s%s)%s\n", LOG_RED,
                       unclassified_message, LOG_CYAN, SV_begin(entry), LOG_RED,
                       fail_mark, LOG_CYAN, LOG_NONE);
                break;
        }
        if (res == TCG_TESTS_PASS)
        {
            ++problems_passed;
        }
        ++problems_ran;
    }
    enum TCG_Tests_status const result
        = problems_passed == problems_ran ? TCG_TESTS_PASS : TCG_TESTS_FAIL;
    (void)closedir(dir_pointer);
    return result;
}

enum TCG_Tests_status
run_problem_process(struct Path_bin pb)
{
    if (SV_is_empty(pb.path))
    {
        logerr("No problem provided.\n");
        return TCG_TESTS_ERROR;
    }
    pid_t const problem_proc = fork();
    if (problem_proc == 0)
    {
        (void)execl(SV_begin(pb.path), SV_begin(pb.bin), NULL);
        logerr("Child problem process could not start.\n");
        return TCG_TESTS_ERROR;
    }
    int status = 0;
    if (waitpid(problem_proc, &status, 0) < 0)
    {
        logerr("Error running problem: %s\n", SV_begin(pb.bin));
        return TCG_TESTS_ERROR;
    }
    if (WIFSIGNALED(status))
    {
        int const sig = WTERMSIG(status);
        char const *const message = strsignal(sig);
        if (message)
        {
            logerr("%sProcess killed with signal %d: %s%s\n", LOG_RED, sig,
                   message, LOG_NONE);
        }
        else
        {
            logerr("%sProcess killed with signal %d: unknown signal code%s\n",
                   LOG_RED, sig, LOG_NONE);
        }
    }
    if (!WIFEXITED(status))
    {
        logerr("%sProcess did not exit.%s", LOG_RED, LOG_NONE);
        return TCG_TESTS_ERROR;
    }
    return WEXITSTATUS(status);
}

static DIR *
open_problem_dir(SV_Str_view problems_folder)
{
    if (SV_is_empty(problems_folder)
        || SV_len(problems_folder) > FILESYS_MAX_PATH)
    {
        logerr("Invalid input to path to problem executables %s\n",
               SV_begin(problems_folder));
        return NULL;
    }
    DIR *dir_pointer = opendir(SV_begin(problems_folder));
    if (!dir_pointer)
    {
        logerr("Could not open directory %s\n", SV_begin(problems_folder));
        return NULL;
    }
    return dir_pointer;
}

static bool
fill_path(char path_buf[FILESYS_MAX_PATH], SV_Str_view problems_dir,
          SV_Str_view entry)
{
    size_t const dir_bytes = SV_fill(FILESYS_MAX_PATH, path_buf, problems_dir);
    if (FILESYS_MAX_PATH - dir_bytes < SV_bytes(entry))
    {
        logerr("Relative path exceeds FILESYS_MAX_PATH?\n%s", path_buf);
        return false;
    }
    (void)SV_fill(FILESYS_MAX_PATH - dir_bytes, path_buf + SV_len(problems_dir),
                  entry);
    return true;
}
