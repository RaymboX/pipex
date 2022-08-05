/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1_child.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mraymond <mraymond@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 12:29:25 by mraymond          #+#    #+#             */
/*   Updated: 2022/08/05 15:15:00 by mraymond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/pipex.h"

int	cmd_loop(t_vars *vars, char **argv)
{
	int		fct_ret;
	char	buf;

	fct_ret = pipe(vars->fd_pipe_err);
	if (fct_ret == -1)
		return (PIPE_ERR);
	vars->i_cmd = -1;
	buf = '0';
	vars->id = 1;
	while (++vars->i_cmd < vars->nb_cmd && vars->id != 0
		&& buf == '0')
	{
		fct_ret = fork_exec(vars, argv[vars->i_cmd + 2]);
		if (vars->id != 0 && fct_ret == 0)
			read(vars->fd_pipe_err[0], &buf, 1);
	}
	if (vars->id == 0)
		return (CHILD_RUNAWAY);
	if (buf != '0')
		return (EXECVE_FAIL);
	return (fct_ret);
}

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
	if (vars->id == 0)
		child_exec(vars, args);
	wait(0);
	close(vars->fd_pipe[1]);
	close(vars->fd_receiver);
	write(vars->fd_pipe_err[1], "0", 1);
	return (0);
}

void	child_exec(t_vars *vars, char *args)
{
	char	**exec_args;
	char	*path;

	exec_args = NULL;
	path = NULL;
	exec_args = args_parcing(args, exec_args);
	if (access(exec_args[0], F_OK) == 0)
		local_cmd_parcing(&path, &exec_args[0]);
	else
		path = find_file(vars->cmd_path, exec_args[0]);
	if (path)
	{
		dup2(vars->fd_giver, 0);
		dup2(vars->fd_receiver, 1);
		execve(path, exec_args, vars->envp);
	}
	free(path);
	free_dbl_ptr((void **)exec_args);
	write(vars->fd_pipe_err[1], "1", 1);
	close_all(vars);
	free_dbl_ptr((void **)vars->cmd_path);
	exit(1);
}

void	local_cmd_parcing(char **path, char **cmd)
{
	if (ft_strrchr_i(*cmd, '/') == -1)
		*path = ft_strjoin("./", *cmd);
	else
	{
		*path = *cmd;
		*cmd = *path + ft_strrchr_i(*path, '/') + 1;
	}
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
