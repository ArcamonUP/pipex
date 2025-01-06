/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 12:57:15 by kbaridon          #+#    #+#             */
/*   Updated: 2025/01/06 13:51:59 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include "libft.h"
#include "pipex.h"

void	error(char *msg)
{
	write(2, msg, ft_strlen(msg));
	perror("Error");
	return ;
}

void	end(char **cmd, int fd[2], int i)
{
	if (fd[1] != -1)
		close(fd[1]);
	if (!i)
	{
		if (fd[0] != -1)
			close(fd[0]);
	}
	free_tab(cmd);
	perror("Unable to open the files\n");
}

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}
