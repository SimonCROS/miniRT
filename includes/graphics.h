#ifndef GRAPHICS_H
# define GRAPHICS_H

typedef struct s_vector2i		t_vector2i;
typedef struct s_vector2f		t_vector2f;
typedef struct s_vector3f		t_vector3f;
typedef struct s_quadric		t_quadric;
typedef struct s_matrix44		t_matrix44;
typedef struct s_bounding_box	t_bounding_box;
typedef struct s_line			t_line;

typedef t_vector2i				t_vec2i;
typedef t_vector2f				t_vec2f;
typedef t_vector3f				t_vec3f;

/*** Matrix implementation ****************************************************/

struct s_matrix44
{
	float	e[4][4];
};

t_matrix44		mat44_null(void);
t_matrix44		mat44_inverse(t_matrix44 m);
t_vector3f		mat44_mul_vec(t_matrix44 matrix, t_vector3f in);

// To hide
float			determin(float matrix[4][4], int k);

/*** Quadrics implementation **************************************************/

struct s_quadric
{
	float	a;
	float	b;
	float	c;
	float	d;
	float	e;
	float	f;
	float	g;
	float	h;
	float	i;
	float	j;
};

float			discriminant(t_vec3f abc);
float			inter_quad_line_sol(t_vec3f abc, float d);
t_vec3f			inter_quad_line_coeff(t_quadric *q, t_vec3f o, t_vec3f d);
void			inter_quad_line_sol_double(t_vec3f abc, float d, float t[2]);

/**
 * @brief Resolve the length of the vector dir to get the firts intersection
 * between the vector and the quadric.
 * 
 * @param quad the quadric
 * @param origin origin
 * @param d direction
 * @return the length smaller and positive or 0 if not found
 */
float			resolve_quad(t_quadric *quad, t_vector3f origin, t_vector3f d);
int				resolve_quad_double(t_quadric *quad, t_vector3f o,
					t_vector3f di, float *t);
/**
 * @brief Resolve the length of the vector dir to get the firts intersection
 * between the vector and the quadric.
 * 
 * @param quad the quadric
 * @param phit point hit
 * @return the norm at this point
 */
t_vector3f		resolve_quad_norm(t_quadric *quad, t_vector3f phit);

/*** Vectors 2i implementation ************************************************/

struct s_vector2i
{
	int	x;
	int	y;
};

/*** Vectors 2f implementation ************************************************/

struct s_vector2f
{
	float	x;
	float	y;
};

/*** Vectors 3f implementation ************************************************/

struct s_vector3f
{
	float	x;
	float	y;
	float	z;
};

t_vector3f		vec3_rotate_axis(t_vector3f vec, t_vector3f a, float theta);
/**
 * @brief Deserialize a vector at format "x.x,y.y,z.z"
 * 
 * @param str the vector at string format
 * @param vector the container of the new vector
 * @return the result of the operation : 1 if no error, 0 else
 */
int				vec3_deserialize(const char *str, t_vector3f *vector);
t_vector3f		*vec3_set(t_vector3f *v, float x, float y, float z);
float			vec3_distance_squared(t_vector3f v1, t_vector3f v2);
float			vec3_distance(t_vector3f v1, t_vector3f v2);
t_vector3f		vec3_rotate_z(t_vector3f vec, float theta);
t_vector3f		vec3_rotate_y(t_vector3f vec, float theta);
t_vector3f		vec3_rotate_x(t_vector3f vec, float theta);
t_vector3f		vec3_cross(t_vector3f v1, t_vector3f v2);
float			vec3_angle(t_vector3f v1, t_vector3f v2);
t_vector3f		vec3_sub(t_vector3f v1, t_vector3f v2);
t_vector3f		vec3_mul(t_vector3f v1, t_vector3f v2);
t_vector3f		vec3_div(t_vector3f v1, t_vector3f v2);
t_vector3f		vec3_add(t_vector3f v1, t_vector3f v2);
float			vec3_dot(t_vector3f v1, t_vector3f v2);
t_vector3f		vec3_new(float x, float y, float z);
float			vec3_length_squared(t_vector3f v);
t_vector3f		vec3_muld(t_vector3f v, float a);
t_vector3f		vec3_divd(t_vector3f v, float a);
t_vector3f		vec3_normalize(t_vector3f v);
int				vec3_is_null(t_vector3f v);
t_vector3f		vec3_negate(t_vector3f v);
float			vec3_length(t_vector3f v);
t_vector3f		vec3_null(void);

/*** Bounding boxes ***********************************************************/

struct s_bounding_box
{
	t_vector2i	min;
	t_vector2i	max;
};

t_bounding_box	bounding_box_from(t_vec2i v1, t_vec2i v2);
t_bounding_box	bounding_box_from3(t_vec2i v1, t_vec2i v2, t_vec2i v3);
t_bounding_box	bounding_box_fromf(t_vec2f v1, t_vec2f v2);
t_bounding_box	bounding_box_fromf3(t_vec2f v1, t_vec2f v2, t_vec2f v3);
int				bounding_box_intersects(t_bounding_box b1, t_bounding_box b2);
t_bounding_box	bounding_box_intersection(t_bounding_box b1, t_bounding_box b2);

/*** Lines ********************************************************************/

struct s_line
{
	t_vector2i	from;
	t_vector2i	to;
};

t_line			line_fromf(t_vec2f v1, t_vec2f v2);
t_line			line_from(t_vec2i v1, t_vec2i v2);

#endif
