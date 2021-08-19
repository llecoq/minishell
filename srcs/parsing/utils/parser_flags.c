/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_flags.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llecoq <llecoq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/14 16:00:50 by abonnel           #+#    #+#             */
/*   Updated: 2021/08/18 15:30:01 by llecoq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/*---------------------------------------------------------------------------*/
/*------------------- FLAG ASSIGNATION---------------------------------------*/
/*---------------------------------------------------------------------------*/

// We first initialize flags values by default to -1 bc it will 
// allow us to make sure they have all been properly set
// Then we set each to 1 or 0


static void	initialize_flags_values(t_token **cmd_array)
{
	int			i;
	t_token		*cpy;

	i = 0;
	while (cmd_array[i])
	{
		cpy = cmd_array[i];
		while (cpy)
		{
			cpy->cmd = -1;
			cpy->arg = -1;
			cpy->redir = -1;
			// cpy->fd = 0;
			cpy->error = 0;
			cpy = cpy->next;
		}
		i++;
	}
}

void	set_redir_arg_flags(t_token **cmd_array)
{
	int			i;
	int			redir_type;
	t_token		*cpy;
	
	initialize_flags_values(cmd_array);
	i = 0;
	while (cmd_array[i])
	{
		cpy = cmd_array[i];
		while (cpy)
		{
			redir_type = is_redirection(cpy->word, 0);
			if (redir_type >= IS_REDIR)
				turn_on_flag(redir_type, cpy);
			else
				turn_on_flag(ARG, cpy);
			cpy = cpy->next;
		}
		i++;
	}
}

static char	*create_error_str(char *next_token)
{
	char		*error_str;

	error_str = calloc(ft_strlen(next_token) + 3, sizeof(char));
	error_str[0] = '`';
	ft_strlcat(error_str, next_token, ft_strlen(next_token) + 3);
	error_str[ft_strlen(next_token) + 1] = '\'';
	return (error_str);
}

static char	*check_after_redir(t_token *cpy, t_token *next_cmd)
{
	if (cpy->redir == PIPE && next_cmd == NULL)
		return (create_error_str("newline"));
	else if (cpy->redir != PIPE)
	{
		if (cpy->next == NULL)
			return (create_error_str("newline"));
		else if (cpy->next->redir >= IS_REDIR)
			return (create_error_str(cpy->next->word));
	}
	return (NULL);
}

void	set_next_flag(t_token *token, int redir_type)
{
	if (redir_type == TRUNC)
		turn_on_flag(IS_FILE, token);//next token is redir
	else if (redir_type == APPEND)
		turn_on_flag(IS_FILE, token);
	else if (redir_type == HEREDOC)
		turn_on_flag(STOP_VALUE, token);
	else if (redir_type == INPUT_REDIR)
		turn_on_flag(IS_FILE, token);
}

void	set_flag_after_redirection(t_token **cmd_array, char **error_str)
{
	int			i;
	t_token		*cpy;
	
	i = 0;
	while (cmd_array[i])
	{
		cpy = cmd_array[i];
		while (cpy)
		{
			if (cpy->redir >= IS_REDIR)
			{
				*error_str = check_after_redir(cpy, cmd_array[i + 1]);
				if (*error_str)
					return ;
				if (cpy->redir != PIPE)
				{
					cpy = cpy->next;
					set_next_flag(cpy, cpy->previous->redir);
				}
			}
			cpy = cpy->next;
		}
		i++;
	}
}