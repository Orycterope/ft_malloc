/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/04 14:20:39 by tvermeil          #+#    #+#             */
/*   Updated: 2017/03/08 02:57:32 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "malloc_functions.h"
#include "malloc_structures.h"
#include "global.h"

/*
** splits a free buffer in two free buffers, making the first one -size- size
** and the second one the remaining size. Also updates linked lists.
** Returns 0, but on error returns 1.
*/
static int	split_free_buffer(t_buffer *buf, size_t first_size)
{
	t_buffer		new_buf;
	t_buffer		*new_buf_saved_location;

	new_buf.len = buf->len - first_size;
	new_buf.alloc_status = BUFFER_FREE;
	new_buf.next_buf = buf->next_buf;
	new_buf.prev_buf = buf;
	new_buf_saved_location = save_buffer_to_tables(new_buf).buf;
	if (new_buf_saved_location == NULL)
		return (1);
	if (buf->next_buf)
		buf->next_buf->prev_buf = new_buf_saved_location;
	buf->next_buf = new_buf_saved_location;
	buf->len = first_size;
	return (0);
}

/*
** Mark a buffer as allocated, splitting it and recalculating
** map->shortest/longest if needed
** alloc must be a valid alloc location
** Returns 0, but on error returns 1.
*/
int			create_allocation(t_alloc_loc_reduced alloc, size_t desired_size)
{
	int		need_to_recompute_len;

	need_to_recompute_len = 0;
	if (alloc.buf->len == alloc.map->longest_free_buffer
			|| alloc.buf->len == alloc.map->shortest_free_buffer)
		need_to_recompute_len = 1;
	if (alloc.buf->len != desired_size)
	{
		if (split_free_buffer(alloc.buf, desired_size))
			return (1);
	}
	alloc.buf->alloc_status = BUFFER_ALLOCATED;
	if (need_to_recompute_len)
		recompute_best_len(alloc.map);
	return (0);
}

/*
** Merges consecutives free buffers, and recomputes best_len if needed.
*/
static void	merge_free_buffers(t_alloc_loc_reduced alloc)
{
	int				need_to_recompute_len;
	t_buffer		*b;
	t_buf_location	next;

	b = alloc.buf;
	while (b->prev_buf && b->prev_buf->alloc_status == BUFFER_FREE)
		b = b->prev_buf;
	need_to_recompute_len = (b->len == alloc.map->shortest_free_buffer);
	while (b->next_buf && b->next_buf->alloc_status == BUFFER_FREE)
	{
		next.buf = b->next_buf;
		next.table = find_table_of_buffer(b->next_buf);
		if (next.buf->len == alloc.map->shortest_free_buffer)
			need_to_recompute_len = 1;
		if (next.buf->next_buf)
			next.buf->next_buf->prev_buf = b;
		b->next_buf = next.buf->next_buf;
		b->len += next.buf->len;
		remove_buffer_from_tables(next);
	}
	if (b->len > alloc.map->longest_free_buffer)
		alloc.map->longest_free_buffer = b->len;
	if (need_to_recompute_len)
		recompute_best_len(alloc.map);
}

/*
** Parses all mappings and returns if there are other mapping of the same type
** Used to allways keep at least one mapping of each type.
*/
static int	is_only_mapping_of_type(enum e_mapping_type type, t_mapping *map)
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
			if (m->mapping_type == type && !(m == map))
				return (0);
			m++;
		}
		t = t->next_table;
	}
	return (1);
}

/*
** Marks the buffer as free, merge it with consecutives free buffers,
** and recompute mapping best_len if needed.
** If the mapping is empty afterward, unmap it unless it is the only mapping of
** this type, so we always keep at least one mapping of each type.
** -alloc- must contain valid pointers.
*/
void		free_buffer_at(t_alloc_location loc)
{
	int					should_free_mapping;
	t_alloc_loc_reduced	alloc;

	should_free_mapping = 0;
	alloc.buf = loc.b.buf;
	alloc.map = loc.m.map;
	alloc.buf->alloc_status = BUFFER_FREE;
	merge_free_buffers(alloc);
	if (alloc.map->mapping_type == MAPPING_LARGE)
		should_free_mapping = 1;
	else if (alloc.map->buffers->alloc_status == BUFFER_FREE
			&& alloc.map->buffers->next_buf == NULL
			&& !is_only_mapping_of_type(alloc.map->mapping_type, alloc.map))
		should_free_mapping = 1;
	if (should_free_mapping)
	{
		t_buf_location	map_first_buf;

		map_first_buf.buf = alloc.map->buffers;
		map_first_buf.table = find_table_of_buffer(alloc.map->buffers);
		remove_buffer_from_tables(map_first_buf);
		munmap(alloc.map->map_addr, alloc.map->map_len);
		remove_mapping_from_tables(loc.m);
	}
}
