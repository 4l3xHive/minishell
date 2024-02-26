/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 10:55:29 by ttakala           #+#    #+#             */
/*   Updated: 2024/02/26 12:34:33 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static
void	free_str_array(char **str_arr)
{
	char	**tmp;

	tmp = str_arr;
	while (tmp && *tmp)
	{
		free(*tmp);
		*tmp = NULL;
		tmp++;
	}
	free(str_arr);
}

static
char	*get_full_path(char **env_paths_arr, char *executable_name)
{
	char	*result;
	char	*executable_name_with_slash;

	executable_name_with_slash = NULL;
	executable_name_with_slash = ft_strjoin("/", executable_name);
	if (!executable_name_with_slash)
		return (NULL);
	result = NULL;
	while (*env_paths_arr)
	{
		result = ft_strjoin(*env_paths_arr, executable_name_with_slash);
		if (access(result, X_OK) == 0)
		{
			free(executable_name_with_slash);
			return (result);
		}
		free(result);
		env_paths_arr++;
	}
	free(executable_name_with_slash);
	return (NULL);
}

static
char	*get_path_to_cmd(char *cmd, const char *env_path)
{
	char		**str_arr_paths;
	char		*full_path;

	if (!cmd || !*cmd || cmd[0] == '/' || cmd[0] == '.')
		return (NULL);
	if (!env_path)
		return (NULL);
	str_arr_paths = ft_split(env_path, ':');
	if (!str_arr_paths)
		return (NULL);
	if (str_arr_paths[0])
	{
		full_path = get_full_path(str_arr_paths, cmd);
		if (!full_path)
			full_path = ft_strdup(cmd);
		free_str_array(str_arr_paths);
		return (full_path);
	}
	free_str_array(str_arr_paths);
	return (NULL);
}

void	execute_cmd(char *cmd, char **cmd_argv)
{
	const char	*env_path = getenv("PATH");
	char		*cmd_path;

	cmd_path = get_path_to_cmd(cmd, env_path);
	if (cmd_path && (cmd_path[0] == '.' || ft_strchr(cmd_path, '/') != NULL))
	{
		if (execve(cmd_path, cmd_argv, __environ) == -1)
		{
			if (access(cmd_path, F_OK) == 0)
			{
				if (access(cmd_path, X_OK) == 0)
					printf("%s: Is a directory\n", cmd_path);
				else
					printf("%s: Permission denied\n", cmd_path);
			}
			else
				printf("%s: No such file or directory\n", cmd_path);
		}
	}
	printf("%s: command not found\n", cmd);
	free_str_array(cmd_argv);
	free(cmd_path);
	exit(EXIT_FAILURE);
}
