/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/29 19:35:25 by tvermeil          #+#    #+#             */
/*   Updated: 2017/03/29 21:09:55 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "malloc_functions.h"
#include "malloc_structures.h"
#include "global.h"

static int	alloc_is_mine(void *addr, t_list **lists)
{
	int		list_num;
	t_list	*p;

	list_num = -1;
	while (++list_num < 3)
	{
		p = lists[list_num];
		while (p)
		{
			if (addr == p)
				return (1);
			p = p->next;
		}
	}
	return (0);
}

static void	add_map_to_list(t_mapping *m, t_list **list)
{
	t_list		*parser;
	t_list		*new;

	new = ft_lstnew_nocpy((void *)m, 0);
	parser = *list;
	while (parser && parser->next)
	{
		if (((t_mapping *)parser->content)->map_addr > m->map_addr)
			break ;
		parser = parser->next;
	}
	if (parser == NULL
			|| (((t_mapping *)list[0]->content)->map_addr > m->map_addr))
		ft_lstadd(list, new);
	else
		ft_lstadd(&parser->next, new);
}

static void	fill_mapping_lists(t_list **lists)
{
	t_table		*t;
	t_mapping	*m;
	int			map_counter;
	int			type;

	t = g_malloc_infos.tables;
	while (t != NULL)
	{
		m = (t_mapping *)((void *)t + sizeof(t_table));
		map_counter = -1;
		while (++map_counter < t->occupied_maps)
		{
			if (m->mapping_type == MAPPING_TINY)
				type = 0;
			else if (m->mapping_type == MAPPING_SMALL)
				type = 1;
			else
				type = 2;
			add_map_to_list(m++, &lists[type]);
		}
		t = t->next_table;
	}
}

static void	show_allocs_of_mapping(t_mapping *m, t_list **lists)
{
	t_buffer	*buf;
	void		*addr;

	ft_printf("-> mapping %p - %p, %ld octets, %d pages\n",
			m->map_addr, m->map_addr + m->map_len - 1,
			m->map_len, m->map_len / g_malloc_infos.page_size);
	buf = m->buffers;
	addr = m->map_addr;
	while (buf)
	{
		if (buf->alloc_status == BUFFER_ALLOCATED
				&& !(alloc_is_mine(addr, lists)))
			ft_printf("   -> buffer %p - %p, %ld octets\n",
				addr, addr + buf->len - 1, buf->len);
		addr += buf->len;
		buf = buf->next_buf;
	}
}

void		show_alloc_mem(void)
{
	t_list		*lists[3];
	int			list_num;
	t_list		*old;

	ft_bzero(&lists, 3 * sizeof(t_list *));
	fill_mapping_lists(lists);
	list_num = -1;
	while (++list_num < 3)
	{
		if (lists[list_num] == NULL)
			continue ;
		if (list_num == 0)
			ft_putstr("MALLOC_TINY :\n");
		else if (list_num == 1)
			ft_putstr("MALLOC_SMALL :\n");
		else
			ft_putstr("MALLOC_LARGE :\n");
		while (lists[list_num])
		{
			show_allocs_of_mapping((t_mapping*)lists[list_num]->content, lists);
			old = lists[list_num];
			lists[list_num] = lists[list_num]->next;
			free(old);
		}
	}
}
