/*
	本程序实现简单的控制输入功能，提示用户输入，获取用户输入，
	判别用户输入执行并执行相应动作	
		while（打印提示用户的输入信息）
 			等待用户输入
			用户合法，执行相应操作
 */
#include "bt.h"

main()
{
	char c;

	printf("p手动输入创建二叉树节点，n自动创建二叉树，f先序遍历，m中序遍历，l后序遍历,q表示退出，请输入：");
	while(c = getchar())	
		if(c == '\n')
			continue;
		else
		{
			switch(c)
			{
				case 'p':
					create(TURE);
					break;
				case 'n':
					create(FALSE);
					break;
				case 'f':
					visit(FIRST);
					break;
				case 'm':
					visit(MIDDLE);
					break;
				case 'l':
					visit(LAST);
					break;
				case 'q':
					exit(0);
				default:
					printf("请正确输入！\n");
			}
		printf("p手动输入创建二叉树节点，n自动创建二叉树，f先序遍历，m中序遍历，l后序遍历,q表示退出，请输入：");
		}
}


