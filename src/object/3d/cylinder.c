/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scros <scros@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/17 14:12:21 by scros             #+#    #+#             */
/*   Updated: 2021/02/09 15:14:29 by scros            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int test_intersect(double t[2], double *current_z)
{
	int retvalue;

	retvalue = 0;
	if ((t[0] > 0) && (t[0] < *(current_z) || *(current_z) == t[0]))
	{
		*(current_z) = t[0];
		retvalue = 1;
	}
	if (!(t[0] == t[1]) && (t[1] > 0) && (t[1] < *(current_z) || *(current_z) == t[1]))
	{
		*(current_z) = t[1];
		retvalue = 1;
	}
	return (retvalue);
}

int intersect_cylinder(t_object *cp, t_ray *r, double *current_z)
{
    t_vector3 eye = vec3_subv(*(r->origin), cp->position);
    double a = vec3_dotv(r->direction, r->direction) - pow(vec3_dotv(r->direction, cp->rotation), 2);
    double b = 2 * (vec3_dotv(r->direction, eye) - vec3_dotv(r->direction, cp->rotation) * vec3_dotv(eye, cp->rotation));
    double c = vec3_dotv(eye, eye) - pow(vec3_dotv(eye, cp->rotation), 2) - cp->data.cylinder.radius * cp->data.cylinder.radius;
    double t[2];
    double delta;
    delta = sqrt((b * b) - (4 * a * c));
    if (delta < 0)
        return (0);
    t[0] = (-b - (delta)) / (2 * a);
    t[1] = (-b + (delta)) / (2 * a);
    return (test_intersect(t, current_z));
}

int			intersect_side(t_object *object, t_ray *ray, double z) {
	ray->length = z;
	ray->phit = vec3_muld(ray->direction, z);
	t_ray to_bot;
	to_bot.direction = object->rotation;
	to_bot.origin = &(ray->phit);
	if (!intersect_plan(object->data.cylinder.position2, object->rotation, &to_bot))
		return 0;
	to_bot.direction = vec3_negate(object->rotation);
	if (!intersect_plan(object->position, object->rotation, &to_bot))
		return 0;
	t_vector3 point = vec3_addv(vec3_muld(object->rotation, to_bot.length), object->position);
	ray->nhit = vec3_normalize(vec3_subv(ray->phit, point));
	return 1;
}

int			intersect_base(t_vector3 position, t_vector3 rotation, t_ray *ray, float radius) {
	t_vector3	v;
	float		d2;
	t_vector3	p;

	if (intersect_plan(position, rotation, ray))
	{
		p = vec3_muld(ray->direction, ray->length);
		p = vec3_addv(p, *(ray->origin));
		ray->phit = p;
		ray->nhit = rotation;
		if (vec3_dotv(rotation, ray->direction) > 0)
			ray->nhit = vec3_negate(ray->nhit);
		v = vec3_subv(ray->phit, position);
		d2 = vec3_length_squared(v);
		if (d2 <= radius * radius)
			return 1;
	}
	return 0;
}

int			collides_cylinder(void *arg1, void *arg2)
{
	t_ray		*ray;
	t_object	*object;
	double		z;
	
	object = arg1;
	ray = arg2;
	z = INFINITY;
	if (!intersect_cylinder(object, ray, &z))
		return 0;
	if (intersect_side(object, ray, z))
		return 1;
	if (intersect_base(object->position, object->rotation, ray, object->data.cylinder.radius))
		return 1;
	if (intersect_base(object->data.cylinder.position2, object->rotation, ray, object->data.cylinder.radius))
		return 1;
	return 0;
}

t_object		*new_cylinder(float diametre, float height, t_vector3 position, t_vector3 rotation, t_color color)
{
	t_object	*object;

	if (!(object = new_default_object(vec3_subv(position, vec3_muld(rotation, height / 2)), rotation, color, &collides_cylinder)))
		return (NULL);
	object->data.cylinder.radius = diametre / 2;
	object->data.cylinder.height = height;
	object->data.cylinder.position2 = vec3_addv(position, vec3_muld(rotation, height / 2));
	return (object);
}
