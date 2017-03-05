/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/04 16:18:24 by tvermeil          #+#    #+#             */
/*   Updated: 2017/03/04 19:47:26 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "malloc_functions.h"
#include "malloc_structures.h"
#include "global.h"

/*
** Searchs for buffer -buf- in mapping -m- and if found it fills -loc-
*/
static void			find_buffer_mapping(char *buf, t_mapping *m,
		t_alloc_location	*loc)
{
	t_buffer	*b;
	char		*addr;

	addr = (char *)m->map_addr;
	b = m->buffers;
	while (b)
	{
		if (addr == buf)
		{
			loc->buf = b;
			loc->map = m;
			return ;
		}
		addr += b->len;
		b = b->next_buf;
	}
}

/*
** Computes address of -loc-
*/
void				*get_address_of_loc(t_alloc_location loc)
{
	t_buffer	*b;
	char		*addr;

	addr = (char *)loc.map->map_addr;
	b = loc.map->buffers;
	while (b)
	{
		if (b == loc.buf)
		{
			return (addr);
		}
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
	t_table				*t;
	t_table_entry		*e;
	t_mapping			*m;

	t = g_malloc_infos.tables;
	while (t)
	{
		e = t->first_mapping_entry;
		while (e)
		{
			m = &e->entry_data.map;
			if (buf_addr >= m->map_addr && buf_addr < m->map_addr + m->map_len)
			{
				find_buffer_mapping(buf_addr, m, &loc);
				return (loc);
			}
			e = e->next_entry;
		}
		t = t->next_table;
	}
	loc.buf = NULL;
	loc.map = NULL;
	return (loc);
}
