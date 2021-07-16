/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnel <abonnel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/14 16:50:37 by abonnel           #+#    #+#             */
/*   Updated: 2021/07/15 14:57:21 by abonnel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/*---------------------------------------------------------------------------*/
/*------------------- VAR EXPANSION -----------------------------------------*/
/*---------------------------------------------------------------------------*/

/*
REPLACE ENV VAR BY THEIR VALUES
For var that does not exist, just prints nothing

Outermost quotes are the one that defines behavior :
echo "aa $USER '$USER' lol"
aa abonnel 'abonnel' lol

Var name :
A word consisting solely of letters, numbers, and underscores
and beginning with a letter or underscore.
If char following $ is not a letter or underscore then $ + this following
char expand to nothing
*/

static char	*get_var_value(int i, int j, const char *str, t_shell *shell)
{
	char		*var_name;
	char		*var_value;

	var_value = NULL;
	var_name = calloc_sh(shell, sizeof(char) * j); //bc j = var_name_len + 1
	ft_strlcpy(var_name, str + i + 1, j);
	var_value = get_env(shell, var_name);
	free(var_name);
	return (var_value);
}

//i starts at $
//echo $$ is not to be handled by us
static int	insert_var_in_str(char **str, const int i, t_shell *shell)
{
	int			j;
	int			dst_len;
	char		*dst;
	char		*value;

	j = 1; // to start after $
	if (!is_word_char((*str)[i + j], FIRST_LETTER)) //if doesn't start by letter or _
	{
		ft_memmove(*str + i, *str + i + 2, ft_strlen(*str));
		return (1);
	}
	while (is_word_char((*str)[i + j], OTHER_LETTERS)) //j goes at the end of var name
		j++;
	value = get_var_value(i, j, *str, shell);
	if (!value) //if !var, replace var name by nothing in str
	{
		ft_memmove(*str + i, *str + i + j, ft_strlen(*str));
		return (0);
	}
	//dst len = src_len + diff between var name and var value + 1 for \0
	dst_len = ft_strlen(*str) - j + ft_strlen(value) + 1;
	dst = calloc_sh(shell, dst_len);
	ft_strlcpy(dst, *str, i + 1); //copy str until $ to dst
	ft_strlcat(dst, value, dst_len); //append var value
	ft_strlcat(dst, *str + i + j, dst_len); //append rest of str
	free_set_null((void **)str);
	*str = dst;
	return (ft_strlen(value) - 1);
}

/*--------------------------PROCESS_VARIABLES() -----------------------------*/	
/*
Takes a token and returns a NEW string that is the token with expanded var

We must restart at i + length of var value bc otherwise it will interpret 
things it should not so we send an int ptr to insert_var_in_str()
Variables that reference other var inside of themselves are already
processed at the export cmd stage so appear already fully expanded in env. 

In the case below, it shows what would happen if we would restart at the 
position were $ used to be instead of i + length of var value,
it would enter "$USER" and would interpret USER whereas it should not
as it was originaly defined as a string literal and should remain so

bash-3.2$ export NEWW='HEHE"$USER"bonjour'
bash-3.2$ echo "bonjour $NEWW et ensuite"
bonjour HEHE"$USER"bonjour et ensuite
bash-3.2$ env | grep "NEWW"
NEWW=HEHE"$USER"bonjour
*/
/*---------------------------------------------------------------------------*/

char	*process_variables(char *token, t_shell *shell) 
{
	int			i;
	char		*tk_cpy;
	
	tk_cpy = ft_strdup(token);
	i = 0;//pour norminette debuter a -1 et str[++i]
	while (tk_cpy[i])
	{
		if (tk_cpy[i] == '"')
		{
			i++;
			while (tk_cpy[i] != '"' && tk_cpy[i])
			{	
				if (tk_cpy[i] == '$' && tk_cpy[i + 1])
					i += insert_var_in_str(&tk_cpy, i, shell);
				i++;
			}
		}
		else if (tk_cpy[i] == '\'') //mettre lignes suivantes dans f() pr norm
		{
			i++;
			while (tk_cpy[i] != '\'' && tk_cpy[i])
				i++;
		}
		else if (tk_cpy[i] == '$' && tk_cpy[i + 1])
			i += insert_var_in_str(&tk_cpy, i, shell);
		i++;
	}
	return (tk_cpy);
}

void	replace_token_with_var(char **token, t_shell *shell)
{
	char		*tk_cpy;
	
	tk_cpy = process_variables(*token, shell);
	free(*token);
	*token = tk_cpy;
}

void arg_syntax_processing(t_token **cmd_array, t_shell *shell)
{
	int			i;
	t_token		*token;
	
	i = 0;
	while (cmd_array[i])
	{
		token = cmd_array[i];
		while (token)
		{	
			if (token->redir == 0)
				replace_token_with_var(&token->word, shell);
			token = token->next;
		}
		i++;
	}
}