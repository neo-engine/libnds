// SPDX-License-Identifier: Zlib
// SPDX-FileNotice: Modified from the original version by the BlocksDS project.
//
// Copyright (C) 2020 Gericom

#ifndef LIBTEAK_CPU_H__
#define LIBTEAK_CPU_H__

// TODO: Document

void cpuDisableIrqs(void);
void cpuEnableIrqs(void);

void cpuDisableInt0(void);
void cpuEnableInt0(void);

void cpuDisableInt1(void);
void cpuEnableInt1(void);

void cpuDisableInt2(void);
void cpuEnableInt2(void);

void cpuDisableVInt(void);
void cpuEnableVInt(void);

#endif // LIBTEAK_CPU_H__
