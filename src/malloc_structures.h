/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_structures.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/02 15:55:45 by tvermeil          #+#    #+#             */
/*   Updated: 2017/03/04 15:11:52 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_STRUCTURES_H
# define MALLOC_STRUCTURES_H

# include "malloc.h"

enum						e_entry_status //
{
	ENTRY_FREE,
	ENTRY_OCCUPIED
};

enum						e_buffer_status
{
	BUFFER_FREE,
	BUFFER_ALLOCATED
};

/*
** Describes an array of -len- consecutives bytes.
** len : the number of consecutive bytes described by this struct
** alloc_status : allocated or free
** next/prev :	pointer to the next buffer of this mapping.
** 				Can point to a table_entry in an other table.
*/
typedef struct				s_buffer
{
	size_t					len;
	enum e_buffer_status	alloc_status;
	struct s_buffer			*next_buf;
	struct s_buffer			*prev_buf;
}							t_buffer;

enum						e_mapping_type
{
	MAPPING_TINY,
	MAPPING_SMALL,
	MAPPING_LARGE
};

/*
** Describes the content of a mapping obtained by mmap
** map_len : use to unmap
** longest_free_buffer : faster parsing
** shortest_free_buffer : faster parsing
** mapping_type : TINY / SMALL / BIG
** buffers : chained list of t_buffers, potentially in an other table
*/
typedef struct				s_mapping
{
	void					*map_addr;
	size_t					map_len;
	size_t					longest_free_buffer;
	size_t					shortest_free_buffer;
	enum e_mapping_type		mapping_type;
	t_buffer				*buffers;
}							t_mapping;

/*
** We want to treat table entries the same way regardless of content,
** so we put them in an union in the t_table_entry struct.
*/
enum						e_entry_type
{
	ENTRY_MAPPING,
	ENTRY_BUFFER
};

union						u_entry_data
{
	t_mapping				map;
	t_buffer				buf;
};

/*
** Describes a table entry, containing either a t_mapping or a t_buffer
** table :	a pointer back to the table header struct
**			used by buffers with container_of()
** entry_status : free or occupied
** next/prev : when free :		pointer to the next free entry in this table
**			   when t_mapping :	pointer to the next t_mapping in this table
**			   when t_buffer :	not used
** entry_type : t_mapping or t_buffer
** entry_data : the union
*/
typedef struct				s_table_entry
{
	struct s_table			*table;
	//enum e_entry_status	entry_status;
	struct s_table_entry	*next_entry;
	struct s_table_entry	*prev_entry;
	//enum e_entry_type		entry_type;
	union u_entry_data		entry_data;
}							t_table_entry;

/*
** Describes the header of a table of t_mapping and t_buffers
** mapping_size : used to unmap
** table_len : the number of entries in this table
** occupied_count : the number of occupied entries in this table
**					if count is 0 we can free this table
** first_free_entry :		chained list of all free entries of this table
** first_mapping_entry :	chained list of all t_mapping entries of this table
** next/prev table : chained list of tables
*/
typedef struct				s_table
{
	size_t					mapping_size;
	int						table_len;
	int						occupied_count;
	t_table_entry			*first_free_entry;
	t_table_entry			*first_mapping_entry;
	struct s_table			*next_table;
	struct s_table			*prev_table;
}							t_table;

/*
** Used as a global variable
*/
typedef struct
{
	size_t					page_size;
	struct s_table			*tables;
}							t_general_struct;

/*
** A struct to save an allocation best location
*/
typedef struct
{
	t_mapping				*map;
	t_buffer				*buf;
}							t_alloc_location;

#endif
