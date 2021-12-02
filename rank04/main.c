/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 19:28:01 by lucocozz          #+#    #+#             */
/*   Updated: 2021/12/02 15:19:55 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "microshell.h"

void	free_pipeline(t_command *pipeline)
{
	t_command	*to_free;

	while (pipeline != NULL)
	{
		to_free = pipeline;
		pipeline = pipeline->next;
		free(to_free->argv);
		free(to_free);
	}
}

int	main(int argc, char **argv, char **env)
{
	int			i = 1;
	t_command	*pipeline;

	while (i < argc)
	{
		pipeline = parse_input(argc, argv, &i);
		exec_pipeline(pipeline, env);
		free_pipeline(pipeline);
		i++;
	}
	return (0);
}
