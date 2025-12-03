/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frogus <frogus@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 11:03:18 by frogus            #+#    #+#             */
/*   Updated: 2025/12/03 13:51:16 by frogus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_table	table;

	if (5 == ac || 6 == ac)
	{
		parse_input(&table, av);
		data_init(&table);
		dinner_start(&table);
		clean_all(&table);
	}
	else
	{
		print_error("wrong input:\n"
			"correct input is ./a.out 5 800 200 200 [5]");
	}
}

