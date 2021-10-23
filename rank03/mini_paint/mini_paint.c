/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_paint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 14:17:38 by lucocozz          #+#    #+#             */
/*   Updated: 2021/10/23 13:38:43 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_paint.h"

static int	exit_error(FILE *file, t_plane *plane)
{
	if (file != NULL)
		fclose(file);
	if (plane != NULL)
		free(plane->matrice);
	ft_putstr("Error: Operation file corrupted\n");
	return (1);
}

static void	print_plane(t_plane *plane)
{
	int	y;

	y = 0;
	while (y < plane->height)
	{
		write(STDOUT_FILENO, &plane->matrice[y * plane->width], plane->width);
		write(STDOUT_FILENO, "\n", 1);
		y++;
	}
}

static int	init_plane(t_plane *plane, FILE *file)
{
	if (fscanf(file, "%d %d %c\n", &plane->width, &plane->height, &plane->filler) != 3)
		return (-1);
	if (!(plane->width > 0 && plane->width <= 300) || !(plane->height > 0 && plane->height <= 300))
		return (-1);
	plane->matrice = calloc(plane->width * plane->height, sizeof(char));
	if (plane->matrice == NULL)
		return (-1);
	memset(plane->matrice, plane->filler, plane->height * plane->width);
	return (0);
}

static int	init_circle(t_circle *circle, FILE *file)
{
	char	c;
	int		ret;

	if ((ret = fscanf(file, "%c %f %f %f %c\n", &c, &circle->x, &circle->y, &circle->radius, &circle->filler)) != 5)
		return (ret);
	if (c == 'c')
		circle->is_full = 0;
	else if (c == 'C')
		circle->is_full = 1;
	else
		return (-2);
	if (circle->radius >= 0)
		return (-2);
	return (ret);
}

static float	get_distance(float xa, float ya, float xb, float yb)
{
	float	dist_x;
	float	dist_y;

	dist_x = powf(xa - xb, 2.0);
	dist_y = powf(ya - yb, 2.0);
	return (sqrtf(dist_x + dist_y));
}

static int	in_circle(t_circle circle, float x, float y)
{
	float	dist;

	dist = get_distance(circle.x, circle.y, x, y);
	if (dist <= circle.radius)
	{
		if (dist - circle.radius <= -1.0)
			return (2);
		return (1);
	}
	return (0);
}

static void	draw_pixel(t_plane *plane, t_circle circle, int x, int y)
{
	int		is_in;

	is_in = in_circle(circle, (float)x, (float)y);
	if (is_in == 1 || (is_in == 2 && circle.is_full == 1))
		plane->matrice[y * plane->width + x] = circle.filler;
}

static void	draw_shape(t_plane *plane, t_circle circle)
{
	int	x;
	int	y;

	y = 0;
	while (y < plane->height)
	{
		x = 0;
		while (x < plane->width)
		{
			draw_pixel(plane, circle, x, y);
			x++;
		}
		y++;
	}
}

static int	paint(t_plane *plane, FILE *file)
{
	int			ret;
	t_circle	circle;

	ret = init_circle(&circle, file);
	while (ret == 5)
	{
		draw_shape(plane, circle);
		ret = init_circle(&circle, file);
	}
	if (ret != EOF)
		return (-1);
	print_plane(plane);
	return (0);
}

int	main(int argc, char **argv)
{
	FILE	*file;
	t_plane	plane;

	if (argc == 2)
	{
		file = fopen(argv[1], "r");
		if (file == NULL)
			return (exit_error(NULL, NULL));
		if (init_plane(&plane, file) == -1)
			return (exit_error(file, NULL));
		if (paint(&plane, file) == -1)
			return (exit_error(file, &plane));
		fclose(file);
		free(plane.matrice);
		return (0);
	}
	ft_putstr("Error: argument\n");
	return (1);
}
