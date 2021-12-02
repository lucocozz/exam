/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 18:43:27 by lucocozz          #+#    #+#             */
/*   Updated: 2021/12/02 15:20:12 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "microshell.h"

t_command	*create_command(int argc)
{
	t_command	*command;

	command = malloc(sizeof(t_command) * 1);
	if (command == NULL)
		return (NULL);
	command->fd_in = STDIN_FILENO;
	command->fd_out = STDOUT_FILENO;
	command->next = NULL;
	command->argv = malloc(sizeof(char *) * (argc + 1));
	if (command->argv == NULL)
	{
		free(command);
		return (NULL);
	}
	return (command);
}

t_command	*get_command(int argc, char **argv, int *i)
{
	int	j;
	t_command	*command;

	j = 0;
	while (*i + j < argc && argv[*i + j][0] != '|' && argv[*i + j][0] != ';')
		j++;
	command = create_command(j);
	if (command == NULL)
		return (NULL);
	j = 0;
	while (*i + j < argc && argv[*i + j][0] != '|' && argv[*i + j][0] != ';')
	{
		command->argv[j] = argv[*i + j];
		j++;	
	}
	command->argv[j] = NULL;
	*i += j;
	return (command);
}

void	push_back(t_command	**pipeline, t_command *command)
{
	t_command	*tmp;

	tmp = *pipeline;
	if (tmp == NULL)
		*pipeline = command;
	else
	{
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = command;
	}
}

t_command	*parse_input(int argc, char **argv, int *i)
{
	t_command	*pipeline = NULL;
	t_command	*command = NULL;

	while (*i < argc && argv[*i][0] != ';')
	{
		command = get_command(argc, argv, i);
		if (command == NULL)
		{
			free_pipeline(pipeline);
			exit_error(EXIT_FAILURE, "error: fatal\n");
		}
		push_back(&pipeline, command);
	}
	return (pipeline);
}
