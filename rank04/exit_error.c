/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 10:22:01 by lucocozz          #+#    #+#             */
/*   Updated: 2021/12/01 10:30:08 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "microshell.h"

void exit_error(int status, char *str)
{
	if (str != NULL)
		ft_putstr_fd(STDERR_FILENO, str);
	exit(status);
}
