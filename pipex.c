/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/01 14:37:39 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/05/01 18:40:34 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include "libft/libft.h"

char	*get_envp_path(char *envp[])
{
	int		i;
	char	*envp_path;

	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], "PATH", ft_strlen("PATH")))
		{
			envp_path = malloc(sizeof(char) * ft_strlen(envp[i]) + 1);
			strcpy(envp_path, envp[i]);
			return (envp_path);
		}
		i++;
	}
	return (NULL); // because path could not be found;
}

char	*init_path(char *element, char *cmd)
{
	char	*path;

	path = malloc(sizeof(char) * ft_strlen(cmd) + ft_strlen(element) + 2);
	path = strcpy(path, element);
	path = strcat(path, "/");
	path = strcat(path, cmd);
	return (path);
}

char	*get_path(char *argv[], char *envp[], int index)
{
	int		i;
	int		fd;
	char	*path;
	char	**cmd;
	char	**full_path;

	i = 0;
	full_path = ft_split((get_envp_path(envp) + 5), ':');
	cmd = ft_split(argv[index], ' ');
	while (full_path[i])
	{
		fd = 0;
		path = init_path(full_path[i], cmd[0]);
		if ((fd = open(path, O_RDONLY)) != -1)
		{
			close(fd); // check if it still works after this addition;
			return (path);
		}
		else
			free(path);
		i++;
	}
	return (NULL); // because in this case there isn't any valid path;
}

char	**init_arg_lst(char **argv, int index)
{
	char	*args;
	char	**lst;

	args = malloc(sizeof(char) * ft_strlen(argv[index]) + ft_strlen(argv[index + 1]) + 2);
	args = strcpy(args, argv[index]);
	args = strncat(args, " ", sizeof(char));
	args = strncat(args, argv[index + 1], ft_strlen(argv[index + 1]));
	lst = ft_split(args, ' ');
	return (lst);
}

int main(int argc, char **argv, char *envp[])
{
	int			index;
	int			id;
	int			pipe_fd[2];
	int			pipe_index[2];
	char		**arg_list;
	char		*test = malloc(sizeof(char) * 100);
	char		*path;

	index = 1;
	pipe(pipe_fd);
	pipe(pipe_index);
	id = fork();
	if (id == 0) // child process;
	{
		id = fork();
		if (id == 0) // child's child process; // aka the "first" process;
		{
			// dup2(pipe_index[1], 1);
			// index = 3;
			// write(1, &index, sizeof(index));
			dup2(pipe_fd[1], 1);
			execve(get_path(argv, envp, index), init_arg_lst(argv, index), NULL);
		}
		wait(NULL);
		index = index + 2;
		int test_fd = open("test", O_RDWR | O_CREAT, 0777);
		if (read(pipe_fd[0], test, 100) == -1)
		{
			printf("read call failed");
			return (1);
		}
		// read(pipe_index[0], &index, sizeof(char));
		ft_putstr_fd(test, test_fd);
		// char	*arg_list_2[] = {"wc", "-c", "test", NULL};
		arg_list = init_arg_lst(argv, index);
		path = get_path(argv, envp, index);
		id = open(argv[4], O_WRONLY);
		dup2(id, 1);
		execve(path, arg_list, NULL);
	}
	wait(NULL);
	unlink("test");
	return (0);
}
