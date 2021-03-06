/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_structures.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/02 15:55:45 by tvermeil          #+#    #+#             */
/*   Updated: 2017/03/30 18:40:52 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_STRUCTURES_H
# define MALLOC_STRUCTURES_H

# include "malloc.h"
# include <pthread.h>

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
	struct s_buffer			*next_buf;
	struct s_buffer			*prev_buf;
	enum e_buffer_status	alloc_status;
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
** Describes the header of a table of t_mapping and t_buffers
** The structure of a table is as follow :
** t-------------------------t
** | h---------------------h |
** | |    t_table header   | |
** | h---------------------h |
** | m-----m m-----m m-----m |
** | | map | | map | | map | |
** | m-----m m-----m m-----m |   occupied_maps = 4
** | m-----m                 |
** | | map |    |            |
** | m-----m    V            |
** |                         |
** |                         |
** |                         |
** |            ^    b-----b |
** |            |    | buf | |
** |                 b-----b |
** | b-----b b-----b b-----b |
** | | buf | | buf | | buf | |   occupied_buffers = 7
** | b-----b b-----b b-----b |
** | b-----b b-----b b-----b |
** | | buf | | buf | | buf | |
** | b-----b b-----b b-----b |
** t-------------------------t
** mapping_size : used to unmap
** occupied_count : the number of occupied entries in this table
**					if count is 0 we can free this table
** next/prev table : chained list of tables
*/
typedef struct				s_table
{
	size_t					mapping_size;
	int						occupied_maps;
	int						occupied_buffers;
	struct s_table			*next_table;
	struct s_table			*prev_table;
}							t_table;

/*
** Used as a global variable
*/
typedef struct				s_genereal_struct
{
	size_t					page_size;
	struct s_table			*tables;
	pthread_mutex_t			mutex;
}							t_general_struct;

/*
** A struct to pass a mapping and its table
*/
typedef struct				s_map_location
{
	t_mapping				*map;
	t_table					*table;
}							t_map_location;

/*
** A struct to pass a buffer and its table
*/
typedef struct				s_buf_location
{
	t_buffer				*buf;
	t_table					*table;
}							t_buf_location;

/*
** A struct to save an allocation location
*/
typedef struct				s_alloc_location
{
	t_map_location			m;
	t_buf_location			b;
}							t_alloc_location;

typedef struct				s_alloc_loc_reduced
{
	t_mapping				*map;
	t_buffer				*buf;
}							t_alloc_loc_reduced;

/*
** Enum to pass as parameter
*/
enum	e_entry_type
{
	ENTRY_MAPPING,
	ENTRY_BUFFER
};

#endif
