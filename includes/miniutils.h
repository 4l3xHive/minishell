/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniutils.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 17:52:45 by apyykone          #+#    #+#             */
/*   Updated: 2024/02/27 21:25:05 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIUTILS_H
# define MINIUTILS_H
# include <stdbool.h>
# include <unistd.h>

typedef struct s_shellstate	t_shellstate;

enum						e_returnvalues
{
	SUCCESS = 0,
	IS_QUOTE = 1,
};

typedef struct s_redirs
{
	char					*command;
	char					*input_file;
	char					*output_file;
	bool					append;
}							t_redirs;

typedef struct s_parsehelper
{
	bool					in_single_quote;
	bool					in_double_quote;
	int						command_count;
	char					**commands;
	char					*curr_cmd;
	int						command_index;
	size_t					j;
	size_t					i;
	size_t					curr_size;
}							t_parsehelper;

typedef struct s_exechelper
{
	char					**cmd_args;
	int						pipefd[2];
	int						fd_in;
	int						i;
	int						status;
	pid_t					pid;
}							t_exechelper;

typedef struct s_envhelper
{
	char					*var_name;
	int						var_name_len;
	char					*var_value;
	int						value_len;
	size_t					req_size;
}							t_envhelper;

typedef struct s_charflags
{
	bool					is_escaped;
	bool					is_env_var;
	bool					is_pipe;
	bool					is_and;
	bool					is_or;
}							t_charflags;

typedef struct s_charflags
{
	bool					is_escaped;
	bool					is_env_var;
	bool					is_pipe;
	bool					is_and;
	bool					is_or;
}							t_charflags;

void						free_str_array(char **str_arr);
int							ft_parseinput(t_shellstate *state);
char						*trim_spaces(const char *str);
int							ft_isenv_var(int c);
void						parse_cmd_char(t_parsehelper *h,
								t_shellstate *state);
void						ensure_memory_for_cmd(t_parsehelper *h,
								t_shellstate *state, size_t additional_length);
int							init_char_flags(t_charflags *flags, char *c,
								t_parsehelper *h);

#endif
