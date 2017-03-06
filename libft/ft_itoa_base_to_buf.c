/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_base_to_buf.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/06 21:18:06 by tvermeil          #+#    #+#             */
/*   Updated: 2017/03/06 21:24:35 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "itoa_base.h"

static int	get_size(unsigned long long n, int base)
{
	int		c;

	c = 1;
	while ((n /= base) > 0)
		c++;
	return (c + 1);
}

char		*ft_itoa_base_to_buf(unsigned long long n, const char *charset,
		char *str, size_t max_len)
{
	int		base;
	int		len;

	base = ft_strlen(charset);
	len = get_size(n, base);
	if ((size_t)len > max_len)
		return (str);
	str[--len] = 0;
	while (len-- > 0)
	{
		str[len] = charset[n % base];
		n /= base;
	}
	return (str);
}
