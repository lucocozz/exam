/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 20:26:00 by lucocozz          #+#    #+#             */
/*   Updated: 2021/12/01 10:36:06 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "microshell.h"

void	free_ast(t_node	*ast)
{
	if (ast == NULL)
		return ;
	free_ast(ast->left);
	free_ast(ast->right);
	if (ast->command != NULL)
		free(ast->command);
	free(ast);
}

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
	t_node	*tmp;

	tmp = *ast;
	node = create_node(Pipe);
	if (node == NULL)
	{
		free_ast(*ast);
		exit_error(EXIT_FAILURE, "error: fatal\n");
	}
	node->left = tmp;
	*ast = node;
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
			node_command(&ast, &i);
		i++;
	}
	return (ast);
}
