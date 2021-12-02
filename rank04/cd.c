/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 22:19:25 by lucocozz          #+#    #+#             */
/*   Updated: 2021/12/01 22:27:59 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "microshell.h"

void	cd(char **argv)
{
	int	i;

	i = 0;
	while (argv[i] != NULL)
		i++;
	if (i != 2)
		ft_putstr_fd(STDERR_FILENO, "error: cd: bad arguments\n");
	else
	{
		if (chdir(argv[1]) == -1)
		{
			ft_putstr_fd(STDERR_FILENO, "error: cd: cannot change directory to ");
			ft_putstr_fd(STDERR_FILENO, argv[1]);
			ft_putstr_fd(STDERR_FILENO, "\n");
		}
	}
}
