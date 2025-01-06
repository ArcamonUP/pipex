/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 16:33:33 by kbaridon          #+#    #+#             */
/*   Updated: 2025/01/06 15:23:43 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "libft.h"
#include <sys/wait.h>

void	write_lines(int fd[2], char *limiter, char **cmd)
{
	char	*line;
	size_t	len;

	close(fd[0]);
	len = ft_strlen(limiter);
	line = get_next_line(STDIN_FILENO);
	while (line)
	{
		if (ft_strncmp(line, limiter, len) == 0 && \
		(!line[len] || line[len] == '\n'))
		{
			free(line);
			get_next_line(-1);
			close(fd[1]);
			free_tab(cmd);
			exit(0);
		}
		write(fd[1], line, ft_strlen(line));
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
	close(fd[1]);
	free_tab(cmd);
}

int	exec_bonus(char **cmd, int fc[2])
{
	char	*limiter;
	__pid_t	p;
	int		fd[2];

	limiter = cmd[0];
	if (pipe(fd) == -1)
		return (error("Execution failed.\n"), 0);
	p = fork();
	if (p == -1)
		return (error("Execution failed.\n"), 0);
	if (p == 0)
	{
		close(fc[1]);
		write_lines(fd, limiter, cmd);
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		wait(NULL);
	}
	close(fd[0]);
	return (1);
}
