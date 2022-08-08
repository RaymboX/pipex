/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_arg_parcing_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mraymond <mraymond@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/05 15:10:15 by mraymond          #+#    #+#             */
/*   Updated: 2022/08/08 09:30:16 by mraymond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/pipex_bonus.h"

char	**args_parcing(char *args, char **exec_args)
{
	int	i;

	in_quote_space_replace(args, SINGLE_QUOTE);
	in_quote_space_replace(args, DOUBLE_QUOTE);
	exec_args = ft_split(args, ' ');
	i = -1;
	while (exec_args[++i])
	{
		put_back_space(exec_args[i]);
		exec_args[i] = remove_backslash_in_quote(exec_args[i]);
		exec_args[i] = remove_outside_quote(exec_args[i]);
	}
	return (exec_args);
}

char	*remove_backslash_in_quote(char *arg)
{
	int		i;
	char	*temp[4];

	if (arg[0] == SINGLE_QUOTE || arg[0] == DOUBLE_QUOTE)
	{
		i = 0;
		while (arg[i])
		{
			if (arg[i] == BACKSLASH)
			{
				temp[0] = ft_substr(arg, 0, i - 1);
				temp[1] = ft_substr(arg, i + 1, ft_strlen(arg) - i);
				temp[2] = arg;
				temp[3] = NULL;
				arg = ft_strjoin(temp[0], temp[1]);
				free_dbl_ptr((void **)temp);
			}
			else
				i++;
		}
	}
	return (arg);
}

char	*remove_outside_quote(char *arg)
{
	int		len;
	char	*arg_no_quote;

	len = ft_strlen(arg);
	if ((arg[0] == SINGLE_QUOTE || arg[0] == DOUBLE_QUOTE)
		&& arg[len - 1] == arg[0])
	{
		arg_no_quote = ft_substr(arg, 1, len - 2);
		free(arg);
		return (arg_no_quote);
	}
	return (arg);
}

void	in_quote_space_replace(char *args, int quote)
{
	int	quote_position[2];
	int	i;

	quote_position[0] = ft_strchr_i(args, quote);
	if (quote_position[0] != -1)
	{
		quote_position[1] = ft_strrchr_i(args, quote);
		if (quote_position[1] != quote_position[0])
		{
			i = quote_position[0];
			while (++i < quote_position[1])
			{
				if (args[i] == ' ')
					args[i] = (char)TEMP_SPACE_CHAR;
			}
		}
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
