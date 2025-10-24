#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void exec(char buff[10])
{
	int count = 0;
	for (int i = 0; buff[i]; i++)
	{
		if (buff[i] == '1')
			count++;
	}
	char **array = malloc(sizeof(char *) * (count + 2));

	array[0] = "./ft_ls";
	array[count + 1] = NULL;
	char flags[] = "lRartufgd";
	char *ptr = &buff[0];
	char *ptr2 = &flags[0];
	while (ptr[0] && ptr[0] != '1')
	{
		ptr++;
		ptr2++;
	}
	for (int i = 1; i < count + 1; i++)
	{
		array[i] = malloc(3);
		array[i][0] = '-';
		array[i][1] = ptr2[0];
		array[i][2] = 0;
		while (ptr[0] && ptr[0] != '1')
		{
			ptr++;
			ptr2++;
		}
	}
	for (int i = 0; array[i]; i++)
		printf("%s", array[i]);
	execve("./ft_ls", array, NULL);
	exit(0);
}

int main()
{
	char buff[10] = {0};

	for (int i = 0; i < 512; i++)
	{
		int bytes = 256;
		int tmp = i;
		for (int j = 0; j < 9; j++)
		{
			buff[j] = tmp / bytes + 48;
			tmp >= bytes ? tmp -= bytes : tmp;
			bytes /= 2;
		}
		pid_t pid = fork();
		if (pid == 0)
			exec(buff);
		else
			waitpid(pid, NULL, 0);
		printf("next %d\n", i);
	}
	exit(0);
	return (0);
}