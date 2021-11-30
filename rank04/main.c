/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 19:28:01 by lucocozz          #+#    #+#             */
/*   Updated: 2021/11/30 20:26:26 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "microshell.h"

int	main(int argc, char **argv, char **env)
{
	t_node	*ast;

	ast = build_ast(argc - 1, &argv[1]);
	return (0);
}
