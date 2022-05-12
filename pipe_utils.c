/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 11:38:47 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/05/12 14:59:26 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
	if (pipe(p->pipes) == -1)
		error_handler(7, NULL);
}

int	*get_pipe(t_pipe *pipe)
{
	int	*result;

	if (pipe == NULL)
		return (0);
	result = pipe->pipes;
	return (result);
}

void	manage_p(int **current_pipe, int **next_pipe, int index, t_pipe **p)
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

void	manage_c(int *current_pipe, int *next_pipe, int index)
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
