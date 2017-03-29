/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/04 14:20:39 by tvermeil          #+#    #+#             */
/*   Updated: 2017/03/29 18:42:06 by tvermeil         ###   ########.fr       */
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
static int	split_buffer(t_buffer *buf, size_t first_size)
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
		if (split_buffer(alloc.buf, desired_size))
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
** Marks the buffer as free, merge it with consecutives free buffers,
** and recompute mapping best_len if needed.
** If the mapping is empty afterward, and -should_try_free_mapping- is not 0
** call try_delete_mapping.
** -alloc- must contain valid pointers.
*/
void		free_buffer_at(t_alloc_location loc, int should_try_free_mapping)
{
	t_alloc_loc_reduced	alloc;

	alloc.buf = loc.b.buf;
	alloc.map = loc.m.map;
	alloc.buf->alloc_status = BUFFER_FREE;
	merge_free_buffers(alloc);
	if (should_try_free_mapping)
		try_delete_mapping(loc.m);
}

/*
** Change the size of an alloc'ed buffer and adjust following buffer's size
** If the buffer is getting bigger, and following free space is reduced to 0,
** 		delete the zero size buffer.
** If the buffer is getting smaller, and no free space is following,
** 		create a free_buffer and isert it inbetween.
**		If creation failed, returns NULL.
** Returns the computed address of the buffer.
*/
void		*resize_buffer(t_alloc_loc_reduced loc, size_t new_size)
{
	size_t			old_size;
	t_buf_location	next_buf;

	old_size = loc.buf->len;
	if (old_size < new_size && (loc.buf->next_buf == NULL
				|| loc.buf->next_buf->alloc_status == BUFFER_ALLOCATED))
	{
		if (split_buffer(loc.buf, new_size))
			return (NULL);
	}
	else
	{
		if (old_size < new_size)
			loc.buf->next_buf->len -= new_size - old_size;
		else
			loc.buf->next_buf->len += old_size - new_size;
		loc.buf->len = new_size;
		if (loc.buf->next_buf->len == 0)
		{
			next_buf.buf = loc.buf->next_buf;
			next_buf.table = find_table_of_buffer(loc.buf->next_buf);
			remove_buffer_from_tables(next_buf);
		}
	}
	return (get_address_of_loc(loc));
}
