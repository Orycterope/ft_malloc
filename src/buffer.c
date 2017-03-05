/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/04 14:20:39 by tvermeil          #+#    #+#             */
/*   Updated: 2017/03/05 19:41:25 by tvermeil         ###   ########.fr       */
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
	t_table_entry	*new_buf_entry;
	t_buffer		*new_buf_saved_location;

	new_buf.len = buf->len - first_size;
	new_buf.alloc_status = BUFFER_FREE;
	new_buf.next_buf = buf->next_buf;
	new_buf.prev_buf = buf;
	new_buf_entry = save_data_to_tables((union u_entry_data)new_buf, ENTRY_BUFFER);
	if (new_buf_entry == NULL)
		return (1);
	new_buf_saved_location = &new_buf_entry->entry_data.buf;
	if (buf->next_buf != NULL)
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
int			create_allocation(t_alloc_location alloc, size_t desired_size)
{
	int		need_to_recompute_len;

	need_to_recompute_len = 0;
	if (alloc.buf->len == alloc.map->longest_free_buffer
			|| alloc.buf->len == alloc.map->longest_free_buffer)
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
static void	merge_free_buffers(t_alloc_location alloc)
{
	int			need_to_recompute_len;
	t_buffer	*b;
	t_buffer	*next;

	need_to_recompute_len = 0;
	b = alloc.buf;
	while (b->prev_buf && b->prev_buf->alloc_status == BUFFER_FREE)
		b = b->prev_buf;
	if (b->len == alloc.map->shortest_free_buffer)
		need_to_recompute_len = 1;
	while (b->next_buf->alloc_status == BUFFER_FREE)
	{
		next = b->next_buf;
		if (next->len == alloc.map->shortest_free_buffer)
			need_to_recompute_len = 1;
		next->next_buf->prev_buf = b;
		b->next_buf = next->next_buf;
		b->len += next->len;
		remove_entry_from_tables((union u_entry_data *)next);
		b = b->next_buf;
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
	t_table			*t;
	t_table_entry	*e;

	t = g_malloc_infos.tables;
	while (t)
	{
		e = t->first_mapping_entry;
		while (e)
		{
			if (e->entry_data.map.mapping_type == type
				&& !(&e->entry_data.map == map))
				return (0);
			e = e->next_entry;
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
void		free_buffer_at(t_alloc_location alloc)
{
	int	should_free_mapping;

	should_free_mapping = 0;
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
		munmap(alloc.map->map_addr, alloc.map->map_len);
		remove_entry_from_tables((union u_entry_data *)alloc.map);
	}
}
