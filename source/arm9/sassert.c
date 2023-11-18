// SPDX-License-Identifier: Zlib
// SPDX-FileNotice: Modified from the original version by the BlocksDS project.
//
// Copyright (C) 2013 Jason Rogers (Dovoto)
// Copyright (C) 2013 Michael Theall (mtheall)
// Copyright (C) 2023 Antonio Niño Díaz

// Simple routine to display assertion failure messages.

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <nds/arm9/input.h>
#include <nds/input.h>
#include <nds/interrupts.h>

void __sassert(const char *fileName, int lineNumber, const char *conditionString,
               const char *format, ...)
{
    (void) fileName;
    (void) lineNumber;
    (void) conditionString;
    (void) format;

    // Return an error code to the loader
    exit(-1);

    while (1)
        swiWaitForVBlank();
}
