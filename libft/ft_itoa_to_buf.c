/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_to_buf.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvermeil <tvermeil@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/06 21:07:23 by tvermeil          #+#    #+#             */
/*   Updated: 2017/03/30 15:52:21 by tvermeil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_size(int n)
{
	int		c;

	c = 1;
	if (n < 0)
	{
		n = -n;
		c++;
	}
	while ((n /= 10) > 0)
		c++;
	return (c + 1);
}

char		*ft_itoa_to_buf(int n, char *str, size_t max_len)
{
	int		is_neg;
	int		len;

	if (n == -2147483648)
		return (ft_memcpy(str, "-2147483648", max_len));
	len = get_size(n);
	if (max_len < (size_t)len)
		return (str);
	is_neg = (n < 0) ? 1 : 0;
	n = (n < 0) ? -n : n;
	str[--len] = 0;
	while (len-- > 0)
	{
		str[len] = (n % 10) + '0';
		n /= 10;
	}
	if (is_neg)
		*str = '-';
	return (str);
}
