/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 11:43:23 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/05/12 12:12:06 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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

void	close_pipes(int *current_pipe, int *next_pipe, int index)
{
	if (index % 2 == 0)
	{
		if (index != 2)
		{
			close(next_pipe[0]);
			close(next_pipe[1]);
		}
	}
	else
	{
		close(current_pipe[0]);
		close(current_pipe[1]);
	}
}
