/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/02 17:00:21 by tvermeil          #+#    #+#             */
/*   Updated: 2017/03/30 17:26:10 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "malloc_structures.h"
#include "malloc_functions.h"
#include <unistd.h>
#include <signal.h>

t_general_struct	g_malloc_infos =
{
	.page_size = 0,
	.tables = NULL,
	.mutex = PTHREAD_MUTEX_INITIALIZER
};

void				*malloc(size_t size)
{
	t_alloc_loc_reduced	loc;

	if (size == 0)
		return (NULL);
	lock_mutex();
	if (g_malloc_infos.page_size == 0)
		g_malloc_infos.page_size = getpagesize();
	if (g_malloc_infos.tables == NULL)
		g_malloc_infos.tables = create_table();
	if (g_malloc_infos.tables == NULL)
		return (unlock_mutex(NULL));
	loc = find_best_mapping_for_size(size);
	if (loc.buf == NULL || loc.map == NULL || create_allocation(loc, size))
		return (unlock_mutex(NULL));
	return (unlock_mutex(get_address_of_loc(loc)));
}

void				free(void *addr)
{
	t_alloc_location	loc;

	if (addr == NULL)
		return ;
	lock_mutex();
	loc = find_buffer_in_tables(addr);
	if (loc.b.buf == NULL)
	{
		ft_putstr("Trying to free a non allock'ed buffer\n");
		raise(SIGABRT);
	}
	else
		free_buffer_at(loc, 1);
	unlock_mutex(NULL);
}

void				*realloc(void *addr, size_t desired_size)
{
	t_alloc_location	old;
	void				*new;

	if (addr == NULL || desired_size == 0)
	{
		free(addr);
		return (malloc(desired_size));
	}
	old = find_buffer_in_tables(addr);
	if (old.b.buf == NULL)
	{
		ft_putstr("Trying to realloc a non allock'ed buffer\n");
		raise(SIGABRT);
	}
	if (desired_size == old.b.buf->len)
		return (addr);
	if (desired_size < old.b.buf->len || (old.b.buf->next_buf
			&& old.b.buf->next_buf->alloc_status == BUFFER_FREE
			&& old.b.buf->len + old.b.buf->next_buf->len >= desired_size))
		return (resize_buffer(to_reduced_location(old), desired_size));
	free_buffer_at(old, 0);
	new = malloc(desired_size);
	ft_memcpy_overlap(new, addr, MIN(old.b.buf->len, desired_size));
	try_delete_mapping(old.m);
	return (new);
}

void				*calloc(size_t count, size_t size)
{
	void	*ptr;

	ft_putstr("Calloc was called\n");
	size *= count;
	ptr = malloc(size);
	if (ptr)
		ft_bzero(ptr, size);
	return (ptr);
}
