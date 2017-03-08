/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mapping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 17:34:24 by tvermeil          #+#    #+#             */
/*   Updated: 2017/03/08 03:24:01 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "malloc_settings.h"
#include "malloc_structures.h"
#include "malloc_functions.h"
#include "global.h"

/*
** Creates a free buffer of the whole mapping len and saves it to the tables
** If saving to the tables fails, returns a location filled with NULL
*/
static t_buf_location	create_initial_buffer(size_t len)
{
	t_buffer	buf;

	buf.len = len;
	buf.alloc_status = BUFFER_FREE;
	buf.next_buf = NULL;
	buf.prev_buf = NULL;
	return (save_buffer_to_tables(buf));
}

/*
** Creates a new mapping and saves it to the tables, also chooses the mapping
** type (TINY/SMALL/BIG) and saves it to t_mapping->mapping_type.
** Initialize the mapping with a single free buffer of the whole mapping length.
** On error nothing is mmaped and the function returns NULL
*/
static t_map_location	create_mapping(size_t desired)
{
	t_mapping			mapping;
	t_alloc_location	loc;

	mapping.map_len = get_page_mapping_size(desired);
	mapping.mapping_type = get_mapping_type(desired);
	mapping.map_addr = mmap(NULL, mapping.map_len, MMAP_PARAMETERS);
	if (!(mapping.map_addr == MAP_FAILED))
	{
		loc.b = create_initial_buffer(mapping.map_len);
		if (!(loc.b.buf == NULL))
		{
			mapping.buffers = loc.b.buf;
			mapping.longest_free_buffer = mapping.map_len;
			mapping.shortest_free_buffer = mapping.map_len;
			loc.m = save_mapping_to_tables(mapping);
			if (!(loc.m.map == NULL))
				return (loc.m);
			remove_buffer_from_tables(loc.b);
		}
		munmap(mapping.map_addr, mapping.map_len);
	}
	ft_bzero(&loc, sizeof(t_alloc_location));
	return (loc.m);
}

/*
** This fucntion runs along the buffers of -mapping-
** and returns the most suitable free space for an allocation of -size-
** if non is suitable, returns NULL.
*/
static t_buffer			*find_best_free_buffer_in_mapping(t_mapping *mapping,
	   	size_t size)
{
	t_buffer	*p;
	t_buffer	*best;

	if (mapping->longest_free_buffer < size)
		return (NULL);
	best = NULL;
	p = mapping->buffers;
	while (p)
	{
		if (p->alloc_status == BUFFER_FREE)
		{
			if (p->len == size)
				return (p);
			else if (p->len > size && (best == NULL || p->len < best->len))
				best = p;
		}
		p = p->next_buf;
	}
	return (best);
}

/*
** This functions checks if a mapping is better for -alloc_size- than
** -best_map-, saying it has a smallest_free_buffer closer to -alloc_size-
** and is of the right type.
** If true it updates -best- to -current-
** If -best_map- points to NULL, -current- is considered better.
*/
static void				check_is_best(t_alloc_loc_reduced *best,
	enum e_mapping_type type, t_mapping	*current_map, size_t alloc_size)
{
	t_buffer			*current_best_buf;

	if (current_map->mapping_type != type)
		return ;
	if (best->buf && current_map->shortest_free_buffer >= best->buf->len)
		return ;
	current_best_buf =
		find_best_free_buffer_in_mapping(current_map, alloc_size);
	if (current_best_buf == NULL)
		return ;
	if (best->map == NULL
			|| current_best_buf->len < best->buf->len)
	{
		best->map = current_map;
		best->buf = current_best_buf;
	}
}

/*
** This function parses all mappings in search for the closest free size buffer
** to alloc_size. This prevents memory fragmentation but is time-consuming.
** If no mapping is suitable, this function creates one.
** On error nothing is mmaped and the function returns a struct pointing to NULL
** When called g_malloc_infos.tables must already be set.
** The table pointers are not saved but saved to NULL
*/
t_alloc_loc_reduced		find_best_mapping_for_size(size_t size)
{
	enum e_mapping_type	type;
	t_alloc_loc_reduced	best;
	int					map_counter;
	t_table				*t;
	t_mapping			*m;

	type = get_mapping_type(size);
	ft_bzero(&best, sizeof(best));
	t = g_malloc_infos.tables;
	while (type != MAPPING_LARGE
			&& t != NULL && !(best.buf && best.buf->len == size))
	{
		m = (t_mapping *)((void *)t + sizeof(t_table));
		map_counter = -1;
		while (++map_counter < t->occupied_maps
				&& !(best.buf && best.buf->len == size))
			check_is_best(&best, type, m++, size);
		t = t->next_table;
	}
	if (best.map == NULL)
	{
		if ((best.map = create_mapping(size).map))
			best.buf = best.map->buffers;
	}
	return (best);
}
