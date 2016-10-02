#include "unp.h"

void *transfer (void *ptr)
{
	int fd, nbytes;
	char buf[BUFF], store[20];
	FILE *fp;
	Exchange *Exptr;

	Exptr = (Exchange *)ptr;
	fd = Exptr->sockfd;

	if (*Exptr->flag == SEND)
	{
		if ((fp = fopen (Exptr->FileName, "rb")) < 0)
		{
			printf (" Open %s failed\n", Exptr->FileName);
			*Exptr->flag = OPEN_NO_PERMIT;
			close (fd);
			pthread_exit (NULL);
		}
		printf ("open %s OK\n", Exptr->FileName);
		while (1)
		{
			bzero (buf, sizeof(buf));
			if ((nbytes = fread (buf, 1, BUFF, fp)) < 0)
			{
				printf ("read error: %s\n", strerror(errno));
				*Exptr->flag = READ_ERROR;
				fclose (fp);
				close (fd);
				pthread_exit (NULL);
			}
		/*	
			strncpy (store, buf, nbytes);
			store[nbytes] = '\0';
			printf ("\nSEND store: %s\n", store);
		*/
			if (writen (fd, buf, nbytes) < 0)
			{
				printf ("writen error: %s\n", strerror(errno));
				*Exptr->flag = WRITE_ERROR;
				fclose (fp);
				close (fd);
				pthread_exit (NULL);
			}
			if (nbytes == 0 || nbytes < BUFF)
			{
				*Exptr->flag = SEND_SUCC;
				fclose (fp);
				close (fd);
				pthread_exit (NULL);
			}
		}
	}
	else	//*Exptr->flag == RECV
	{
		if ((fp = fopen (Exptr->FileName, "wb")) < 0)
		{
			printf (" create %s failed\n", Exptr->FileName);
			*Exptr->flag = CREAT_NO_PERMIT;
			close (fd);
			pthread_exit (NULL);
		}
		while (1)
		{
			bzero (buf, BUFF);
			if ((nbytes = read (fd, buf, BUFF)) < 0)
			{
				printf ("read error: %s\n", strerror(errno));
				*Exptr->flag = READ_ERROR;
				fclose(fp);
				close (fd);
				pthread_exit (NULL);
			}
		/*	
			strncpy (store, buf, nbytes);
			store[nbytes] = '\0';
			printf (" RECV store: %s\n", store);
		*/
			if (nbytes == 0)
			{
				*Exptr->flag = RECV_SUCC;
				fclose(fp);
				close (fd);
				pthread_exit (NULL);
			}
			if (fwrite (buf, 1, nbytes, fp) < 0)
			{
				printf ("write error: %s\n", strerror(errno));
				*Exptr->flag = WRITE_ERROR;
				fclose(fp);
				close (fd);
				pthread_exit (NULL);
			}
		}
	}
}
