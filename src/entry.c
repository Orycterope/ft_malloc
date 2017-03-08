/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entry.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 15:14:01 by tvermeil          #+#    #+#             */
/*   Updated: 2017/03/08 02:42:45 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "malloc_structures.h"
#include "malloc_functions.h"
#include "global.h"

/*
** Finds the first table that can contain a type -type-
** returns a pointer to that table.
** If no entry is free, mmap a new table.
** If mmap fails, returns NULL.
** When called g_malloc_infos.tables must already be set.
*/
static t_table	*find_table_for_type(enum e_entry_type type)
{
	t_table			*table;
	t_table			*new_table;

	table = g_malloc_infos.tables;
	while (table)
	{
		if (table_has_room_for(table, type))
			return (table);
		if (table->next_table == NULL)
			if ((new_table = create_table()))
			{
				table->next_table = new_table;
				new_table->prev_table = table;
			}
		table = table->next_table;
	}
	return (table);
}

/*
** Saves -buffer- to tables and updates occupied_count.
** Returns a t_buf_location struct saving also the buffer's table
** If no spot is suitable, this funciton mmaps a new table.
** If the mmap failed, the return is filled with NULL
** When called g_malloc_infos.tables must already be set.
*/
t_buf_location	save_buffer_to_tables(t_buffer buf)
{
	t_buf_location	loc;

	loc.buf = NULL;
	loc.table = find_table_for_type(ENTRY_BUFFER);
	if (loc.table)
	{
		loc.buf = (t_buffer *)((void *)loc.table + loc.table->mapping_size) - 1;
		loc.buf -= loc.table->occupied_buffers;
		*loc.buf = buf;
		loc.table->occupied_buffers++;
	}
	return (loc);
}

/*
** Saves -map- to tables and updates occupied_count.
** Returns a t_map_location struct saving also the map's table
** If no spot is suitable, this funciton mmaps a new table.
** If the mmap failed, the return is filled with NULL
** When called g_malloc_infos.tables must already be set.
*/
t_map_location	save_mapping_to_tables(t_mapping map)
{
	t_map_location	loc;

	loc.map = NULL;
	loc.table = find_table_for_type(ENTRY_MAPPING);
	if (loc.table)
	{
		loc.map = (t_mapping *)((void *)loc.table + sizeof(struct s_table));
		loc.map += loc.table->occupied_maps;
		*loc.map = map;
		loc.table->occupied_maps++;
	}
	return (loc);
}

/*
** Remove a t_mapping from the table it's on, defragments the table,
** and updates table->occupied_count.
** Attempts to free the table.
*/
void			remove_mapping_from_tables(t_map_location loc)
{
	t_mapping	*first_mapping;
	t_mapping	*last_mapping;

	first_mapping = (t_mapping *)((void *)loc.table + sizeof(struct s_table));
	last_mapping = first_mapping + (loc.table->occupied_maps - 1);
	if (loc.map != last_mapping)
		*loc.map = *last_mapping;
	loc.table->occupied_maps--;
	ft_bzero(last_mapping, sizeof(struct s_mapping)); // debug, remove me
	try_free_table(loc.table);
}

/*
** Remove a t_buffer from the table it's on, defragments the table,
** and updates table->occupied_count.
** If the defragmentation moves a buffer, it will update its linked list
** Attempts to free the table.
** All the data in -loc- must be valid.
*/
void			remove_buffer_from_tables(t_buf_location loc)
{
	t_buffer	*first_buffer;
	t_buffer	*last_buffer;

	first_buffer = (t_buffer *)
		((void *)loc.table + loc.table->mapping_size) - 1;
	last_buffer = first_buffer - (loc.table->occupied_buffers - 1);
	if (loc.buf != last_buffer)
	{
		*loc.buf = *last_buffer;
		if (loc.buf->next_buf)
			loc.buf->next_buf->prev_buf = loc.buf;
		if (loc.buf->prev_buf)
			loc.buf->prev_buf->next_buf = loc.buf;
		else
			find_mapping_of_first_buffer(last_buffer)->buffers = loc.buf;
	}
	loc.table->occupied_buffers--;
	ft_bzero(last_buffer, sizeof(struct s_buffer)); // debug, remove me
	try_free_table(loc.table);
}
