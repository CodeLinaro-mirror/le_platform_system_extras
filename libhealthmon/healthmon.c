/*
 * ---------------------------------------------------------------------------
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 * ---------------------------------------------------------------------------
 */

#include "libhealthmon.h"


void print_help(void) {
        printf("Health-check Usage: \n"
                        "\tmemory\t Memory details\n"
			"\t\t\t RAM total\n"
			"\t\t\t RAM free\n"
			"\t\t\t RAM available\n"
			"\t\t\t RAM apps Usage\n"
			"\t\t\t RAM kernel Usage\n"
			"\tnand\t Total nand flash size\n"
                        "\tlog\t Syslog logging info\n"
                        "\tpscount\t Process count\n"
                        "\tcpu\t CPU usage details\n"
                        "\tsst\t\t System start/up time\n"
                        "\tosv\t\t OS version\n"
                        "\trlog\t Reset/clear logging\n"
                        "\thelp\t\t Print help options\n"
                        "\tversion\t Print utility version info\n");
	exit(0);
}

int main(int argc, char *argv[]){
	printf("SDX Health-check utility V%s\n", LIB_VERSION);
	if(argc < 2)
		print_help();

        struct memory mem_detail;
	unsigned long long nsize = 0;

        mem_detail.mem_total = (char*)malloc(sizeof(char)*50);
        mem_detail.mem_free = (char*)malloc(sizeof(char)*50);
        mem_detail.mem_aval = (char*)malloc(sizeof(char)*50);
        mem_detail.mem_cache = (char*)malloc(sizeof(char)*50);
	mem_detail.mem_anon = (char*)malloc(sizeof(char)*50);
	mem_detail.mem_mapp = (char*)malloc(sizeof(char)*50);
	mem_detail.mem_apps_usage = (char*)malloc(sizeof(char)*50);
	mem_detail.mem_kernel_usage = (char*)malloc(sizeof(char)*50);

	if(strcmp(argv[1], "memory") == 0){
	        printf("memory details :\n");
		memdetail(&mem_detail);
		printf("Memory Total : %skB\n", mem_detail.mem_total);
		printf("Memory Free : %skB\n", mem_detail.mem_free);
		printf("%s\n", mem_detail.mem_aval);
		printf("%s\n", mem_detail.mem_cache);
		printf("Total Apps Usage : %s kB\n", mem_detail.mem_apps_usage);
		printf("Total kernel usage : %s kB\n",mem_detail.mem_kernel_usage);
	} else if(strcmp(argv[1], "nand") == 0){
		nsize = nand_info();
		printf("Nand flash size : %llu bytes\n", nsize);
	} else if(strcmp(argv[1], "log") == 0){
	        printf("log detail : \n");
		char *out = logdetail();
                if(out != NULL)
                        printf("%s", out);
				free(out);
	} else if(strcmp(argv[1], "pscount") == 0){
		printf("process count : \n");
		char *out = pscount();
		if(out != NULL)
			printf("%s", out);
		free(out);
	} else if(strcmp(argv[1], "cpu") == 0){
		printf("Cpu Usage : ");
		float out = cpudetail();
			printf("%.2f%%\n", out);
	} else if(strcmp(argv[1], "sst") == 0){
		printf("Device start time : \n");
		char *out = starttime();
		if(out != NULL)
			printf("%s", out);
		free(out);
	} else if(strcmp(argv[1], "osv") == 0){
		printf("OS version : \n");
		char *out = osdetail();
		if(out != NULL)
			printf("%s", out);
		free(out);
	} else if(strcmp(argv[1], "rlog") == 0){
		printf("Resetting kernel log : \n");
		printf("Restarting system log : \n");
		rlog();
	} else if(strcmp(argv[1],"version")==0)
	{
	;	
        }
	else{
		
		print_help();
	}

	return 0;
}
