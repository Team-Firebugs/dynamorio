/* **********************************************************
 * Copyright (c) 2014-2015 Google, Inc.  All rights reserved.
 * Copyright (c) 2003-2008 VMware, Inc.  All rights reserved.
 * **********************************************************/

/*
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of VMware, Inc. nor the names of its contributors may be
 *   used to endorse or promote products derived from this software without
 *   specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL VMWARE, INC. OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 */

/*
 * test of execve
 */

#include "tools.h"

#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h> /* for wait and mmap */
#include <sys/wait.h>  /* for wait */
#include <assert.h>
#include <stdio.h>
#include <string.h>

/***************************************************************************/

int
main(int argc, char *argv[])
{
    pid_t child;
    if (argc < 2) {
        print("ERROR: not enough args\n");
        return -1;
    }

    if (find_dynamo_library())
        print("parent is running under DynamoRIO\n");
    else
        print("parent is running natively\n");
    child = fork();
    if (child < 0) {
        perror("ERROR on fork");
    } else if (child > 0) {
        pid_t result;
        result = waitpid(child, NULL, 0);
        assert(result == child);
        print("child has exited\n");
    } else {
        int result;
        const char *arg[3];
        arg[0] = argv[1];
        arg[1] = "/fake/path/it_worked";
        arg[2] = NULL;
        if (find_dynamo_library())
            print("child is running under DynamoRIO\n");
        else
            print("child is running natively\n");
        result = execv(argv[1], (char **)arg);
        if (result < 0)
            perror("ERROR in execv");
    }
    return 0;
}
