/***************************************************************************************
* Copyright (c) 2014-2024 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <isa.h>
#include <cpu/cpu.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "sdb.h"

#include <memory/paddr.h>//加上这个读内存

static int is_batch_mode = false;

void init_regex();
void init_wp_pool();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}


static int cmd_c(char *args) {
  cpu_exec(-1);
  
  return 0;
}

static int cmd_q(char *args) {
  nemu_state.state = NEMU_QUIT;     //change on 10 Dec      Writed by Twtf
  return -1;
}


static int cmd_si(char *args) {         //Writed by Twtf

	uint32_t step;
	//printf("%s",args);//测试信息
	if(args==NULL)						//if NULL 
        step=1;							//defaults to 1
	else{
		int get_args = sscanf(args, "%d", &step);
		if(get_args<=0)		//未收到具有step
		{
			printf("Error!!! Enter: si (number)!!! \n");
			return 0;
		}	
	}
	printf("Equivalent to:cpu_exec(%d)\t\tWrited by Twantifred\n",step);
	cpu_exec(step);
	return 0;
}

static int cmd_info(char *args) {         //Writed by Twtf
  	
	if(args == NULL)
	{
		printf("Error!!! Enter: info r(registers) or info w(watchpoints) to info!!!\r\n");
		return 0;
	}
	//printf("Print register status or monitoring point information\n");//测试信息
	 
	char info_option[80];
	sscanf(args, "%s", info_option);
	if(strcmp(info_option,"r") == 0)
	{
		printf("Print register status\t\tWrited by Twantifred\n");
		isa_reg_display();//----->nemu/src/isa/riscv32/reg.c   --->>function
	}
	else if(strcmp(info_option,"w") == 0)
	{
		printf("Print monitoring point information\t\tWrited by Twantifred\n");
		//isa_wpoint_display();
	}
	else{
		printf("Error!!! Please enter: info r(registers) or w(watchpoints)!!!\r\n");
	}
	return 0;
}

static int cmd_x(char *args) {         //Writed by Twtf
  	
	if(args == NULL)//若x后无参数
	{
		printf("Error!!! Enter: x N EXPR !!!\r\n");
		return 0;
	}
	char *argsN = strtok(args, " ");	//用strtoks函数分解args,得到args1=“N/0EXPR”
	uint32_t N=atoi(argsN);		//长度
	if(N==0)//若x后参数为空或N位置不是数字 
	{
		printf("Error!!! No N !!! Enter: x N EXPR !!!\r\n");
		return 0;
	}
	char* EXPR = strtok(NULL," ");		//用strtoks函数得到EXPR
	if(EXPR == NULL)//若x后只有一个参数N 缺少EXPR
	{
		printf("Error!!! No EXPR !!! Enter: x N EXPR !!!\r\n");
		return 0;
	}
	if( strtok(NULL," ") != NULL)//若x后有三个参数以及以上参数
	{
		printf("Error!!! Too many parameters !!! Enter: x N EXPR !!!\r\n");
		return 0;
	}

	char *endptr;
    vaddr_t addr =  strtol( EXPR ,&endptr, 16);
	if (*endptr != '\0') 
	{
        printf("Error!!! EXPR is error!!!\n");
		return 0;
    }
	//vaddr_t addr;	//地址
	//sscanf(EXPR,"%x",&addr);
	
	printf("N=%d,EXPR=%s\t\tWrited by Twantifred\n",N,EXPR);//测试信息
	printf("addr=0x%x   \t\tWrited by Twantifred\n",addr);//测试信息

    //正常执行:
    for(int i = 0 ; i < N ; i++){
        uint32_t data = paddr_read(addr+i*4, 4);
        printf("0x%08x:", addr+ i*4 ); 
        for(int j =0 ; j < 4 ; j++){
            //printf("0x%02x " , data);
			printf("0x%02x " , data & 0xff); //取最低两位
            data = data >> 8 ;
        }
        printf("\n");
    } 
	return 0;
}

static int cmd_help(char *args);

///////////////////////////////////////////////
static struct {
  const char *name;
  const char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display information about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },

  /* TODO: Add more commands */
  { "si", "Pause the program after stepping through N instructions. When N is not given, it defaults to 1", cmd_si },   //Writed by Twtf
  { "info", "Print register status or monitoring point information. ", cmd_info },   //Writed by Twtf
  { "x", "Scan the memory from EXPR by N bytes. ", cmd_x },   //Writed by Twtf

};
///////////////////////////////////////////////////
#define NR_CMD ARRLEN(cmd_table)

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void sdb_set_batch_mode() {
  is_batch_mode = true;
}

void sdb_mainloop() {					//接收命令
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef CONFIG_DEVICE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}

void init_sdb() {
  /* Compile the regular expressions. */
  init_regex();     //编译正则表达式

  /* Initialize the watchpoint pool. */
  init_wp_pool();   //初始化监视点池
}
