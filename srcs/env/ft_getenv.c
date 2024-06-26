/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttakala <ttakala@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 10:16:57 by ttakala           #+#    #+#             */
/*   Updated: 2024/03/06 11:06:49 by ttakala          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Returns a pointer to the value of an environment variable
 * in envp, or NULL if the variable is not found.
 */
char	*ft_getenv(const char *name, char *const envp[])
{
	int	i;
	int	len;

	if (!name || !envp)
		return (NULL);
	i = 0;
	len = ft_strlen(name);
	while (len && envp[i])
	{
		if (ft_strncmp(envp[i], name, len) == 0)
		{
			if (envp[i][len] == '=')
				return (envp[i] + len + 1);
		}
		i++;
	}
	return (NULL);
}
