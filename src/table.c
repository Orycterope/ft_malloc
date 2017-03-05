/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/02 17:12:41 by tvermeil          #+#    #+#             */
/*   Updated: 2017/03/05 17:58:36 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_structures.h"
#include "malloc_settings.h"
#include "malloc_functions.h"
#include "global.h"

static void	init_free_entry(t_table *header, int index)
{
	t_table_entry	*entry;

	entry = (void *)header->first_free_entry + index * sizeof(t_table_entry);
	//entry->entry_status = ENTRY_FREE;
	if (index == 0)
		entry->prev_entry = NULL;
	else
		entry->prev_entry = entry - 1;
	if (index + 1 == header->table_len)
		entry->next_entry = NULL;
	else
		entry->next_entry = entry + 1;
}

t_table		*create_table(void)
{
	size_t	mapping_size;
	t_table	*header;
	int		i;

	mapping_size = round_upper_page_size(sizeof(t_table)
			+ sizeof(t_table_entry) * MIN_ENTRIES_PER_TABLE);
	header = (t_table *)mmap(NULL, mapping_size, MMAP_PARAMETERS);
	header = (header == MAP_FAILED) ? NULL : header;
	if (header != NULL)
	{
		ft_bzero(header, sizeof(*header));
		header->mapping_size = mapping_size;
		header->table_len =
			(mapping_size - sizeof(*header)) / sizeof(t_table_entry);
		header->first_free_entry = (t_table_entry *)((void *)header + sizeof(*header));
		i = -1;
		while (++i < header->table_len)
			init_free_entry(header, i);
	}
	return (header);
}

void		try_free_table(t_table *table)
{
	if (table->occupied_count != 0)
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
