/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_settings.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/02 17:31:56 by tvermeil          #+#    #+#             */
/*   Updated: 2017/03/05 21:08:46 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_SETTINGS_H
# define MALLOC_SETTINGS_H

# include <sys/mman.h>

# define MMAP_PARAMETERS PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0
# define MIN_ENTRIES_PER_TABLE 100
# define TINY_SIZE 32
# define SMALL_SIZE 128

#endif
