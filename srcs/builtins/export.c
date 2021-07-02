/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llecoq <llecoq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/28 18:32:03 by llecoq            #+#    #+#             */
/*   Updated: 2021/07/02 17:03:36 by llecoq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// name :
// A word consisting solely of letters, numbers, and underscores, and beginning with
// a letter or underscore. Names are used as shell variable and function names. Also
// referred to as an identifier.

// RESTE A GERER = EXPORT seul
size_t	count_args(char **argv)
{
	int	len;

	len = 0;
	while (argv[len])
		len++;
	return (len);	
}

char	*join_args(char	**argv)
{
	char	*tmp;
	char	*export_name_and_value;

	argv++;
	export_name_and_value = ft_strjoin(*argv, " ");
	while (*argv)
	{
		tmp = export_name_and_value;
		export_name_and_value = ft_strjoin(tmp, *argv++);
		free(tmp);
		if (*argv)
		{
			tmp = export_name_and_value;
			export_name_and_value = ft_strjoin(tmp, " ");
			free(tmp);
		}
	}
	return (export_name_and_value);
}

int	print_export_list(t_list *export_list)
{
	while (export_list)
	{
		ft_printf(1, "declare -x %s\n", export_list->content);
		export_list = export_list->next;
	}
	return (0);
}

int	ft_export(t_shell *shell, char **argv)
{
	char	*export_name_and_value;
	
	if (invalid_args_or_options(argv, "export"))
		return (-1);
	if (!argv[1])
		return (print_export_list(shell->export_list));
	if (valid_name(argv[1], "export"))
	{
		export_name_and_value = ft_strdup(argv[1]);
		if (count_args(argv) > 2)
		{
			free_set_null((void **)&export_name_and_value);
			export_name_and_value = join_args(argv);
		}
		put_env(shell, export_name_and_value);
		free_set_null((void **)&export_name_and_value);
	}
	else
		ft_printf(2, "minishell: export: `%s': not a valid identifier\n", argv[1]);
	return (0);
}
