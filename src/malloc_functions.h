/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_functions.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/02 18:54:56 by tvermeil          #+#    #+#             */
/*   Updated: 2017/03/30 14:10:51 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_FUNCTIONS_H
# define MALLOC_FUNCTIONS_H

# include "libft.h"
# include "malloc.h"
# include "malloc_structures.h"
# include <sys/mman.h>

# define OFFSET_OF(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
# define MAX(A, B) (A > B ? A : B)
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
int						table_has_room_for(t_table *t, enum e_entry_type type);

/*
** entry.c
*/
t_buf_location			save_buffer_to_tables(t_buffer buf);
t_map_location			save_mapping_to_tables(t_mapping map);
void					remove_mapping_from_tables(t_map_location loc);
void					remove_buffer_from_tables(t_buf_location loc);

/*
** mapping.c
*/
t_alloc_loc_reduced		find_best_mapping_for_size(size_t size);

/*
** delete_mapping.c
*/
void					try_delete_mapping(t_map_location m);

/*
** len_counters.c
*/
void					recompute_best_len(t_mapping *map);

/*
** search.c
*/
t_table					*find_table_of_buffer(t_buffer *buf);
void					*get_address_of_loc(t_alloc_loc_reduced loc);
t_alloc_location		find_buffer_in_tables(void *buf_addr);
t_mapping				*find_mapping_of_first_buffer(t_buffer *buf);

/*
** buffer.c
*/
int						create_allocation(t_alloc_loc_reduced alloc,
		size_t desired_size);
void					free_buffer_at(t_alloc_location loc,
		int should_try_free_mapping);
void					*resize_buffer(t_alloc_loc_reduced loc, size_t new_size);

/*
** location_utils.c
*/
t_alloc_loc_reduced		to_reduced_location(t_alloc_location loc);

/*
** mutex.c
*/
void					*unlock_mutex(void *retrurn_value);
void					lock_mutex(void);

#endif
