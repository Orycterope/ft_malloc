/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy_overlap.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/08 05:11:34 by tvermeil          #+#    #+#             */
/*   Updated: 2017/03/08 05:23:15 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	*reverse_memcpy(void *dst, void *src, size_t len)
{
	if (dst == NULL || src == NULL)
		return (NULL);
	dst += len - 1;
	src += len - 1;
	while (len--)
		*(unsigned char *)dst-- = *(unsigned char *)src--;
	return (dst + 1);
}

void		*ft_memcpy_overlap(void *dst, void *src, size_t len)
{
	if (src < dst)
		return (ft_memcpy(dst, src, len));
	else
		return (reverse_memcpy(dst, src, len));
}
