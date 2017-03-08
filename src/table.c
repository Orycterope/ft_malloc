/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/02 17:12:41 by tvermeil          #+#    #+#             */
/*   Updated: 2017/03/08 01:31:33 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_structures.h"
#include "malloc_settings.h"
#include "malloc_functions.h"
#include "global.h"

/*
** A funciton to know if a table as enough room to store the type
** described by -type-
*/
int			table_has_room_for(t_table *table, enum e_entry_type type)
{
	size_t	remaining_size;

	remaining_size = table->mapping_size
		- sizeof(struct s_table)
		- table->occupied_maps * sizeof(struct s_mapping)
		- table->occupied_buffers * sizeof(struct s_buffer);
	if (type == ENTRY_MAPPING)
		return (remaining_size >= sizeof(struct s_mapping));
	else
		return (remaining_size >= sizeof(struct s_buffer));
}

t_table		*create_table(void)
{
	size_t	mapping_size;
	t_table	*header;

	mapping_size = round_upper_page_size(sizeof(t_table)
			+ MAX(sizeof(t_buffer), sizeof(t_mapping)) * MIN_ENTRIES_PER_TABLE);
	header = (t_table *)mmap(NULL, mapping_size, MMAP_PARAMETERS);
	header = (header == MAP_FAILED) ? NULL : header;
	if (header != NULL)
	{
		ft_bzero(header, sizeof(*header));
		header->mapping_size = mapping_size;
	}
	return (header);
}

void		try_free_table(t_table *table)
{
	if (table->occupied_maps != 0 || table->occupied_buffers != 0)
		return ;
	if (table == g_malloc_infos.tables && table->next_table == NULL)
		return ;
	if (table->prev_table == NULL)
		g_malloc_infos.tables = table->next_table;
	else
		table->prev_table->next_table = table->next_table;
	if (table->next_table)
		table->next_table->prev_table = table->prev_table;
	munmap(table, table->mapping_size);
}
