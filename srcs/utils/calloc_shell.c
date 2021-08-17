/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calloc_shell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llecoq <llecoq@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 14:08:02 by abonnel           #+#    #+#             */
/*   Updated: 2021/08/16 18:15:42 by llecoq           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*calloc_sh(t_shell *shell, int size)
{
	unsigned char	*ptr;
	int				i;
	
	i = 0;
	ptr = NULL;
	ptr = malloc(size);
	if (!ptr)
		error_quit(shell, 0, NULL);
	while (i < size)
	{
		ptr[i] = '\0';
		i++;
	}
	return ((void *)ptr);
}
