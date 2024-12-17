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

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum {
	TK_NOTYPE = 256,		//？？
	/* TODO: Add more token types */
	//Writed by Twantifred
	TK_EQ,		//==
	TK_NEQ,		//!=
	TK_AND,		//&&
	TK_OR,		//||
	TK_NUM,		//0|[0-9]
	TK_HEX,		//0x[1-9A-Fa-f]
	TK_REG,		//寄存器？？			这里还需要补充哦

	TK_NEGATIVE,	//负运算
	TK_DEREF,		//指针解引用
};

static struct rule {
  const char *regex;	//匹配字符
  int token_type;		//类型
} rules[] = {
	/* TODO: Add more rules.
	* Pay attention to the precedence level of different rules.
	*/
	//Writed by Twantifred
	// 空格
	{" +", 		TK_NOTYPE},    	//空格
	//	运算符号
	{"\\+", 	'+'},          	//加plus
	{"\\-", 	'-'}, 			//减 
	{"\\*", 	'*'}, 			//乘 
	{"\\/", 	'/'},			//除
	{"!",		'!'},			// ！
	//    ):
	{"\\(", 	'('}, 			//左括号
	{"\\)", 	')'}, 			//右括号
	//	双目运算符
	{"==",		TK_EQ},			//==
	{"!=",		TK_NEQ},		//!=
	{"&&",		TK_AND},		//&&
	{"\\|\\|",	TK_OR},			//||
	// 	数
	{"0x[1-9A-Fa-f][0-9A-Fa-f]*",		TK_HEX},	//十六进制
 	{"0|[1-9][0-9]*"			, 		TK_NUM},	//整数
	//	寄存器
	//！！！！
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;				//类型type用于记录具体的十进制整数
  char str[32];			//Token记录字符串
} Token;		

//
static Token tokens[32] __attribute__((used)) = {}; 	
//nr_token 指示已经被识别出的token数目
static int nr_token __attribute__((used))  = 0;			

static bool make_token(char *e){ 	//给出一个待求值表达式, make_token() 函数可以根据规则识别出其中的token,将token的信息依次记录到tokens数组中	 
	int position = 0;		//position 变量用来指示当前处理到的位置
	int i;
	regmatch_t pmatch;		

	nr_token = 0;

	//注意字符串复制的长度，以及末尾加\0
  	while (e[position] != '\0') {	
    /* Try all rules one by one. */
	// 按顺序尝试用不同的规则来匹配当前位置的字符串
		for (i = 0; i < NR_REGEX; i ++) 
		{
			if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) //regexec函数
			//关键的regexec函数
			{
				//一条规则匹配成功，并且匹配出的子串正好是 position 所在位置的时候，我们就成功地识别出一个token
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;
				//Log()宏会输出识别成功的信息
				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
								i,    rules[i].regex,   position, substr_len, substr_len, substr_start);
				position += substr_len;		//进入下一个位置
				/* TODO: 
				*  Now a new token is recognized with rules[i]. 
				*  Add codes to record the token in the array `tokens'. 
				*  For certain types of tokens, some extra actions should be performed.
				*/
				//	Writed by Twantifred
				//1. 先判断token长度是否大于32，大于32将不满足
				if(substr_len>32){
					printf("Error!!! At position %d Token's length>32. It's len=%d",
									position-substr_len, 	substr_len);
					assert(0);
					return false;
				}
				//2. 再跳过空格			
				if(rules[i].token_type==TK_NOTYPE)
					break;		
				//3. 逐一匹配ing  	
				tokens[nr_token].type=rules[i].token_type;	//类型赋值
				switch (rules[i].token_type) //运算符和括号直接用类型确定
				{
					case TK_NOTYPE:
						break;
					case TK_NUM:	//整数0-9
						strncpy(tokens[nr_token].str, substr_start, substr_len);
						//用strncpy函数提取匹配的字符
						*(tokens[nr_token].str+substr_len)='\0'; //确保结尾
						break;
					case TK_HEX:	//16进制以0x开头
						strncpy(tokens[nr_token].str,substr_start+2,substr_len-2);
						//用strncpy函数提取匹配的字符 +2目的是跳过0x
						*(tokens[nr_token].str+substr_len-2)='\0';//确保结尾
						break;
					default: 
						//TODO();
				}
				printf("成功匹配: nr_token=%d,type=%d,str=%s\n",
							nr_token,tokens[nr_token].type,tokens[nr_token].str);
				nr_token++;		//字符数嘉嘉 (:
				break;
			}
    	}

    if (i == NR_REGEX) {
		printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
		return false;
    }
  }

  return true;
}


word_t expr(char *e, bool *success) {
	//词法分析只负责拆分出单词，
	//由于减法a-b与负号-5、乘法*与指针解引用*ptr在做词法分析阶段是没有办法完成
	//故只能在该函数中通过遍历tokens数组来区分
	//通过单目运算符前面的一个单词的类型来确定-与*符号的类型。
	if (!make_token(e)) {
		*success = false;
		return 0;
	}
	/* TODO: Insert codes to evaluate the expression. */
	//TODO();
	//	Writed by Twantifred
	/*
	for(int i=0;i<nr_token;i++){
		//区分负号-与减法a-b
		if(tokens[i].type=='-'){
			
			if(i==0)
				tokens[i].type=TK_NEGATIVE;//若前一字符为空，则为负运算
			//若前一个为数或者）则为减法
			if(		tokens[i-1].type==TK_NUM
				||	tokens[i-1].type==TK_HEX
				||	tokens[i-1].type==')');
			
			
	else
	tokens[i].type=TK_NEGATIVE;//否则是负运算
	}
	if(tokens[i].type=='*'){//区分乘法与指针解引用
	if(i==0)
	tokens[i].type=TK_DEREF;//若前一字符为空，则为指针解引用
	if(tokens[i-1].type==TK_NUM||tokens[i
	1].type==TK_HEX||tokens[i-1].type==')');//若前一个为数或者）则为乘法
	else
	tokens[i].type=TK_DEREF;//否则是指针解引用
	}
	}
	*success=true;
	return eval(0,nr_token-1);//计算整个表达式
	*/
  	return 0;
}
