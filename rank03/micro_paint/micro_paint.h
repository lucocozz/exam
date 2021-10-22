/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   micro_paint.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 19:00:47 by lucocozz          #+#    #+#             */
/*   Updated: 2021/10/22 20:38:07 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MICRO_PAINT_H
# define MICRO_PAINT_H

# include <unistd.h>
# include <string.h>
# include <stdlib.h>
# include <stdio.h>
# include <math.h>

typedef struct s_plane
{
	int			width;
	int			height;
	char		filler;
	char		*matrice;
}				t_plane;

typedef struct s_square
{
	float		x;
	float		y;
	float		width;
	float		height;
	char		filler;
	int			is_full;
}				t_square;

void	ft_putstr(char *str);
int		ft_strlen(char *str);

#endif