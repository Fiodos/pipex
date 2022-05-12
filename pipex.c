/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 14:41:31 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/05/12 15:21:26 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_pipe	*p;
	int		index;
	int		*current_pipe;
	int		*next_pipe;

	if (argc < 5)
		error_handler(1, NULL);
	p = lstnew();
	current_pipe = init_setup(argv, argc, p);
	index = 2;
	while (index < (argc - 1))
	{
		if (fork() == 0)
		{
			manage_c(current_pipe, next_pipe, index);
			exec_cmd(argv, argc, envp, index);
		}
		close_pipes(current_pipe, next_pipe, index);
		manage_p(&current_pipe, &next_pipe, index, &p);
		index++;
	}
	system("leaks pipex.c");
	return (0);
}
