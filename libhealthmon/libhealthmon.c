/*
 * ---------------------------------------------------------------------------
 * Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause-Clear
 * ---------------------------------------------------------------------------
 */

#include "libhealthmon.h"
char* memdetail(struct memory *mem_detail) {
#if DEBUG
	printf("\n*** Memory Details ***\n");
#endif
	size_t ret, len;
	char *outbuff = (char*)malloc(sizeof(char)*BUFFLEN);
	if(outbuff == NULL) {
		perror("Memory allocation failed\n");
		return NULL;
	}

	memset(outbuff, '\0', BUFFLEN);
	FILE *fp = popen(CMD_MEMTOTAL, "r");
	if(fp == NULL){
		perror("popen");
		return NULL;
		}
	ret = fread(outbuff, BUFFLEN-1, 1, fp);
	if(ret == -1){
		perror("fread");
		return NULL;
	}
#if DEBUG
	printf("%s", outbuff);
#endif
	strlcpy(mem_detail->mem_total, outbuff, ret+1);
	pclose(fp);
	memset(outbuff, '\0', BUFFLEN);

        fp = popen(CMD_MEMFREE, "r");
        if(fp == NULL){
                perror("popen");
		return NULL;
		}

        ret = fread(outbuff, BUFFLEN-1, 1, fp);
        if(ret == -1){
                perror("fread");
		return NULL;
		}
#if DEBUG
        printf("%s", outbuff);
#endif
	strlcpy(mem_detail->mem_free, outbuff, ret+1);
        pclose(fp);

        memset(outbuff, '\0', BUFFLEN);

        fp = popen(CMD_MEMAVAL, "r");
        if(fp == NULL){
                perror("popen");
		return NULL;
		}
        ret = fread(outbuff, BUFFLEN-1, 1, fp);
        if(ret == -1){
                perror("fread");
		return NULL;
		}
#if DEBUG
        printf("%s", outbuff);
#endif
	strlcpy(mem_detail->mem_aval, outbuff, ret+1);
        pclose(fp);

	memset(outbuff, '\0', BUFFLEN);

        fp = popen(CMD_MEMCACHE, "r");
        if(fp == NULL){
                perror("popen");
		return NULL;
		}
        ret = fread(outbuff, BUFFLEN-1, 1, fp);
        if(ret == -1){
                perror("fread");
		return NULL;
		}
	char *pos = strstr(outbuff, "SwapCached");
	len = pos - outbuff;
	outbuff[len] = '\0';
#if DEBUG
        printf("%s", outbuff);
#endif

	strlcpy(mem_detail->mem_cache, outbuff, len);
        pclose(fp);

	memset(outbuff, '\0', BUFFLEN);
	fp = popen(CMD_MEMANON, "r");
	if(fp == NULL){
		perror("popen");
		return NULL;
	}
	ret = fread(outbuff, BUFFLEN-1, 1, fp);
        if(ret == -1){
                perror("fread");
		return NULL;
		}
#if DEBUG
        printf("%s",outbuff);
#endif
	strlcpy(mem_detail->mem_anon,outbuff, ret+1);
        pclose(fp);

	memset(outbuff, '\0', BUFFLEN);
	fp = popen (CMD_MEMMAPPED, "r");
	if(fp == NULL){
		perror("popen");
		return NULL;
	}
	ret = fread(outbuff, BUFFLEN-1, 1, fp);
        if(ret == -1){
		perror("fread");
		return NULL;
		}
#if DEBUG
    printf("%s",outbuff);
#endif
        strlcpy(mem_detail->mem_mapp, outbuff, ret+1);
	pclose(fp);
	free(outbuff);

	unsigned long total_apps,total_kernel;
	total_apps = atoi (mem_detail->mem_anon) + atoi (mem_detail->mem_mapp);
	snprintf(mem_detail->mem_apps_usage, sizeof(mem_detail->mem_apps_usage), "%lu", total_apps);

	len=strlen(mem_detail->mem_apps_usage);
	mem_detail->mem_apps_usage[len]='\0';
	
	total_kernel = (atoi (mem_detail->mem_total)) -((atoi (mem_detail->mem_free)) +total_apps);
	snprintf(mem_detail->mem_kernel_usage, sizeof(mem_detail->mem_kernel_usage), "%lu", total_kernel);
	len=strlen(mem_detail->mem_kernel_usage);
        mem_detail->mem_kernel_usage[len]='\0';
    

#if DEBUG
	printf("Total Apps : %s",mem_detail->mem_apps_usage);
#endif

#if DEBUG
        printf("Total kernel : %s",mem_detail->mem_kernel_usage);
#endif
	
}

float cpudetail() {
#if DEBUG
	printf("*** CPU usage detail\n");
#endif

		int ret;
       float cpu_usage = -1.0f;
        char *outbuff = (char*)malloc(sizeof(char)*BUFFLEN);
	if(outbuff == NULL){
		perror("Memory allocation failed\n");
		return cpu_usage;
	}
		memset(outbuff, '\0',BUFFLEN);
        FILE *fp = popen(CMD_CPUDETAIL, "r");
        if(fp == NULL){
                perror("popen");
		return cpu_usage;
		}
	ret = fread(outbuff, BUFFLEN-1, 1, fp);
        if(ret == -1){
            perror("fread");
		return cpu_usage;
		}
	cpu_usage = 100-(atof(outbuff));
#if DEBUG
        printf("%.2f", cpu_usage);
#endif
        pclose(fp);
	free(outbuff);
	return cpu_usage;
}

char *starttime() {
#if DEBUG
	printf("\n*** System start time ***\n");
#endif

	int ret;
        char *outbuff = (char*)malloc(sizeof(char)*BUFFLEN);
	if(outbuff == NULL) {
		perror("Memory allocation failed\n");
		return NULL;
	}
		memset(outbuff, '\0', BUFFLEN);
        FILE *fp = popen(CMD_STIME, "r");
        if(fp == NULL){
                perror("popen");
		exit(EXIT_FAILURE);
        }
	ret = fread(outbuff, BUFFLEN-1, 1, fp);
        if(ret == -1){
                perror("fread");
		return NULL;
		}
#if DEBUG
        printf("%s", outbuff);
#endif
        pclose(fp);
	return outbuff;
}

char *pscount() {
#if DEBUG
	printf("\n*** Process count ***\n");
#endif

	int ret;
        char *outbuff = (char*)malloc(sizeof(char)*BUFFLEN);
	if(outbuff == NULL) {
		perror("Memory allocation failed\n");
		return NULL;
	}
		memset(outbuff, '\0', BUFFLEN);
        FILE *fp = popen(CMD_PSCOUNT, "r");
        if(fp == NULL){
                perror("popen");
		exit(EXIT_FAILURE);
        }
        ret = fread(outbuff, BUFFLEN-1, 1, fp);
        if(ret == -1){
                perror("fread");
		exit(EXIT_FAILURE);
        }
#if DEBUG
        printf("%s", outbuff);
#endif
        pclose(fp);
	return outbuff;
}

char *logdetail() {
#if DEBUG
	printf("\n *** Log detail ***\n");
#endif
	
	int ret;
        char *outbuff = (char*)malloc(sizeof(char)*BUFFLEN);
	if(outbuff == NULL) {
		perror("memory allocation failed\n");
		return NULL;
	}
	memset(outbuff, '\0', BUFFLEN);
        FILE *fp = popen(CMD_LOGDETAIL, "r");
        if(fp == NULL){
                perror("popen");
		exit(EXIT_FAILURE);
        }
	ret = fread(outbuff, BUFFLEN-1, 1, fp);
        if(ret == -1){
                perror("fread");
		exit(EXIT_FAILURE);
        }
#if DEBUG
        printf("%s", outbuff);
#endif
        pclose(fp);
	return outbuff;
}

char *osdetail() {

#if DEBUG
	printf("\n*** OS version info ***\n");
#endif
      char *line = NULL;
	  size_t str_len =0;
        char *outbuff = (char*)malloc(sizeof(char)*BUFFLEN);
	if(outbuff == NULL) {
		perror("Memory allocation failed\n");
		return NULL;
	}
		memset(outbuff, '\0', BUFFLEN);
        FILE *fp = popen(CMD_OSDETAIL, "r");
        if(fp == NULL){
                perror("popen");
		exit(EXIT_FAILURE);
        }
      
    while (getline(&line, &str_len, fp) != -1) {
        if (strstr(line, "VERSION=") != NULL) {
	    strlcpy(outbuff, line, BUFFLEN);
            break; // Stop reading further lines
        }
    }


#if DEBUG
        printf("%s", outbuff);
#endif
	pclose(fp);
	return outbuff;
}

char* rlog(void) {
#if DEBUG
	printf("\n*** Resetting kernel log ***\n");
#endif
	int ret;
	char *outbuff = (char*)malloc(sizeof(char)*BUFFLEN);
	if(outbuff == NULL) {
		perror("Memory allocation failed\n");
		return NULL;
	}
	memset(outbuff, '\0', BUFFLEN);

	FILE *fp = popen(CMD_RLOG, "r");
	if(fp == NULL){
                perror("popen");
		exit(EXIT_FAILURE);
        }
	ret = fread(outbuff, BUFFLEN, 1, fp);
        if(ret == -1)
                perror("fread");
	pclose(fp);
#if DEBUG
	printf("\n*** Restarting system log ***\n");
#endif
	fp = popen (CMD_SLOG,"r");
	if(fp == NULL){
                perror("popen");
		exit(EXIT_FAILURE);
        }
	
	ret = fread(outbuff, BUFFLEN, 1, fp);
        if(ret == -1)
                perror("fread");
	pclose(fp);
	free(outbuff);

}

unsigned long long nand_info(){
	unsigned long long tsize = 0;
        char *outbuff = (char*)malloc(sizeof(char)*BUFFLEN);
	if(outbuff == NULL) {
		perror("Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}
        memset(outbuff, '\0', BUFFLEN);
        FILE *fp = popen(CMD_NAND, "r");
	if(fp == NULL){
		perror("popen");
		exit(EXIT_FAILURE);
	}

	while(fgets(outbuff, BUFFLEN, fp)){
		unsigned long long size;
		if(sscanf(outbuff, "mtd%*d: %llx", &size) == 1){
			tsize += size;

		}
	}
	pclose(fp);
	free(outbuff);
#if DEBUG
	printf("Total NAND size:%llu bytes\n", tsize);
#endif
	return tsize;
}
