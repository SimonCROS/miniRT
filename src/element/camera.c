#include <stdlib.h>

#include "convert.h"
#include "matrix.h"
#include "vector3.h"

#include "element/camera.h"

static int	rot_deserialize(const char *str, t_vector3 *vector)
{
	t_vector3	rot;

	if (!vec3_deserialize(str, &rot))
		return (0);
	if (rot.x < -1 || rot.x > 1 ||
		rot.y < -1 || rot.y > 1 ||
		rot.z < -1 || rot.z > 1)
		return (0);
	*vector = rot;
	return (1);
}

t_camera	*parse_camera(t_list *data)
{
	t_vector3	pos;
	t_vector3	rot;
	int			fov;
	int			e;

	if (data->size != 4)
		return (NULL);
	e = 1;
	e = e && vec3_deserialize((char *)lst_get(data, 1), &pos);
	e = e && rot_deserialize((char *)lst_get(data, 2), &rot);
	e = e && ft_atoi_full((char *)lst_get(data, 3), &fov);
	if (!e || fov < 0 || fov > 180)
		return (NULL);
	return (new_camera(pos, rot, fov));
}

t_matrix44	look_at(t_vector3 from, t_vector3 to)
{
	static t_vector3	tmp = (t_vector3) { 0, 1, 0 };
	t_matrix44			camToWorld;
    t_vector3			forward;
    t_vector3			right;
    t_vector3			up;

	forward = vec3_normalize(vec3_subv(from, to));
	right = vec3_crossv(tmp, forward);
	if (vec3_length_squared(right) == 0)
		right = vec3_new(1, 0, 0);
	up = vec3_crossv(forward, right);
    camToWorld.e[0][0] = right.x;
    camToWorld.e[0][1] = right.y;
    camToWorld.e[0][2] = right.z;
    camToWorld.e[1][0] = up.x;
    camToWorld.e[1][1] = up.y;
    camToWorld.e[1][2] = up.z;
    camToWorld.e[2][0] = forward.x;
    camToWorld.e[2][1] = forward.y;
    camToWorld.e[2][2] = forward.z;
    camToWorld.e[3][0] = from.x;
    camToWorld.e[3][1] = from.y;
    camToWorld.e[3][2] = from.z;
    return (camToWorld);
}

t_camera	*new_camera(t_vector3 position, t_vector3 direction, float fov)
{
	t_camera	*camera;

	if (!(camera = malloc(sizeof(t_camera))))
		return (NULL);
	camera->position = position;
	camera->direction = vec3_normalize(direction);
	camera->fov = fov;
	camera->c2w = look_at(vec3_new(0, 0, 0), camera->direction);
	camera->render = NULL;
	return (camera);
}