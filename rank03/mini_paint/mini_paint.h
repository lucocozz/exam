/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_paint.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 19:00:47 by lucocozz          #+#    #+#             */
/*   Updated: 2021/10/22 20:16:00 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_PAINT_H
# define MINI_PAINT_H

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

typedef struct s_circle
{
	float		x;
	float		y;
	float		radius;
	char		filler;
	int			is_full;
}				t_circle;

void	ft_putstr(char *str);
int		ft_strlen(char *str);

#endif