/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llecoq <llecoq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/28 10:30:12 by llecoq            #+#    #+#             */
/*   Updated: 2021/08/19 13:49:34 by llecoq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

    //    The putenv() function adds or changes the value of environment
    //    variables.  The argument string is of the form name=value.  If
    //    name does not already exist in the environment, then string is
    //    added to the environment.  If name does exist, then the value of
    //    name in the environment is changed to value.  The string pointed
    //    to by string becomes part of the environment, so altering the
    //    string changes the environment.

	// The putenv() function returns zero on success.  On failure, it
	// returns a nonzero value, and errno is set to indicate the error.

// EXPORTED VAR ARE RANDOMLY ADDED INSIDE THE ENV, WHAT'S THE LOGIC ?
// Our fonction just add them at the end of the list so far
int	valid_var_name(t_shell *shell, char *string, char **name)
{
	int		i;
	t_list	*new_elem;

	if (ft_isdigit(string[0]))
		return (NOT_A_VALID_IDENTIFIER);		// if first character is digit, export: not an identifier: $name
	i = 0;
	while (string[i] && string[i] != '=')
		i++;
	if (string[i] != '=')
	{
		new_elem = ft_lstnew(ft_strdup(string));
		new_elem->variable = IS_SET;
		ft_lstadd_back(&shell->export_list, new_elem);
		sort_alphabetically_list(&shell->export_list);
		return (ADDED_TO_EXPORT_LIST);		// only visible in export, but not in env
	}
	i++;
	*name = calloc_sh(shell, i);
	ft_strlcpy(*name, string, i);
	return (IS_VALID);
}

int	found_var(t_list *env_list, char *name, t_list **variable_ptr, size_t len)
{
	char	*content;

	while (env_list)
	{
		content = (char *)env_list->content;
		if (ft_strncmp(content, name, len) == 0 && content[len] == '=')
		{
			free(env_list->content);
			*variable_ptr = env_list;
			return (1);
		}
		env_list = env_list->next;
	}
	return (0);
}

void	insert_into_list(t_shell *shell, char *string, char *name, int type)
{
	size_t	len;
	t_list	*list;
	t_list	*variable_ptr;

	list = NULL;
	if (type == EXPORT_LIST)
		list = shell->export_list;
	else if (type == ENV_LIST)
		list = shell->env_list;
	len = ft_strlen(name);
	if (found_var(list, name, &variable_ptr, len))
	{
		variable_ptr->content = ft_strdup(string);
		variable_ptr->variable = IS_SET;
		if (ft_strncmp(name, "PATH", 5) == 0)
			shell->path->variable = IS_SET;
	}
	else
		ft_lstadd_back(&shell->env_list, ft_lstnew(ft_strdup(string)));
}

int	put_env(t_shell *shell, char *string)
{
	char	*name;
	int		return_value;

	if (!string) // quelle valeur retourner ?
		return (-1);
	return_value = valid_var_name(shell, string, &name);
	if (return_value == IS_VALID)
	{
		insert_into_list(shell, string, name, ENV_LIST);
		insert_into_list(shell, string, name, EXPORT_LIST);
		free(name);
		store_environment_tab(shell, shell->env_list, env_size(shell->env_list));
	}
	if (return_value == ADDED_TO_EXPORT_LIST)
		return_value = IS_VALID;
	return (return_value); // error code
}
