/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1_child.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mraymond <mraymond@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 12:29:25 by mraymond          #+#    #+#             */
/*   Updated: 2022/08/03 10:47:21 by mraymond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/pipex.h"

int	fork_exec(t_vars *vars, char *args)
{
	if (vars->i_cmd == 0)
		vars->fd_giver = vars->fd_file_in;
	else
		vars->fd_giver = vars->fd_pipe[0];
	if (vars->i_cmd + 1 == vars->nb_cmd)
		vars->fd_receiver = vars->fd_file_out;
	else
	{
		if (pipe(vars->fd_pipe) == -1)
			return (PIPE_EXEC);
		vars->fd_receiver = vars->fd_pipe[1];
	}
	vars->id = fork();
	if (vars->id != 0)
	{
		wait(0);
		close(vars->fd_pipe[1]);
		close(vars->fd_receiver);
		write(vars->fd_pipe_err[1], "0", 1);
	}
	else
	{
		child_exec(vars, args);
		return (1);
	}
	return (0);
}

void	child_exec(t_vars *vars, char *args)
{
	char	**exec_args;
	char	*cmd;

	exec_args = NULL;
	cmd = NULL;
	//close(vars->fd_pipe[0]);
	//close(vars->fd_pipe_err[0]);
	exec_args = args_parcing(args, exec_args);
	cmd = find_file(vars->cmd_path, exec_args[0]);
	if (cmd)
	{
		dup2(vars->fd_giver, 0);
		dup2(vars->fd_receiver, 1);
		execve(cmd, exec_args, NULL);
	}
	close(vars->fd_giver);
	close(vars->fd_receiver);
	free(cmd);
	free_dbl_ptr((void **)exec_args);
	write(vars->fd_pipe_err[1], "1", 1);
	close(vars->fd_pipe_err[1]);
}

char	*find_file(char **path, char *file)
{
	int		i;
	char	*path_cmd;
	char	*path_slash;

	i = -1;
	while (path[++i])
	{
		path_slash = ft_strjoin(path[i], "/");
		path_cmd = ft_strjoin(path_slash, file);
		free(path_slash);
		if (access(path_cmd, F_OK) == 0)
			return (path_cmd);
		free(path_cmd);
	}
	return (NULL);
}

char	**args_parcing(char *args, char **exec_args)
{
	int	i;

	in_quote_space_replace(args, 34);
	in_quote_space_replace(args, 39);
	exec_args = ft_split(args, ' ');
	i = -1;
	while (exec_args[++i])
		put_back_space(exec_args[i]);
	return (exec_args);
}

void	in_quote_space_replace(char *args, int quote)
{
	char	*ptr_quote;
	int		i;

	ptr_quote = ft_strchr(args, quote);
	if (ptr_quote != NULL)
	{
		i = 1;
		while (ptr_quote[i] && ptr_quote[i] != (char)quote)
		{
			if (ptr_quote[i] == ' ')
				ptr_quote[i] = (char)TEMP_SPACE_CHAR;
			i++;
		}
		if (!(ptr_quote[i]))
			put_back_space(args);
	}
}

void	put_back_space(char *args)
{
	int	i;

	i = -1;
	while (args[++i])
	{
		if (args[i] == (char)TEMP_SPACE_CHAR)
			args[i] = ' ';
	}
}
