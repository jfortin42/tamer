/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fitoa.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/06 17:54:47 by fsidler           #+#    #+#             */
/*   Updated: 2016/06/06 18:12:22 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static long		ft_digitnb(float n)
{
	long		size;

	n *= 10;
	if (n == 0.0)
		return (1);
	size = 1;
	if (n < 0)
	{
		size++;
		n = -n;
	}
	while (n >= 10.0)
	{
		n /= 10.0;
		size++;
	}
	return (size++);
}

static int		ft_sign(float n)
{
	if (n < 0)
		return (1);
	return (0);
}

char			*ft_fitoa(float n)
{
	float		n_long;
	long		length;
	long		tmp;
	char		*fresh;

	n_long = n * 10.0;
	length = (n < 1.0 && n > -1.0) ? 2 : ft_digitnb(n);
	fresh = (char *)malloc((length + 2) * sizeof(char));
	if (!fresh)
		return (NULL);
	fresh[length + 1] = '\0';
	if (n_long < 0.0)
		n_long = -n_long;
	tmp = length;
	while (length + 1 >= 0)
	{
		if (length == tmp - 1)
			fresh[length] = '.';
		else if ((fresh[length] = ((int)n_long % 10) + '0') != 0)
			n_long /= 10;
		length--;
	}
	if (ft_sign(n))
		fresh[0] = '-';
	return (fresh);
}
