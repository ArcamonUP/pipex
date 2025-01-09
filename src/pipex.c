/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 17:23:58 by kbaridon          #+#    #+#             */
/*   Updated: 2025/01/09 15:09:38 by kbaridon         ###   ########.fr       */
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
			error("Execution failed.\n");
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

void	pre_exec(char *cmd, t_data data)
{
	pid_t	p;
	int		fd[2];

	if (pipe(fd) == -1)
		return ;
	p = fork();
	if (p == -1)
		return ;
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
		waitpid(p, NULL, 0);
	}
	close(fd[0]);
	return ;
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
	waitpid(p, NULL, 0);
	free_tab(data.cmd);
	return (0);
}

int	do_cmd(t_data data)
{
	int	i;
	int	y;

	i = 0;
	if (data.fd[0])
		i = 1;
	while (data.cmd[i + 1])
		pre_exec(data.cmd[i++], data);
	y = last_exec(data, data.fd[1]);
	if (data.fd[0])
		close(data.fd[0]);
	close(data.fd[1]);
	return (y);
}

int	main(int ac, char **av, char **envp)
{
	t_data	data;

	data.envp = envp;
	if (!init(ac, av, &data.cmd))
		return (1);
	if (ft_strncmp(av[1], "here_doc", 8) == 0)
	{
		data.fd[1] = open(av[ac - 1], O_RDWR | O_TRUNC);
		data.fd[0] = 0;
		if (data.fd[1] == -1 || !exec_bonus(data))
			return (end(data, 1), 1);
	}
	else
	{
		data.fd[0] = open(av[1], O_RDWR);
		data.fd[1] = open(av[ac - 1], O_RDWR | O_TRUNC | O_CREAT, 0666);
		if (data.fd[0] == -1 || data.fd[1] == -1)
			return (end(data, 0), 1);
		dup2(data.fd[0], STDIN_FILENO);
	}
	return (do_cmd(data));
}
