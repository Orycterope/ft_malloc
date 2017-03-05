/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   len_counters.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/04 14:28:02 by tvermeil          #+#    #+#             */
/*   Updated: 2017/03/04 15:16:13 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "malloc_functions.h"
#include "global.h"

/*
** Recomputes the new table->shortest_free_buffer and table->longest.
** buf is a buffer pointer from which table is obtained
*/
void	recompute_best_len(t_mapping *map)
{
	t_buffer		*p;

	p = map->buffers;
	map->shortest_free_buffer = 0;
	map->longest_free_buffer = 0;
	while (p)
	{
		if (p->alloc_status == BUFFER_FREE)
		{
			if (p->len > map->longest_free_buffer
					|| map->longest_free_buffer == 0)
				map->longest_free_buffer = p->len;
			if (p->len < map->shortest_free_buffer
					|| map->shortest_free_buffer == 0)
				map->shortest_free_buffer = p->len;
		}
		p = p->next_buf;
	}
}
