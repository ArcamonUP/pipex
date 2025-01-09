/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 17:23:56 by kbaridon          #+#    #+#             */
/*   Updated: 2025/01/09 14:44:37 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

typedef struct t_data
{
	char	**cmd;
	char	**envp;
	int		fd[2];
}	t_data;

int		init(int ac, char **av, char ***cmd);
char	**init_cmd(int ac, char **av);

int		exec_bonus(t_data data);
char	*get_path(char *cmd, char **envp);

void	free_tab(char **tab);
void	error(char *msg);
void	end(t_data data, int i);

#endif
