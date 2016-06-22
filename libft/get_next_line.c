/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/04 13:13:20 by fsidler           #+#    #+#             */
/*   Updated: 2016/06/21 19:34:19 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		ft_read(char **str, int fd)
{
	int		ret;
	char	*s;
	char	buf[BUFF_SIZE + 1];

	if ((ret = read(fd, buf, BUFF_SIZE)) == -1)
		return (-1);
	buf[ret] = '\0';
	s = *str;
	*str = ft_strjoin(*str, buf, 'N');
	if (*s != 0)
		;//free(s);
	return (ret);
}

static int		ft_get_line(char **str, char **line, char *s)
{
	int		i;
	char	*join;

	i = 0;
	if (*s == '\n')
		i = 1;
	*s = 0;
	*line = ft_strjoin("", *str, 'N');
	if (i == 0 && ft_strlen(*str) != 0)
	{
		*str = ft_strnew(1);
		return (1);
	}
	else if (i == 0 && !(ft_strlen(*str)))
		return (0);
	join = *str;
	*str = ft_strjoin(s + 1, "", 'N');
	//free(join);
	return (i);
}

int				get_next_line(int const fd, char **line)
{
	int			ret;
	char		*s;
	static char	*str;

	if (str == 0)
		str = "";
	if (!line || BUFF_SIZE < 1)
		return (-1);
	ret = BUFF_SIZE;
	while (line)
	{
		s = str;
		while (*s || ret < BUFF_SIZE)
		{
			if (*s == '\n' || *s == 0 || *s == -1)
				return (ft_get_line(&str, line, s));
			s++;
		}
		ret = ft_read(&str, fd);
		if (ret == -1)
			return (-1);
	}
	return (0);
}
