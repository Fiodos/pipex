/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fyuzhyk <fyuzhyk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 11:45:02 by fyuzhyk           #+#    #+#             */
/*   Updated: 2022/05/14 15:46:14 by fyuzhyk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*ft_strcpy(char *dest, char *src)
{
	int	i;

	if (dest == NULL)
		return (NULL);
	if (src == NULL)
		return (dest);
	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strcat(char *dest, char *src)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(dest);
	while (src[i])
	{
		dest[len + i] = src[i];
		i++;
	}
	dest[len + i] = '\0';
	return (dest);
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
				error_handler(4, NULL);
			ft_strcpy(envp_path, envp[i]);
			return (envp_path);
		}
		i++;
	}
	return (NULL);
}

char	*init_path(char *element, char *cmd)
{
	char	*path;

	path = malloc(sizeof(char) * ft_strlen(cmd) + ft_strlen(element) + 2);
	if (path == NULL)
		error_handler(4, NULL);
	path = ft_strcpy(path, element);
	path = ft_strcat(path, "/");
	path = ft_strcat(path, cmd);
	return (path);
}

char	*get_path(char *argv[], char *envp[], int index)
{
	int		i;
	char	*path;
	char	**cmd;
	char	*envp_path;
	char	**full_path;

	i = 0;
	envp_path = get_envp_path(envp);
	full_path = ft_split(envp_path + 5, ':');
	cmd = ft_split(argv[index], ' ');
	while (full_path[i])
	{
		path = init_path(full_path[i], cmd[0]);
		if (access(path, F_OK) != -1)
		{
			return (path);
		}
		else
			free(path);
		i++;
	}
	return (NULL);
}
