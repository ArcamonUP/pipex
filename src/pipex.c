/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 17:23:58 by kbaridon          #+#    #+#             */
/*   Updated: 2025/01/13 12:07:56 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "libft.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>

void	exec(char *cmd, t_data data, int fd[2], int i)
{
	char	**args;
	char	*path;

	args = ft_split(cmd, ' ');
	path = get_path(args[0], data.envp);
	if (!path)
	{
		free_tab(data.cmd);
		if (i == 1)
		{
			free(data.pid_tab);
			error("Execution failed.\n");
		}
		free_tab(args);
		if (fd)
			(close(fd[0]), close(fd[1]));
		if (data.fd[0])
			close(data.fd[0]);
		close(data.fd[1]);
		exit(EXIT_FAILURE);
	}
	execve(path, args, data.envp);
	free_tab(args);
	free(path);
	exit(EXIT_FAILURE);
}

pid_t	pre_exec(char *cmd, t_data data)
{
	pid_t	p;
	int		fd[2];

	if (pipe(fd) == -1)
		return (-1);
	p = fork();
	if (p == -1)
		return (-1);
	if (p == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		exec(cmd, data, fd, 0);
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
	}
	close(fd[0]);
	return (p);
}

int	last_exec(t_data data, int fd)
{
	pid_t	p;
	int		i;

	dup2(fd, STDOUT_FILENO);
	p = fork();
	if (p == -1)
	{
		free_tab(data.cmd);
		return (error("Execution failed."), 1);
	}
	i = 0;
	while (data.cmd[i + 1])
		i++;
	if (p == 0)
		exec(data.cmd[i], data, NULL, 1);
	wait_children(data, p);
	free_tab(data.cmd);
	return (0);
}

int	do_cmd(t_data data)
{
	int	i;
	int	x;
	int	y;

	i = 0;
	x = 0;
	if (data.fd[0] == -1)
	{
		x = 1;
		i = 1;
	}
	data.pid_tab = ft_calloc(sizeof(pid_t), data.ac - 3 - x);
	if (!data.pid_tab)
		return (end(data, 0), 1);
	while (data.cmd[i + 1])
	{
		data.pid_tab[i - x] = pre_exec(data.cmd[i], data);
		i++;
	}
	y = last_exec(data, data.fd[1]);
	if (data.fd[0] != -1)
		close(data.fd[0]);
	close(data.fd[1]);
	return (y);
}

int	main(int ac, char **av, char **envp)
{
	t_data	data;

	data.envp = envp;
	data.ac = ac;
	if (!init(ac, av, &data.cmd))
		return (1);
	if (ft_strncmp(av[1], "here_doc", 8) == 0)
	{
		data.fd[1] = open(av[ac - 1], O_WRONLY | O_APPEND | O_CREAT, 0644);
		data.fd[0] = -1;
		if (data.fd[1] == -1 || !exec_bonus(data))
			return (end(data, 1), 1);
	}
	else
	{
		data.fd[0] = open(av[1], O_RDONLY);
		data.fd[1] = open(av[ac - 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
		if (data.fd[0] == -1 || data.fd[1] == -1)
			return (end(data, 0), 1);
		dup2(data.fd[0], STDIN_FILENO);
	}
	return (do_cmd(data));
}
