/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 12:57:56 by kbaridon          #+#    #+#             */
/*   Updated: 2025/01/06 14:08:05 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "pipex.h"

int	file_access(char *filename)
{
	if (access(filename, F_OK) < 0 || access(filename, R_OK) < 0 \
	|| access(filename, W_OK) < 0)
		return (1);
	return (0);
}

char	**init_cmd(int ac, char **av)
{
	char	**cmd;
	int		i;

	i = 0;
	cmd = ft_calloc(sizeof(char *), ac - 2);
	if (!cmd)
		return (NULL);
	while (av[i + 1])
	{
		cmd[i] = ft_strdup(av[i]);
		if (!cmd[i])
			return (free_tab(cmd), NULL);
		i++;
	}
	return (cmd);
}

int	init(int ac, char **av, char ***cmd)
{
	if (ac < 5)
		return (error("Missing parameters.\n"), 0);
	if (ft_strncmp(av[1], "here_doc", 8) == 0 && ac < 6)
		return (error("Missing paramaters.\n"), 0);
	if ((file_access(av[1]) || ft_strncmp(av[1], "here_doc", 8) == 0) \
	&& file_access(av[ac - 1]))
		return (error("Missing permissions on the files given.\n"), 0);
	*cmd = init_cmd(ac, av + 2);
	if (!(*cmd))
		return (error("Failed to init cmd.\n"), 0);
	return (1);
}
