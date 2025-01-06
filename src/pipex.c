/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 17:23:58 by kbaridon          #+#    #+#             */
/*   Updated: 2025/01/06 15:23:16 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "libft.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>

char	*get_path(char *cmd, char **envp)
{
	char	**path;
	char	*result;
	char	*temp;
	int		i;

	i = 0;
	while (envp[i] && ft_strncmp("PATH=", envp[i], 5) != 0)
		i++;
	if (!envp[i])
		return (NULL);
	path = ft_split(envp[i] + 5, ':');
	i = 0;
	while (path[i])
	{
		temp = ft_strjoin(path[i], "/");
		result = ft_strjoin(temp, cmd);
		free(temp);
		if (access(result, F_OK) == 0)
		{
			free_tab(path);
			return (result);
		}
		(free(result), i++);
	}
	return (free_tab(path), NULL);
}

int	exec(char *cmd, char **envp)
{
	char	**args;
	char	*path;

	args = ft_split(cmd, ' ');
	path = get_path(args[0], envp);
	if (!path)
	{
		free_tab(args);
		return (0);
	}
	execve(path, args, envp);
	free_tab(args);
	free(path);
	return (1);
}

int	pre_exec(char *cmd, char **envp)
{
	__pid_t	p;
	int		fd[2];

	if (pipe(fd) == -1)
		return (0);
	p = fork();
	if (p == -1)
		return (0);
	if (p == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		return (exec(cmd, envp));
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

void	last_exec(int fd[2], char **cmd, char **envp)
{
	int		p;
	int		i;

	p = fork();
	if (p == -1)
	{
		free_tab(cmd);
		return (error("Failed to fork process\n"));
	}
	i = 0;
	while (cmd[i + 1])
		i++;
	if (p == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		exec(cmd[i], envp);
		error("Last execution failed.\n");
		free_tab(cmd);
	}
	wait(NULL);
	free_tab(cmd);
}

int	main(int ac, char **av, char **envp)
{
	char	**cmd;
	int		fd[2];
	int		i;

	if (!init(ac, av, &cmd))
		return (0);
	if (ft_strncmp(av[1], "here_doc", 8) == 0)
	{
		fd[1] = open(av[ac - 1], O_RDWR | O_TRUNC);
		if (fd[1] == -1 || !exec_bonus(cmd, fd))
			return (end(cmd, fd, 1), 0);
		i = 1;
	}
	else
	{
		fd[0] = open(av[1], O_RDWR | O_TRUNC);
		fd[1] = open(av[ac - 1], O_RDWR | O_TRUNC);
		if (fd[0] == -1 || fd[1] == -1)
			return (end(cmd, fd, 0), 0);
		dup2(fd[0], STDIN_FILENO);
		i = 0;
	}
	while (cmd[i + 1])
	{
		if (!pre_exec(cmd[i++], envp))
		{
			free_tab(cmd);
			error("Execution failed.\n");
			break ;
		}
	}
	last_exec(fd, cmd, envp);
	if (ft_strncmp(av[1], "here_doc", 8) != 0)
		close(fd[0]);
	close(fd[1]);
}
