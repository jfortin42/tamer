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

void		ft_put_pixel(t_th *mlx, int x, int y, int color)
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

static void	ft_set_ray(t_th *mlx, double u, int x, int y)
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
	int		x;
	int		y;
	t_th	*th;

	th = (t_th *)malloc(sizeof(t_th));
	y = 0;
	while (y < WIN_H)
	{
		x = 0;
		while (x < WIN_W)
		{
			ft_put_pixel(th, x, y, BLACK);
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->img, 0, 0);
	return (0);
}

static int	ft_raytrace(t_th *mlx, t_obj *node, int x, int y)
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

void	copy_vec(t_vec *dest, t_vec *src)
{
	dest->x = src->x;
	dest->y = src->y;
	dest->z = src->z;
}

t_obj	*copy_obj(t_obj *obj)
{
	t_obj	*newlist;

	newlist = (t_obj *)malloc(sizeof(t_obj));
	if (obj && newlist)
	{
		newlist->type = obj->type;
		newlist->size = obj->size;
		copy_vec(&newlist->rot, &obj->rot);
		copy_vec(&newlist->pos, &obj->pos);
		newlist->col.red = obj->col.red;
		newlist->col.green = obj->col.green;
		newlist->col.blue = obj->col.blue;
		newlist->next = NULL;
		if (newlist && obj->next)
			newlist->next = copy_obj(obj->next);
		return (newlist);
	}
	return (NULL);
}

void	ft_copy(t_mlx *mlx, t_th *th)
{
	th->spec = mlx->spec;
	copy_vec(&th->cam_pos, &mlx->cam_pos);
	copy_vec(&th->cam_dir, &mlx->cam_dir);
	copy_vec(&th->ray_dir, &mlx->ray_dir);
	th->light = copy_obj(mlx->light);
	th->obj = copy_obj(mlx->obj);
	th->bpp = mlx->bpp;
	th->size_line = mlx->size_line;
	th->d = mlx->d;
}

void	*my_thread_process(void *arg)
{
	t_tab_th	*tab;
	t_th		*th;
	t_obj		*node;
	int			x;
	int			y;

	tab = (t_tab_th *)arg;
	node = tab->mlx->obj;
	th = (t_th *)malloc(sizeof(t_th));
	ft_copy(tab->mlx, th);
	y = 0;
	while (y < WIN_H)
	{
		x = WIN_W * tab->i / NB_THREAD;
		while (x < WIN_W * (tab->i + 1) / NB_THREAD)
			ft_raytrace(th, node, x++, y);
		y++;
	}
	free(th);
	pthread_exit(0);
}

int			ft_draw(t_mlx *mlx)
{
	pthread_t	th[NB_THREAD];
	void		*ret;
	t_tab_th	tab[NB_THREAD];
	int			i;

	i = -1;
	ft_clear(mlx);
	while (++i < NB_THREAD)
	{
		tab[i].i = i;
		tab[i].mlx = mlx;
		pthread_create(&th[i], NULL, my_thread_process, &tab[i]);
	}
	i = -1;
	while (++i < NB_THREAD)
		(void)pthread_join(th[i], &ret);
	mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->img, 0, 0);
	return (0);
}
