/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/02 17:00:21 by tvermeil          #+#    #+#             */
/*   Updated: 2017/03/29 21:04:29 by tvermeil         ###   ########.fr       */
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
	.tables = NULL
};

void				*malloc(size_t size)
{
	t_alloc_loc_reduced	loc;

	//ft_putstr("Malloc was called\n");
	if (size == 0)
		return (NULL);
	//size += 100;
	if (g_malloc_infos.page_size == 0)
		g_malloc_infos.page_size = getpagesize();
	if (g_malloc_infos.tables == NULL)
		g_malloc_infos.tables = create_table();
	if (g_malloc_infos.tables == NULL)
		return (NULL);
	loc = find_best_mapping_for_size(size);
	if (loc.buf == NULL || loc.map == NULL)
	{
		ft_putstr("Mmmhh ... didn't find a good spot\n");
		return (NULL);
	}
	if (create_allocation(loc, size))
	{
		ft_putstr("Mmmhh ... final splitting failed\n");
		return (NULL);
	}
/*
	char				ptr_name[20];

	ft_itoa_base_to_buf((unsigned long long)get_address_of_loc(loc), "0123456789abcdef", ptr_name, 20);
	ft_putstr("malloc: address : +0x");
	ft_putendl(ptr_name);


	ft_putstr("malloc seems ... ok ???\n");
*/
	return (get_address_of_loc(loc));
}

void				free(void *addr)
{
	t_alloc_location	loc;

//	ft_putstr("Free was called\n");
	if (addr == NULL)
		return ;
	loc = find_buffer_in_tables(addr);
	if (loc.b.buf == NULL)
	{
		char				ptr_name[20];

		ft_itoa_base_to_buf((unsigned long long)addr, "0123456789abcdef", ptr_name, 20);
		ft_putstr("free  : address : -0x");
		ft_putendl(ptr_name);

		ft_putstr("Trying to free a non allock'ed buffer\n");
		//raise(SIGABRT);
	}
	else
		free_buffer_at(loc, 1);
}

void				*realloc(void *addr, size_t desired_size)
{
	t_alloc_location	old;
	void				*new;

	ft_putstr("--Realloc was called\n"); //
	if (addr == NULL || desired_size == 0)
	{
		free(addr);
		return (malloc(desired_size));
	}
	old = find_buffer_in_tables(addr);
	if (old.b.buf == NULL)
	{
		ft_putstr("Trying to realloc a non allock'ed buffer\n");
	//	raise(SIGABRT);
		return (NULL); //
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
