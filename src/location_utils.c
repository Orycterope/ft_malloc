/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/29 16:56:36 by tvermeil          #+#    #+#             */
/*   Updated: 2017/03/30 17:17:52 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "malloc_functions.h"
#include "malloc_structures.h"
#include "global.h"

/*
** Returns a reduced version of the location -loc-
*/

t_alloc_loc_reduced	to_reduced_location(t_alloc_location loc)
{
	t_alloc_loc_reduced	ret;

	ret.map = loc.m.map;
	ret.buf = loc.b.buf;
	return (ret);
}
