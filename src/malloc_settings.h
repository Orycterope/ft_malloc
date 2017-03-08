/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_settings.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/02 17:31:56 by tvermeil          #+#    #+#             */
/*   Updated: 2017/03/08 04:38:22 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_SETTINGS_H
# define MALLOC_SETTINGS_H

# include <sys/mman.h>

# define MMAP_PARAMETERS PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0
# define MIN_ENTRIES_PER_TABLE 100
# define MIN_BLOCK_PER_MAPPING 128
# define TINY_SIZE (g_malloc_infos.page_size / 64)
# define SMALL_SIZE (g_malloc_infos.page_size)

#endif
