/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/06/22 14:08:15 by fsidler           #+#    #+#             */
/*   Updated: 2016/06/22 15:43:11 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int		ft_free_lists(t_mlx *mlx)
{
	t_obj	*fst;
	t_obj	*tmp;

	fst = mlx->obj;
	while (fst != NULL)
	{
		tmp = fst->next;
		free(fst);
		fst = tmp;
	}
	fst = mlx->light;
	while (fst != NULL)
	{
		tmp = fst->next;
		free(fst);
		fst = tmp;
	}
	return (-1);
}

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
	else if (r == 3)
	{
		ft_putendl("error: Dynamoc memory allocation failed");
		free(mlx->scene);
		ft_free_lists(mlx);
	}
	return (-1);
}
