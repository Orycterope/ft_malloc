/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/04 16:18:24 by tvermeil          #+#    #+#             */
/*   Updated: 2017/03/30 17:12:49 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "malloc_functions.h"
#include "malloc_structures.h"
#include "global.h"

/*
** Searchs for buffer -buf- in tables, and returns its table
** If nothing is found returns NULL
** If buf is NULL returns NULL.
*/

t_table				*find_table_of_buffer(t_buffer *buf)
{
	t_table	*t;

	t = g_malloc_infos.tables;
	while (t && buf)
	{
		if ((void *)buf > (void *)t
				&& (void *)buf < (void *)t + t->mapping_size)
			return (t);
		t = t->next_table;
	}
	return (NULL);
}

/*
** Searchs for buffer at -buf- in mapping -m-.
** If not found returns NULL
*/

static t_buffer		*find_buffer_in_mapping(void *buf_addr, t_mapping *m)
{
	t_buffer		*b;
	char			*addr;

	addr = (char *)m->map_addr;
	b = m->buffers;
	while (b)
	{
		if (addr == buf_addr)
			return (b);
		addr += b->len;
		b = b->next_buf;
	}
	return (NULL);
}

/*
** Computes address of -buf- in -map- mapping.
*/

void				*get_address_of_loc(t_alloc_loc_reduced loc)
{
	t_buffer	*b;
	char		*addr;

	addr = (char *)loc.map->map_addr;
	b = loc.map->buffers;
	while (b)
	{
		if (b == loc.buf)
			return (addr);
		addr += b->len;
		b = b->next_buf;
	}
	return (NULL);
}

/*
** Searchs a buffer in all tables, and returns a t_alloc_location of it
** If the buffer is not found in the tables, returns a t_alloc_location
** filled with NULL pointers.
** Can be safely called if g_malloc_infos.tables is not set.
*/

t_alloc_location	find_buffer_in_tables(void *buf_addr)
{
	t_alloc_location	loc;
	int					map_counter;

	loc.m.table = g_malloc_infos.tables;
	while (loc.m.table)
	{
		map_counter = -1;
		loc.m.map = (t_mapping *)((void *)loc.m.table + sizeof(struct s_table));
		while (++map_counter < loc.m.table->occupied_maps)
		{
			if (buf_addr >= loc.m.map->map_addr
					&& buf_addr < loc.m.map->map_addr + loc.m.map->map_len)
			{
				loc.b.buf = find_buffer_in_mapping(buf_addr, loc.m.map);
				loc.b.table = find_table_of_buffer(loc.b.buf);
				if (loc.b.buf == NULL || loc.b.table == NULL)
					ft_bzero(&loc, sizeof(t_alloc_location));
				return (loc);
			}
			loc.m.map++;
		}
		loc.m.table = loc.m.table->next_table;
	}
	ft_bzero(&loc, sizeof(t_alloc_location));
	return (loc);
}

/*
** This parses all mappings in search of one having -buf- as its first buffer
** this is used only for defragmentation of buffers in tables
*/

t_mapping			*find_mapping_of_first_buffer(t_buffer *buf)
{
	t_table		*t;
	t_mapping	*m;
	int			map_counter;

	t = g_malloc_infos.tables;
	while (t)
	{
		map_counter = -1;
		m = (t_mapping *)((void *)t + sizeof(struct s_table));
		while (++map_counter < t->occupied_maps)
		{
			if (m->buffers == buf)
				return (m);
			m++;
		}
		t = t->next_table;
	}
	return (NULL);
}
