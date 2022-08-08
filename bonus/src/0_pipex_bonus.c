/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   0_pipex_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mraymond <mraymond@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/02 12:27:59 by mraymond          #+#    #+#             */
/*   Updated: 2022/08/08 09:30:06 by mraymond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/pipex_bonus.h"

int	main(int argc, char **argv, char **envp)
{
	t_vars	vars;
	int		fct_ret;

	if (argc < 5)
		return (error_message(&vars, argc, argv, ARG_INSUF));
	if (pathfinder(&vars, envp) == ENVP_PATH)
		return (error_message(&vars, argc, argv, ENVP_PATH));
	vars.nb_cmd = argc - 3;
	fct_ret = open_file(&vars, argc, argv);
	if (fct_ret != 0)
		return (error_message(&vars, argc, argv, fct_ret));
	vars.envp = envp;
	fct_ret = cmd_loop(&vars, argv);
	close_all(&vars);
	free_dbl_ptr((void **)vars.cmd_path);
	if (fct_ret != 0)
		return (error_message(&vars, argc, argv, fct_ret));
	return (0);
}

int	pathfinder(t_vars *vars, char **envp)
{
	int	i;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!(envp[i]))
		return (ENVP_PATH);
	vars->cmd_path = ft_split(&envp[i][5], ':');
	if (vars->cmd_path == NULL)
		return (ENVP_PATH);
	i = -1;
	return (0);
}

int	open_file(t_vars *vars, int argc, char **argv)
{
	char	buf[100000];
	int		i;

	vars->fd_file_in = open(argv[1], O_RDONLY);
	if (vars->fd_file_in == -1)
		return (OPEN_FILE1);
	i = 0;
	while (i < 100 && read(vars->fd_file_in, &buf, 100000) > 0)
		i++;
	if (read(vars->fd_file_in, &buf, 1) != -1 && i < 100)
	{
		close(vars->fd_file_in);
		vars->fd_file_in = open(argv[1], O_RDONLY);
	}
	else
		return (FILE1_UNREADABLE);
	vars->fd_file_out = open(argv[argc - 1], O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (vars->fd_file_out == -1)
	{
		close(vars->fd_file_in);
		return (OPEN_FILE2);
	}
	return (0);
}

void	close_all(t_vars *vars)
{
	close(vars->fd_file_in);
	close(vars->fd_file_out);
	close(vars->fd_giver);
	close(vars->fd_receiver);
	close(vars->fd_pipe[0]);
	close(vars->fd_pipe[1]);
	close(vars->fd_pipe_err[0]);
	close(vars->fd_pipe_err[1]);
}

int	error_message(t_vars *vars, int argc, char **argv, int error)
{
	if (error == ARG_INSUF)
		printf("Missing arguments: ./pipex file1 cmd1 cmd2 file2\n");
	if (error == OPEN_FILE1 || error == OPEN_FILE2 || error == FILE1_UNREADABLE)
		close_all(vars);
	if (error == OPEN_FILE1)
		printf("Error opening file %s\n", argv[1]);
	if (error == OPEN_FILE2)
		printf("Error opening file %s\n", argv[argc - 1]);
	if (error == FILE1_UNREADABLE)
	{
		printf("File %s is unreadable or too big\n", argv[1]);
		close_all(vars);
		free_dbl_ptr((void **)vars->cmd_path);
		return (0);
	}
	if (error == EXECVE_FAIL)
		printf("Error execve on cmd %s\n", argv[vars->i_cmd + 1]);
	if (error == PIPE_ERR)
		printf("Error creating pipe for error management");
	if (error == PIPE_EXEC)
		printf("Error creating pipe for cmd %s\n", argv[vars->i_cmd + 2]);
	return (1);
}
