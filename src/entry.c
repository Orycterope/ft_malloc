/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entry.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 15:14:01 by tvermeil          #+#    #+#             */
/*   Updated: 2017/03/05 19:55:55 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "malloc_structures.h"
#include "malloc_functions.h"
#include "global.h"

/*
** Saves data to table->first_free_entry and updates linked lists,
** table->occupied_count and table->first_free_entry.
*/
static t_table_entry	*save_entry(union u_entry_data data, t_table *table,
		enum e_entry_type e_type)
{
	t_table_entry	*entry;
	t_table_entry	*p;

	entry = table->first_free_entry;
	table->first_free_entry = entry->next_entry;
	if (table->first_free_entry)
		table->first_free_entry->prev_entry = NULL;
	entry->prev_entry = NULL;
	entry->next_entry = NULL;
	if (e_type == ENTRY_MAPPING)
	{
		p = table->first_mapping_entry;
		while (p && p->next_entry)
			p = p->next_entry;
		if (p == NULL)
			table->first_mapping_entry = entry;
		else
			p->next_entry = entry;
		entry->prev_entry = p; /* p can be NULL */
	}
	table->occupied_count++;
	entry->table = table;
	entry->entry_data = data;
	return (entry);
}

/*
** Finds the first free entry and add -content- to it.
** returns a pointer to that new location
** If no entry is free, mmap a new table.
** If mmap fails, returns NULL.
** When called g_malloc_infos.tables must already be set.
*/
t_table_entry			*save_data_to_tables(union u_entry_data data,
		enum e_entry_type e_type)
{
	t_table_entry	*entry;
	t_table			*table;

	table = g_malloc_infos.tables;
	entry = NULL;
	while (table)
	{
		if (table->first_free_entry != NULL)
		{
			entry = save_entry(data, table, e_type);
			break;
		}
		if (table->next_table == NULL)
			table->next_table = create_table();
		table = table->next_table;
	}
	return (entry);
}

/*
** Remove a table_entry from the list it's on, and add it to the free list,
** and updates table->occupied_count.
*/
void					remove_entry_from_tables(union u_entry_data *data)
{
	t_table_entry	*entry;

	entry = (t_table_entry *)(data - OFFSET_OF(struct s_table_entry, entry_data));
	//		container_of(data, struct s_table_entry, entry_data);
	if (entry->next_entry)
		entry->next_entry->prev_entry = entry->prev_entry;
	if (entry->prev_entry)
		entry->prev_entry->next_entry = entry->next_entry;
	if (entry == entry->table->first_mapping_entry)
		entry->table->first_mapping_entry = entry->next_entry;
	entry->table->occupied_count--;
	entry->prev_entry = NULL;
	entry->next_entry = entry->table->first_free_entry;
	entry->table->first_free_entry = entry;
	ft_bzero(&entry->entry_data, sizeof(union u_entry_data)); // debug, remove me
}
