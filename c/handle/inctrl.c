/*
	本程序实现简单的控制输入功能，提示用户输入，获取用户输入，
	判别用户输入执行并执行相应动作	
		while（打印提示用户的输入信息）
 			等待用户输入
			用户合法，执行相应操作
 */
#include <stdio.h>
#include <stdlib.h>

main()
{
	char c;

	printf("p表示打印，b...,f...,q表示退出，请输入：");
	while(c = getchar())	
		if(c == '\n')
			continue;
		else
		{
			switch(c)
			{
				case 'p':
					printf("p function\n");
					break;
				case 'b':
					printf("b function\n");
					break;
				case 'f':
					printf("f function\n");
					break;
				case 'q':
					exit(0);
				default:
					printf("请正确输入！\n");
			}
			printf("p表示打印，b...,f...,q表示退出，请输入：");
		}
}


