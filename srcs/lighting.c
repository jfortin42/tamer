/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lighting.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsidler <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/18 15:48:31 by fsidler           #+#    #+#             */
/*   Updated: 2016/06/22 16:14:53 by fsidler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int				ft_check_objects(t_mlx *mlx, t_obj *object)
{
	object = mlx->light;
	while (object != NULL)
	{
		if (!(object->col.red >= 0 && object->col.red <= 255) ||
				!(object->col.green >= 0 && object->col.green <= 255) ||
				!(object->col.blue >= 0 && object->col.blue <= 255))
			return (-1);
		object = object->next;
	}
	object = mlx->obj;
	while (object != NULL)
	{
		if (!(object->col.red >= 0 && object->col.red <= 255) ||
				!(object->col.green >= 0 && object->col.green <= 255) ||
				!(object->col.blue >= 0 && object->col.blue <= 255))
			return (-1);
		if (object->type == 2 || object->type == 4)
		{
			if (object->size < 0)
				return (-1);
		}
		object = object->next;
	}
	return (0);
}

static t_vec	ft_norm(t_mlx *mlx, t_obj *tmp, t_vec pos)
{
	t_vec	norm;
	t_vec	temp;
	t_vec	temp2;

	if (tmp->type == 1 || tmp->type == 2)
	{
		temp = ft_vectorscale(&tmp->rot,
		(ft_vectordot(&mlx->ray_dir, &tmp->rot) * mlx->t0
		+ ft_vectordot(&mlx->dist, &tmp->rot)));
		if (tmp->type == 1)
			temp = ft_vectorscale(&temp, (1 + pow(tan(tmp->size), 2)));
		temp2 = ft_vectorsub(&pos, &tmp->pos);
		norm = ft_vectorsub(&temp2, &temp);
	}
	else if (tmp->type == 3)
		norm = tmp->rot;
	if (tmp->type == 4)
		norm = ft_vectorsub(&pos, &tmp->pos);
	ft_vectornorm(&norm);
	return (norm);
}

static int		ft_shadow(t_mlx *mlx, t_obj *tmp, t_obj *light, t_vec pos)
{
	t_obj	*node;
	t_vec	dist;

	node = mlx->obj;
	dist = ft_vectorsub(&light->pos, &pos);
	mlx->t = sqrtf(ft_vectordot(&dist, &dist));
	ft_vectornorm(&dist);
	while (node != NULL)
	{
		if (node != tmp)
		{
			if (node->type == 1)
				mlx->a = ft_inter_cone(mlx, node, dist, pos);
			else if (node->type == 2)
				mlx->a = ft_inter_cylinder(mlx, node, dist, pos);
			else if (node->type == 3)
				mlx->a = ft_inter_plane(mlx, node, dist, pos);
			else if (node->type == 4)
				mlx->a = ft_inter_sphere(mlx, node, dist, pos);
			if (mlx->a > 0.0001 && mlx->a < mlx->t)
				return (1);
		}
		node = node->next;
	}
	return (0);
}

static float	ft_spec(t_mlx *mlx, t_vec dist)
{
	float	spec;
	float	tmp;
	t_vec	ref;

	spec = 0.0;
	ref = ft_vectorscale(&mlx->norm, (2.0 * ft_vectordot(&mlx->norm, &dist)));
	ref = ft_vectorsub(&dist, &ref);
	if ((tmp = ft_vectordot(&ref, &mlx->ray_dir)) > 0.0)
	{
		spec = pow(tmp, 30.0) * 1.0;
		spec = ft_clamp(spec, 0.0, 1.0);
	}
	return (spec);
}

float			*ft_lambert(t_mlx *mlx, t_obj *tmp, t_obj *light, float *tab)
{
	t_vec	pos;
	t_vec	dist;
	float	d;

	pos = (t_vec){mlx->cam_pos.x + mlx->t * mlx->ray_dir.x, mlx->cam_pos.y +
		mlx->t * mlx->ray_dir.y, mlx->cam_pos.z + mlx->t * mlx->ray_dir.z};
	mlx->norm = ft_norm(mlx, tmp, pos);
	while (light != NULL)
	{
		LAMBERT = 0.2;
		dist = ft_vectorsub(&light->pos, &pos);
		d = ft_clamp((1.0 / sqrtf(sqrtf(ft_vectordot(&dist, &dist)))), 0., 1.);
		ft_vectornorm(&dist);
		if (ft_shadow(mlx, tmp, light, pos) == 0)
			LAMBERT += ft_clamp(ft_vectordot(&dist, &mlx->norm), 0.0, 1.0);
		else
			LAMBERT -= 0.03;
		tab = ft_endlight(tmp, light, tab, d);
		tab[0] += (mlx->spec == 1 && tmp->type != 3) ? ft_spec(mlx, dist) : 0.;
		tab[1] += (mlx->spec == 1 && tmp->type != 3) ? ft_spec(mlx, dist) : 0.;
		tab[2] += (mlx->spec == 1 && tmp->type != 3) ? ft_spec(mlx, dist) : 0.;
		light = light->next;
	}
	return (tab);
}
