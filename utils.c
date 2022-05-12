/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 11:47:44 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/05/12 14:57:34 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error_handler(int i, char **argv)
{
	if (i == 1)
		perror("zsh: too few arguments");
	if (i == 2)
		perror("dup2() call failed");
	if (i == 3)
		perror("invalid command");
	if (i == 4)
		perror("malloc() failed");
	if (i == 6)
		perror("invalid null pointer argument");
	if (i == 7)
		perror("open() failed");
	if (i == 8)
		perror("close() failed");
	if (i == 9)
		perror (argv[1]);
	exit(1);
}

void	call_dup(char *c, int fd, int *current_pipe, int *next_pipe)
{
	if (!ft_strncmp(c, "co", 2))
	{
		if (dup2(current_pipe[fd], STDOUT_FILENO) == -1)
			error_handler(2, NULL);
	}
	else if (!ft_strncmp(c, "no", 2))
	{
		if (dup2(next_pipe[fd], STDOUT_FILENO) == -1)
			error_handler(2, NULL);
	}
	else if (!ft_strncmp(c, "ci", 2))
	{
		if (dup2(current_pipe[fd], STDIN_FILENO) == -1)
			error_handler(2, NULL);
	}
	else
	{
		if (dup2(next_pipe[fd], STDIN_FILENO) == -1)
			error_handler(2, NULL);
	}
}

char	**init_arg_lst(char **argv, int index)
{
	char	**lst;

	lst = ft_split(argv[index], ' ');
	return (lst);
}

void	exec_cmd(char *argv[], int argc, char *envp[], int index)
{
	int		file_fd;
	char	*path;

	path = get_path(argv, envp, index);
	if (path == NULL)
		error_handler(3, NULL);
	if ((index + 1) == (argc - 1))
	{
		file_fd = open(argv[argc - 1], O_RDWR | O_CREAT, 0777);
		if (file_fd == -1)
			error_handler(8, NULL);
		if (dup2(file_fd, STDOUT_FILENO) == -1)
			error_handler(2, NULL);
		execve(path, init_arg_lst(argv, index), NULL);
	}
	if (index == 2)
	{
		file_fd = open(argv[1], O_RDONLY, 0777);
		if (file_fd == -1)
			error_handler(9, argv);
		if (dup2(file_fd, STDIN_FILENO) == -1)
			error_handler(2, NULL);
		execve(path, init_arg_lst(argv, index), NULL);
	}
	execve(path, init_arg_lst(argv, index), NULL);
}

int	*init_setup(char **argv, int argc, t_pipe *p)
{
	if (p == NULL)
		error_handler(6, NULL);
	open(argv[argc - 1], O_RDWR | O_CREAT, 0777);
	create_pipes(&p, argc);
	pipe_iter(p, *open_pipes);
	return (get_pipe(p));
}
