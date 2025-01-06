/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 17:23:56 by kbaridon          #+#    #+#             */
/*   Updated: 2025/01/06 15:38:25 by kbaridon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

int		init(int ac, char **av, char ***cmd);
char	**init_cmd(int ac, char **av);

int		exec_bonus(char **cmd, int fc[2]);
char	*get_path(char *cmd, char **envp);

void	free_tab(char **tab);
void	error(char *msg);
void	end(char **cmd, int fd[2], int i);

#endif
