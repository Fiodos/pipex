/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/01 14:37:39 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/05/04 10:46:15 by fyuzhyk          ###   ########.fr       */
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
	char	**lst;

	lst = ft_split(argv[index], ' ');
	return (lst);
}


void	exec_cmd(char *argv[], int argc, char *envp[], int index)
{
	int	file_fd;

	if ((index + 1) == (argc - 1))
	{
		file_fd = open(argv[index + 1], O_WRONLY, 0777);
		dup2(file_fd, STDOUT_FILENO);
		execve(get_path(argv, envp, index), init_arg_lst(argv, index), NULL);
	}
	if (index == 2)
	{
		file_fd = open(argv[1], O_RDONLY, 0777);
		dup2(file_fd, STDIN_FILENO);
		execve(get_path(argv, envp, index), init_arg_lst(argv, index), NULL);
	}
	execve(get_path(argv, envp, index), init_arg_lst(argv, index), NULL);
}

int main(int argc, char **argv, char **envp)
{
	int	id;
	int pipe_fd[2];
	int	index;

	index = 2;
	pipe(pipe_fd);
	id = fork();
	if (id == 0)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		exec_cmd(argv, argc, envp, index);
	}
	wait(NULL);
	close(pipe_fd[1]);
	index++;
	dup2(pipe_fd[0], STDIN_FILENO);
	exec_cmd(argv, argc, envp, index);
}
