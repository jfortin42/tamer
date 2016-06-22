/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/07 16:20:03 by fsidler           #+#    #+#             */
/*   Updated: 2016/06/22 15:44:21 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void		ft_put_pixel(t_mlx *mlx, int x, int y, int color)
{
	int				i;
	unsigned int	p;

	i = 0;
	p = x * (mlx->bpp / 8) + y * (mlx->size_line);
	while (i < (mlx->bpp / 8))
	{
		mlx->d[p + i] = color;
		color >>= 8;
		i++;
	}
}

static void	ft_set_ray(t_mlx *mlx, double u, int x, int y)
{
	double	v;

	u = (WIN_W - (double)x * 2.0) / WIN_H;
	v = (WIN_H - (double)y * 2.0) / WIN_W;
	K = ft_vectorsub(&mlx->cam_dir, &mlx->cam_pos);
	ft_vectornorm(&K);
	I = ft_vectorcross(&K, &(t_vec){0.0, 1.0, 0.0});
	ft_vectornorm(&I);
	J = ft_vectorcross(&I, &K);
	mlx->ray_dir = (t_vec){u * I.x + v * J.x + FOV * K.x, u * I.y + v * J.y
		+ FOV * K.y, u * I.z + v * J.z + FOV * K.z};
	ft_vectornorm(&mlx->ray_dir);
}

static int	ft_clear(t_mlx *mlx)
{
	int	x;
	int	y;

	y = 0;
	while (y < WIN_H)
	{
		x = 0;
		while (x < WIN_W)
		{
			ft_put_pixel(mlx, x, y, BLACK);
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->img, 0, 0);
	return (0);
}

static int	ft_raytrace(t_mlx *mlx, t_obj *node, int x, int y)
{
	t_obj	*tmp;
	t_obj	*light;
	float	*tab;
	double	u;

	if (!(tab = (float *)malloc(sizeof(float) * 4)))
		return (-1);
	u = 0.0;
	tmp = NULL;
	light = mlx->light;
	ft_set_ray(mlx, u, x, y);
	tmp = ft_intersection(mlx, node, mlx->ray_dir, mlx->cam_pos);
	if (!tmp)
		return (0);
	tab[0] = 0;
	tab[1] = 0;
	tab[2] = 0;
	tab = ft_lambert(mlx, tmp, light, tab);
	tab[0] = ft_clamp(tab[0], 0.0, 1.0);
	tab[1] = ft_clamp(tab[1], 0.0, 1.0);
	tab[2] = ft_clamp(tab[2], 0.0, 1.0);
	ft_put_pixel(mlx, x, y, (((int)(tab[0] * 255) & 0xff) << 16) +
			(((int)(tab[1] * 255) & 0xff) << 8) + ((int)(tab[2] * 255) & 0xff));
	free(tab);
	return (0);
}

int			ft_draw(t_mlx *mlx)
{
	t_obj	*node;
	int		x;
	int		y;

	ft_clear(mlx);
	node = mlx->obj;
	y = 0;
	while (y < WIN_H)
	{
		x = 0;
		while (x < WIN_W)
		{
			if (ft_raytrace(mlx, node, x, y) == -1)
				return (ft_free_arg(mlx, NULL, 3));
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->img, 0, 0);
	return (0);
}
