#include <stdio.h>

void matrix(int *p, int n, int m[][7], int s[][7])
{
	int i, r, j, k, t;
	for(r=2; r<=n; r++)
		for(i=1; i<=n-r+1; i++)
		{
			j = i+r-1;
			m[i][j] = m[i+1][j]+p[i-1]*p[i]*p[j];
			s[i][j] = i;
			for(k=i+1; k<j; k++)
			{
				t =m[i][k]+m[k+1][j]+p[i-1]*p[k]*p[j];
				if(t<m[i][j])
				{
					m[i][j] = t;
					s[i][j] = k;
				}
			}
		}

}

main()
{
	int i, j;
	int p[]={30, 35, 15, 5, 10, 20, 25};
	int m[7][7];
	int s[7][7];

	for(i=0;i<=6;i++)
		for(j=0; j<=6; j++)
		{
			m[i][j] = 0;
			s[i][j] = 0;
		}

	matrix(p, 6, m, s);

	for(i=0;i<=6;i++)
	{
		for(j=0; j<=6; j++)
			printf("\t%d\t", m[i][j]);
		printf("\n");
	}
	printf("-----------------------------------------------------------------------------------------------------------\n");
	for(i=0;i<=6;i++)
	{
		for(j=0; j<=6; j++)
			printf("\t%d\t", s[i][j]);
		printf("\n");
	}
}

