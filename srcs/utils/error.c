/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_typeor.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnel <abonnel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 15:25:16 by abonnel           #+#    #+#             */
/*   Updated: 2021/06/18 15:28:00 by abonnel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error_quit(t_shell *shell, int error_type)
{
	if (error_type == 0) //syscall errors
		dprintf(2, "%s\n", strerror(errno));
	// else if (error_type == 1)
	// 	dprintf(2, "");
	// else if (error_type == 2)
	// 	dprintf(2, "");
	// else if (error_type == 3)
	// 	dprintf(2, "");
	// else if (error_type == 4)
	// 	dprintf(2, "");
	// else if (error_type == 5)
	// 	dprintf(2, "");
	// else if (error_type == 6)
	// 	dprintf(2, "");
	clear_memory(shell);
	exit(EXIT_FAILURE);
}


/*
For NOTHING_AFTER_REDIR

bash-3.2$ ls -la | wc |
> bash: syntax error: unexpected end of file
--> I CHOSE the other error msg bc "unexpected end of file"
happens bc of multiline that we do not havw to handle
*/
void	error(t_shell *shell, int error_type)
{
	if (error_type == 0) //errno pour les built-in 
		dprintf(2, "%s\n", strerror(errno));
	else if (error_type == NO_CLOSING_QUOTE)
	{
		dprintf(2, "minishell: unexpected EOF while looking for matching `''\n");
		dprintf(2, "minishell: syntax error: unexpected end of file");
	}
	else if (error_type == NOTHING_AFTER_REDIR)
		dprintf(2, "minishell: syntax error near unexpected token `newline'");
	// else if (error_type == 3)
	// 	dprintf(2, "");
	// else if (error_type == 4)
	// 	dprintf(2, "");
	// else if (error_type == 5)
	// 	dprintf(2, "");
	// else if (error_type == 6)
	// 	dprintf(2, "");
	dprintf(2, "\n");
	clear_nonessential_memory(shell);
	//must stop current command and go back to prompt
}