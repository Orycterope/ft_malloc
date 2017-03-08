/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   page.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/03 17:42:26 by tvermeil          #+#    #+#             */
/*   Updated: 2017/03/08 04:33:15 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "malloc_functions.h"
#include "malloc_settings.h"
#include "global.h"

/*
** Returns the type TINY/SMALL/LARGE corresponding to -size-
*/
inline enum e_mapping_type	get_mapping_type(size_t size)
{
	if (size <= TINY_SIZE)
		return (MAPPING_TINY);
	else if (size <= SMALL_SIZE)
		return (MAPPING_SMALL);
	else
		return (MAPPING_LARGE);
}

size_t						get_page_mapping_size(size_t desired)
{
	enum e_mapping_type	type;
	size_t				size;

	type = get_mapping_type(desired);
	if (type == MAPPING_TINY)
		size = MIN_BLOCK_PER_MAPPING * TINY_SIZE;
	else if (type == MAPPING_SMALL)
		size = MIN_BLOCK_PER_MAPPING * SMALL_SIZE;
	else
		size = desired;
	return (round_upper_page_size(size));
}

inline size_t				round_upper_page_size(size_t size)
{
	if (size % g_malloc_infos.page_size == 0)
		return (size);
	else
		return (size + g_malloc_infos.page_size
				- (size % g_malloc_infos.page_size));
}
