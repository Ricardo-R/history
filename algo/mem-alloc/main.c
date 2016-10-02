/*
 	内存分配方式：首次适应算法
	前提：存储容量大到一定限度，能适应本程序的的操作要求
	
		while（打印提示用户的输入信息）
 			等待用户输入
			用户合法，执行相应操作
 */
#include "mem.h"

main()
{
	char c;
	initalize();

	printf("p表示打印空闲空间块大小，b表示最佳适应法，f表示首次适应法,q表示退出，请输入：");
	while(c = getchar())	
		if(c == '\n')
			continue;
		else
		{
			switch(c)
			{
				case 'p':
					print();
					break;
				case 'b':
					bassign();
					break;
				case 'f':
					fassign();
					break;
				case 'q':
					exit(0);
				default:
					printf("请正确输入！\n");
			}
			printf("p表示打印空闲空间块大小，b表示最佳适应法，f表示首次适应法,q表示退出，请输入：");
		}
}


