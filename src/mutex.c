/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mutex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/30 13:43:04 by tvermeil          #+#    #+#             */
/*   Updated: 2017/03/30 17:15:33 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_structures.h"
#include "global.h"
#include "libft.h"
#include <signal.h>

/*
** Attemps to lock the mutex in g_malloc_infos
*/

void	lock_mutex(void)
{
	int	ret;

	ret = pthread_mutex_lock(&g_malloc_infos.mutex);
	if (ret)
	{
		ft_putstr_fd("Malloc : Error locking the mutex, aborting\n", 2);
		raise(SIGABRT);
	}
}

/*
** Attempts to unlock the mutex in g_malloc_infos
** Returns the value passed as parameter, unchanged. Used because of norme
*/

void	*unlock_mutex(void *retrurn_value)
{
	int	ret;

	ret = pthread_mutex_unlock(&g_malloc_infos.mutex);
	if (ret)
	{
		ft_putstr_fd("Malloc : Error unlocking the mutex, aborting\n", 2);
		raise(SIGABRT);
	}
	return (retrurn_value);
}
