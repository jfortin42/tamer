/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/22 14:08:15 by fsidler           #+#    #+#             */
/*   Updated: 2016/06/22 14:15:18 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int		ft_free_arg(t_mlx *mlx, char *buf, int r)
{
	if (r == 0)
		free(buf);
	else if (r == 1)
	{
		ft_putendl("error: Dynamic memory allocation failed");
		free(mlx->scene);
	}
	else if (r == 2)
	{
		ft_putendl("error: scene descriptor incorrect or incomplete");
		free(mlx->scene);
	}
	return (-1);
}

int		ft_free_lists(t_mlx *mlx)//ajouter tmp = NULL apres les free(tmp)?
{
	t_obj	*tmp;

	while (mlx->obj != NULL)
	{
		tmp = mlx->obj;
		while (tmp->next != NULL)
			tmp = tmp->next;
		free(tmp);
	}
	while (mlx->light != NULL)
	{
		tmp = mlx->light;
		while (tmp->next != NULL)
			tmp = tmp->next;
		free(tmp);
	}
	return (-1);
}
