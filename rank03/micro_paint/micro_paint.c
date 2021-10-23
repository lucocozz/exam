/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   micro_paint.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/19 14:17:38 by lucocozz          #+#    #+#             */
/*   Updated: 2021/10/23 13:38:04 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "micro_paint.h"

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

static int	init_square(t_square *square, FILE *file)
{
	char	c;
	int		ret;

	if ((ret = fscanf(file, "%c %f %f %f %f %c\n", &c, &square->x, &square->y, &square->width, &square->height, &square->filler)) != 6)
		return (ret);
	if (c == 'r')
		square->is_full = 0;
	else if (c == 'R')
		square->is_full = 1;
	else
		return (-2);
	if (square->width >= 0 || square->height >= 0)
		return (-2);
	return (ret);
}

static int	in_square(t_square square, float x, float y)
{
	if ((x >= square.x && x <= square.x + square.width) && (y >= square.y && y <= square.y + square.height))
	{
		if ((x >= square.x + 1 && x <= square.x + square.width - 1) && (y >= square.y + 1 && y <= square.y + square.height - 1))
			return (2);
		return (1);
	}
	return (0);
}

static void	draw_pixel(t_plane *plane, t_square square, int x, int y)
{
	int		is_in;

	is_in = in_square(square, (float)x, (float)y);
	if (is_in == 1 || (is_in == 2 && square.is_full == 1))
		plane->matrice[y * plane->width + x] = square.filler;
}

static void	draw_shape(t_plane *plane, t_square square)
{
	int	x;
	int	y;

	y = 0;
	while (y < plane->height)
	{
		x = 0;
		while (x < plane->width)
		{
			draw_pixel(plane, square, x, y);
			x++;
		}
		y++;
	}
}

static int	paint(t_plane *plane, FILE *file)
{
	int			ret;
	t_square	square;

	ret = init_square(&square, file);
	while (ret == 6)
	{
		draw_shape(plane, square);
		ret = init_square(&square, file);
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
		if ((file = fopen(argv[1], "r")) == NULL)
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
