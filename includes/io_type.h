/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_type.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 19:29:23 by ttakala           #+#    #+#             */
/*   Updated: 2024/03/13 15:15:42 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IO_TYPE_H
# define IO_TYPE_H

# include "libft.h"

typedef struct s_io
{
	t_io_type	type;
	bool		ambiguous_redirect;
	char		*filename;
	int			fd;
}				t_io;

typedef struct s_command
{
	char		**argv;
	t_vec		io_vec;
	int			fd_stdin_backup;
	int			fd_stdout_backup;
}				t_command;

t_io_type		get_io_type(const char *str);
int				store_redirections_in_vec(t_vec *io_vec, t_list *arg_list);
void			remove_redirections_from_args(char **args, t_list *arg_list);

int				dup_fd(t_io *io);
int				open_file(t_io *io);

#endif