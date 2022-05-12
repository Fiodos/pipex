/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 12:03:52 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/05/12 14:58:30 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdio.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <string.h>
# include <stdlib.h>
# include "libft/libft.h"

typedef struct t_pipes
{
	int				pipes[2];
	struct t_pipes	*next;
}	t_pipe;

char	*ft_strcpy(char *dest, char *src);

char	*ft_strcat(char *dest, char *src);

void	error_handler(int i, char **argv);

t_pipe	*lstlast(t_pipe *lst);

void	lstadd_back(t_pipe **head, t_pipe *new_pipe);

t_pipe	*lstnew(void);

int		*get_pipe(t_pipe *pipe);

void	create_pipes(t_pipe **header, int argc);

void	open_pipes(t_pipe *p);

void	pipe_iter(t_pipe *pipe, void (*open_pipes)(t_pipe *));

void	call_dup(char *c, int fd, int *current_pipe, int *next_pipe);

void	manage_c(int *current_pipe, int *next_pipe, int index);

void	manage_p(int **current_pipe, int **next_pipe, int index, t_pipe **p);

void	close_pipes(int *current_pipe, int *next_pipe, int index);

char	*get_envp_path(char *envp[]);

char	*init_path(char *element, char *cmd);

char	*get_path(char *argv[], char *envp[], int index);

char	**init_arg_lst(char **argv, int index);

void	exec_cmd(char *argv[], int argc, char *envp[], int index);

int		*init_setup(char **argv, int argc, t_pipe *p);

#endif
