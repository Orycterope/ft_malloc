/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_functions.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/02 18:54:56 by tvermeil          #+#    #+#             */
/*   Updated: 2017/03/06 19:25:34 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_FUNCTIONS_H
# define MALLOC_FUNCTIONS_H

# include "libft.h"
# include "malloc.h"
# include "malloc_structures.h"
# include <sys/mman.h>

# define OFFSET_OF(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
# define MIN(A, B) (A < B ? A : B)

/*
** page.c
*/
size_t					round_upper_page_size(size_t size);
enum e_mapping_type		get_mapping_type(size_t size);
size_t					get_page_mapping_size(size_t desired);

/*
** table.c
*/
void					try_free_table(t_table *table);
t_table					*create_table(void);

/*
** entry.c
*/
t_table_entry			*save_data_to_tables(union u_entry_data data,
		enum e_entry_type e_type);
void					remove_entry_from_tables(union u_entry_data *data);

/*
** mapping.c
*/
t_alloc_location		find_best_mapping_for_size(size_t size);

/*
** len_counters.c
*/
void					recompute_best_len(t_mapping *map);

/*
** search.c
*/
t_alloc_location		find_buffer_in_tables(void *buf);
void					*get_address_of_loc(t_alloc_location loc);

/*
** buffer.c
*/
int						create_allocation(t_alloc_location alloc,
		size_t desired_size);
void					free_buffer_at(t_alloc_location alloc);

#endif
