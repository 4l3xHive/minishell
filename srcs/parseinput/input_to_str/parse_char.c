/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_char.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 17:03:52 by apyykone          #+#    #+#             */
/*   Updated: 2024/03/06 13:12:22 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_escape_sequence(t_parsehelper *ph, const char *input_string)
{
	ph->i++;
	if (input_string[ph->i] == 'n')
		ph->curr_cmd[ph->j++] = '\n';
	else if (input_string[ph->i] == 't')
		ph->curr_cmd[ph->j++] = '\t';
	else if (input_string[ph->i] == 'r')
		ph->curr_cmd[ph->j++] = '\r';
	else if (input_string[ph->i] == '\\')
		ph->curr_cmd[ph->j++] = '\\';
	else if (input_string[ph->i] == '"')
		ph->curr_cmd[ph->j++] = '\"';
	else if (input_string[ph->i] == '\'')
		ph->curr_cmd[ph->j++] = '\'';
	else
		ph->curr_cmd[ph->j++] = input_string[ph->i];
}

static void	expand_env_variable(t_parsehelper *ph, t_shellstate *s)
{
	char		*var_value;
	int			val_len;
	bool		free_var_value;
	const bool	should_quote = !ph->in_double_quote;

	free_var_value = false;
	ph->i += 1;
	var_value = get_env_var_value(s, ph, &free_var_value);
	if (var_value)
	{
		val_len = ft_strlen(var_value);
		ensure_mem_for_buff(ph, s, ft_strlen(ph->curr_cmd) + val_len + 10,
			ph->curr_cmd);
		if (should_quote)
			ft_strncat(ph->curr_cmd, "\"", 1);
		ft_strncat(ph->curr_cmd, var_value, val_len);
		if (should_quote)
			ft_strncat(ph->curr_cmd, "\"", 1);
		ph->j = ft_strlen(ph->curr_cmd);
		if (free_var_value)
			free(var_value);
	}
}

static void	separate_redir_with_spaces(t_parsehelper *ph, t_shellstate *state)
{
	const char	*c = &state->input_string[ph->i];
	const bool	space_before = (ph->i > 0) && (*(c - 1) != ' ' && *(c
				- 1) != '\t');
	const bool	space_after = (*(c + 1) != ' ' && *(c + 1) != '\t' && *(c
				+ 1) != '\0');
	const bool	is_double_redir = (*(c + 1) == *c);

	ph->was_redirect = true;
	ensure_mem_for_buff(ph, state, 8, ph->curr_cmd);
	if (space_before)
		ph->curr_cmd[ph->j++] = ' ';
	ph->curr_cmd[ph->j++] = *c;
	if (is_double_redir)
	{
		ph->curr_cmd[ph->j++] = *c;
		ph->i++;
	}
	if (space_after)
		ph->curr_cmd[ph->j++] = ' ';
}

static void	handle_non_quoted_char(t_parsehelper *ph, t_shellstate *s, int f)
{
	if (f & (1 << REDIR_BIT))
		separate_redir_with_spaces(ph, s);
	else if ((f & (1 << PIPE_BIT)) || (f & (1 << AND_BIT))
		|| (f & (1 << OR_BIT)))
	{
		ph->curr_cmd[ph->j] = '\0';
		ph->commands[ph->command_index++] = ft_strdup(ph->curr_cmd);
		ph->j = 0;
		if ((f & (1 << AND_BIT)) || (f & (1 << OR_BIT)))
			ph->i++;
	}
	else if (f & (1 << ENVVAR_BIT))
	{
		expand_env_variable(ph, s);
	}
	else
	{
		ensure_mem_for_buff(ph, s, 1, ph->curr_cmd);
		if (ph->was_redirect && ph->curr_cmd[ph->j] != ' '
			&& ph->curr_cmd[ph->j])
			ph->was_redirect = false;
		ph->curr_cmd[ph->j++] = s->input_string[ph->i];
	}
}

void	parse_character(t_parsehelper *ph, t_shellstate *s)
{
	int	flags;

	init_char_flags(&flags, &s->input_string[ph->i], ph);
	if (flags & (1 << TILDA_BIT))
		handle_tilda(ph, s);
	else if ((flags & (1 << ENVVAR_BIT)) && ft_checkdollar(s, ph))
		flags &= ~(1 << ENVVAR_BIT);
	else if (!(flags & (1 << QUOTE_BIT)) && (ph->in_single_quote
			|| ph->in_double_quote))
	{
		if (flags & (1 << ESCAPED_BIT))
			handle_escape_sequence(ph, s->input_string);
		else if (ph->in_double_quote && (flags & (1 << ENVVAR_BIT)))
		{
			expand_env_variable(ph, s);
		}
		else
			ph->curr_cmd[ph->j++] = s->input_string[ph->i];
	}
	else
		handle_non_quoted_char(ph, s, flags);
	ensure_mem_for_buff(ph, s, 5, ph->curr_cmd);
	ph->curr_cmd[ph->j] = '\0';
}
