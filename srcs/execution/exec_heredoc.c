/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apyykone <apyykone@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 05:32:28 by apyykone          #+#    #+#             */
/*   Updated: 2024/03/07 05:32:29 by apyykone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "libft.h"
#include "minimessages.h"
#include "minishell.h"
#include "miniutils.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

static void	heredoc_signal_handler(int signo)
{
	if (signo == SIGINT)
	{
		g_signal_flag = 130;
		exit(SIGINT_EXIT);
	}
}

static void	heredoc_child_process(t_hdochelper *hh)
{
	char	*line;

	signal(SIGINT, heredoc_signal_handler);
	close(hh->pipe_fds[0]);
	while (true)
	{
		line = readline("heredoc> ");
		if (line == NULL)
		{
			ft_fprintf(STDERR_FILENO, HDOC_DELIMMSG, hh->delimiter);
			break ;
		}
		if (ft_strcmp(line, hh->delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(hh->pipe_fds[1], line, ft_strlen(line));
		write(hh->pipe_fds[1], "\n", 1);
		free(line);
	}
	close(hh->pipe_fds[1]);
	exit(EXIT_SUCCESS);
}

static void	handle_higher_process(t_hdochelper *hh, t_exechelper *h)
{
	int	status;

	sigaction(SIGINT, &hh->s->ignoreaction, &hh->s->sigaction);
	close(hh->pipe_fds[1]);
	waitpid(hh->pid, &status, 0);
	sigaction(SIGINT, &hh->s->sigaction, NULL);
	if (WEXITSTATUS(status) == SIGINT_EXIT)
		return (close(hh->pipe_fds[0]), close(h->pipefd[0]),
			close(h->pipefd[1]), exit(SIGINT_EXIT));
	hh->rh->fd = hh->pipe_fds[0];
	update_fds(NULL, hh->rh, false);
}

void	handle_heredoc(t_redirecthelper *rh, char *delimiter, t_shellstate *s,
		t_exechelper *eh)
{
	t_hdochelper	hh;

	hh = (t_hdochelper){0};
	hh.s = s;
	hh.rh = rh;
	hh.delimiter = delimiter;
	if (!delimiter)
		return (ft_putstr_fd(ERR_HEREDOC_DELIMITER, STDERR_FILENO));
	if (pipe(hh.pipe_fds) == -1)
		return (perror("pipe"), exit(EXIT_FAILURE));
	hh.pid = fork();
	if (hh.pid == -1)
		return (perror("fork() heredoc"), close(hh.pipe_fds[0]),
			close(hh.pipe_fds[1]), exit(EXIT_FAILURE));
	if (hh.pid == 0)
		heredoc_child_process(&hh);
	else
		handle_higher_process(&hh, eh);
}
