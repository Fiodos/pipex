/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/01 14:37:39 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/05/11 13:36:02 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h> // are we allowed to use it?
#include "libft/libft.h"


#define CMD_NOT_FOUND perror("command not found")

typedef struct t_pipes
{
	int pipes[2];
	struct t_pipes *next;
} t_pipe;

t_pipe	*lstlast(t_pipe *lst)
{
	t_pipe	*curr;

	curr = lst;
	if (curr == NULL)
		return (curr);
	while (curr->next != NULL)
		curr = curr->next;
	return (curr);
}

void	lstadd_back(t_pipe **head, t_pipe *new)
{
	t_pipe	*last;

	if (*head == NULL)
	{
		*head = new;
		new->next = NULL;
		return ;
	}
	last = lstlast(*head);
	last->next = new;
	new = lstlast(new);
	new->next = NULL;
}

t_pipe	*lstnew(void)
{
	t_pipe	*new_node;

	new_node = malloc(sizeof(t_pipe));
	if (new_node == NULL)
		return (NULL);
	new_node->next = NULL;
	return (new_node);
}

int	*get_pipe(t_pipe *pipe)
{
	int	*result;

	if (pipe == NULL)
		return (0);
	result = pipe->pipes;
	return (result);
}

void	create_pipes(t_pipe **header, int argc)
{
	int	i;

	i = 0;
	while (i < (argc - 3))
	{
		lstadd_back(header, lstnew());
		i++;
	}
}

void	open_pipes(t_pipe *p)
{
	if (p == NULL)
		perror("open_pipes: null pointer argument");
	if (pipe(p->pipes) == -1)
		strerror(errno);
}

void	pipe_iter(t_pipe *pipe, void (*open_pipes)(t_pipe *))
{
	t_pipe	*curr;

	curr = pipe;
	while (curr != NULL)
	{
		open_pipes(curr);
		curr = curr->next;
	}
}

void	call_dup(char *c, int fd, int *current_pipe, int *next_pipe)
{
	if (!ft_strncmp(c, "co", 2))
	{
		if (dup2(current_pipe[fd], STDOUT_FILENO) == -1)
			strerror(errno);
	}
	else if (!ft_strncmp(c, "no", 2))
	{
		if (dup2(next_pipe[fd], STDOUT_FILENO) == -1)
			strerror(errno);
	}
	else if (!ft_strncmp(c, "ci", 2))
	{
		if (dup2(current_pipe[fd], STDIN_FILENO) == -1)
			strerror(errno);
	}
	else
	{
		if (dup2(next_pipe[fd], STDIN_FILENO) == -1)
			strerror(errno);
	}
}

void	manage_pipes_child(int *current_pipe, int *next_pipe, int index)
{
	if (index % 2 == 0)
	{
		call_dup("co", 1, current_pipe, next_pipe);
		close(current_pipe[1]);
		close(current_pipe[0]);
		if (index > 2)
		{
			call_dup("ni", 0, current_pipe, next_pipe);
			close(next_pipe[1]);
			close(next_pipe[0]);
		}
	}
	else
	{
		call_dup("ci", 0, current_pipe, next_pipe);
		close(current_pipe[0]);
		close(current_pipe[1]);
		if (index >= 3)
		{
			call_dup("no", 1, current_pipe, next_pipe);
			close(next_pipe[1]);
			close(next_pipe[0]);
		}
	}
}

void	mange_pipes_parent(int **current_pipe, int **next_pipe, int index, t_pipe **p)
{
	if (index % 2 == 0)
	{
		*p = (*p)->next;
		*next_pipe = get_pipe(*p);
	}
	else
	{
		*p = (*p)->next;
		*current_pipe = get_pipe(*p);
	}
}

void	close_pipes(int *current_pipe, int *next_pipe, int index)
{
	if (index % 2 == 0)
	{
		if (index != 2)
		{
			if ((close(next_pipe[0])) == -1 && (close(next_pipe[1]) == -1))
				strerror(errno);
		}
	}
	else
	{
			if ((close(current_pipe[0])) == -1 && (close(current_pipe[1]) == -1))
				strerror(errno);
	}
}

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
			if (envp_path == NULL)
				perror("get_envp_path: malloc failed"); // like this, or is return (NULL) in such a case enough?
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
	if (path == NULL)
		perror("init_path: malloc failed");
	path = strcpy(path, element);
	path = strcat(path, "/");
	path = strcat(path, cmd);
	return (path);
}

char	*get_path(char *argv[], char *envp[], int index)
{
	int		i;
	char	*path;
	char	**cmd;
	char	**full_path;

	i = 0;
	full_path = ft_split((get_envp_path(envp) + 5), ':');
	cmd = ft_split(argv[index], ' ');
	while (full_path[i])
	{
		path = init_path(full_path[i], cmd[0]);
		if (access(path, F_OK) != -1)
			return (path);
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

int	exec_cmd(char *argv[], int argc, char *envp[], int index)
{
	int		file_fd;
	char	*path;

	path = get_path(argv, envp, index);
	if (path == NULL)
	{
		// perror("invalid command");
		return (1);
	}
	if ((index + 1) == (argc - 1))
	{
		if ((file_fd = open(argv[index + 1], O_WRONLY, 0777)) == -1)
			strerror(errno);
		if (dup2(file_fd, STDOUT_FILENO) == -1)
			strerror(errno);
		execve(path, init_arg_lst(argv, index), NULL);
	}
	if (index == 2)
	{
		if ((file_fd = open(argv[1], O_RDONLY, 0777)) == -1)
			strerror(errno);
		if (dup2(file_fd, STDIN_FILENO) == -1)
			strerror(errno);
		execve(path, init_arg_lst(argv, index), NULL);
	}
	execve(path, init_arg_lst(argv, index), NULL);
	return (0);
}

int main(int argc, char **argv, char **envp)
{
	t_pipe	*p;
	int		id;
	int		index;
	int		*current_pipe;
	int		*next_pipe;

	p = lstnew();
	create_pipes(&p, argc);
	pipe_iter(p, *open_pipes);
	current_pipe = get_pipe(p);
	index = 2;
	if (argc < 5)
		perror("main: too few arguments");
	while (index < (argc - 1))
	{
		id = fork();
		if (id == 0)
		{
			manage_pipes_child(current_pipe, next_pipe, index);
			exec_cmd(argv, argc, envp, index);
		}
		close_pipes(current_pipe, next_pipe, index);
		mange_pipes_parent(&current_pipe, &next_pipe, index, &p);
		index++;
	}
}
// need to handle input of non-existing commands;
// check out whether you need to write a clear function, e.g if split or create_pipes fails?
// in general take a look at how to check for mem leaks;
