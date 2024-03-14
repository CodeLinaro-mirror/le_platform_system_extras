/*
 * ---------------------------------------------------------------------------
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 * ---------------------------------------------------------------------------
 */

#ifndef _LIBMEMHEALTH_H_
#define _LIBMEMHEALTH_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <time.h>
#include <ifaddrs.h>
#include <sys/stat.h>
#include <sys/types.h>

#define LIB_VERSION "1.0"
#define CMD_MEMTOTAL "grep 'MemTotal' /proc/meminfo | awk '{print $2}'"
#define CMD_MEMFREE  "grep 'MemFree' /proc/meminfo | awk '{print $2}'"
#define CMD_MEMAVAL  "grep 'MemAvailable' /proc/meminfo"
#define CMD_MEMCACHE "grep 'Cached' /proc/meminfo"
#define CMD_MEMANON  "grep 'AnonPages' /proc/meminfo | awk '{print $2}'"
#define CMD_MEMMAPPED "grep 'Mapped' /proc/meminfo | awk '{print $2}'"

#define CMD_NAND  "cat /proc/mtd"
#define CMD_CPUDETAIL   "mpstat | tail -1 | awk '{print $12}'"
#define CMD_STIME    "uptime"
#define CMD_PSCOUNT  "ps | wc -l"
#define CMD_LOGDETAIL   "grep 'log_size' /etc/config/system"
#define CMD_OSDETAIL    "cat /etc/os-release"
#define CMD_RLOG    "dmesg -c"
#define CMD_SLOG    "service log restart"
#define BUFFLEN  400
#define DEBUG 0

struct memory {
        char *mem_total;
        char *mem_free;
        char *mem_aval;
        char *mem_cache;
	char *mem_anon;
	char *mem_mapp;
	char *mem_apps_usage;
	char *mem_kernel_usage;
};

void memdetail(struct memory *);
float cpudetail();
char *starttime();
char *logdetail();
char *pscount();
char *osdetail();
void rlog(void);
unsigned long long nand_info(void );

#endif
