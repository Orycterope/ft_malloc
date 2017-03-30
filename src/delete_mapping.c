/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete_mapping.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/29 12:45:45 by tvermeil          #+#    #+#             */
/*   Updated: 2017/03/30 17:18:57 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "malloc_functions.h"
#include "malloc_structures.h"
#include "global.h"

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
** Decides if the mapping should be deleted, and if yes removes its
** first and only buffer from tables, and then deletes it.
*/

void		try_delete_mapping(t_map_location m)
{
	t_buf_location	map_first_buf;

	if (m.map->buffers->alloc_status == BUFFER_FREE
			&& m.map->buffers->next_buf == NULL
			&& !is_only_mapping_of_type(m.map->mapping_type, m.map))
	{
		map_first_buf.buf = m.map->buffers;
		map_first_buf.table = find_table_of_buffer(m.map->buffers);
		remove_buffer_from_tables(map_first_buf);
		munmap(m.map->map_addr, m.map->map_len);
		remove_mapping_from_tables(m);
	}
}
