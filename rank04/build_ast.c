/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 20:26:00 by lucocozz          #+#    #+#             */
/*   Updated: 2021/11/30 21:05:57 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "microshell.h"

t_node	*create_node(t_token type)
{
	t_node	*node;

	node = malloc(sizeof(t_node) * 1);
	if (node == NULL)
		return (NULL);
	node->type = type;
	node->left = NULL;
	node->right = NULL;
	node->command = NULL;
	return (node);
}

void	node_pipe(t_node **ast)
{
	t_node	*node;

	node = create_node(Pipe);
	if (node == NULL)
		
}

t_node	*build_ast(int size, char **arg)
{
	int		i = 0;
	t_node	*ast;

	while (i < size)
	{
		if (arg[i][0] == '|')
			node_pipe(&ast);
		else if (arg[i][0] == ';')
			node_semicolon(&ast);
		else
			i += node_command(&ast);
		i++;
	}
	return (ast);
}
