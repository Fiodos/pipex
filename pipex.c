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

int main(int argc, char **argv, char **envp)
{
	int		i;
	int		fd;
	char	**result;
	char	*test;
	char	*cmd;
	char	*paths;
	char	*arg_list[] = {argv[1], argv[2], NULL};

	i = 0;
	cmd = malloc(sizeof(char) * ft_strlen(argv[1]) + 1);
	cmd = strcpy(cmd, argv[1]); // the command;
	paths = malloc(sizeof(char) * ft_strlen(envp[1]) + 1);
	paths = strncat(paths, (envp[1] + 5), ft_strlen(envp[1]) - 5); //PATHs without "PATH=";
	result = ft_split(paths, ':');

	// printf("%s\n", envp[1]);
	// while (result[i])
	// {
	// 	printf("%s\n", result[i]);
	// 	i++;
	// }

	while (result[i])
	{
		// printf("%s\n", result[i]);
		fd = 0;
		test = malloc(sizeof(char) * ft_strlen(cmd) + ft_strlen(result[i]) + 5);
		test = strcpy(test, result[i]);
		test = strcat(test, "/");
		test = strcat(test, cmd);
		// printf("test: %s\n", test);
		if ((fd = open(test, O_RDONLY)) != -1)
		{
			printf("the right path is: %s\n", test);
			break;
		}
		else
			free(test);
		i++;
	}
	//printf("the right path is: %s", test);
	execve(test, arg_list, NULL);
	return (0);
}
