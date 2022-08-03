/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mraymond <mraymond@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 13:54:33 by mraymond          #+#    #+#             */
/*   Updated: 2022/08/03 11:29:33 by mraymond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# ifdef __linux__
#  include <sys/errno.h>
#  include <sys/wait.h>
#  include <sys/stat.h>
# endif /* linux */

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <errno.h>
# include "../libft/libft.h"

enum e_error
{
	ARG_INSUF,
	ENVP_PATH,
	OPEN_FILE1,
	OPEN_FILE2,
	EXECVE_FAIL,
	CHILD_RUNAWAY,
	PIPE_ERR,
	PIPE_EXEC
};

# define TEMP_SPACE_CHAR '\n'
# define CMD_PATH "/bin/"

typedef struct s_vars
{
	int		i_cmd;
	int		nb_cmd;
	int		id;
	int		fd_file_in;
	int		fd_file_out;
	int		fd_giver;
	int		fd_receiver;
	int		fd_pipe[2];
	int		fd_pipe_err[2];
	char	**cmd_path;
}				t_vars;

//0_pipex.c
int		open_file(t_vars *vars, int argc, char **argv);
void	close_all(t_vars *vars);
int		cmd_loop(t_vars *vars, char **argv);
int		error_message(t_vars *vars, int argc, char **argv, int error);

//1_child.c
int		fork_exec(t_vars *vars, char *args);
void	child_exec(t_vars *vars, char *args);
char	**args_parcing(char *args, char **exec_args);
void	in_quote_space_replace(char *args, int quote);
void	put_back_space(char *args);

int		pathfinder(t_vars *vars, char **envp);
char	*find_file(char **path, char *file);

#endif