/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorette <tlorette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 12:03:13 by frogus            #+#    #+#             */
/*   Updated: 2025/12/04 10:27:40 by tlorette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	philo_dead(t_philo *philo)
{
	long	elapsed;
	long	time_to_die;

	if (get_bool(&philo->philo_mutex, &philo->full))
		return (false);
	elapsed = get_time(MILLISECOND) - get_long(&philo->philo_mutex,
			&philo->last_meal_time);
	time_to_die = philo->table->time_to_die / 1e3;
	if (elapsed > time_to_die)
		return (true);
	return (false);
}

void	*monitor_dinner(void *data)
{
	t_table	*table;
	int		i;

	table = (t_table *)data;
	while (!all_threads_running(&table->table_mutex, &table->threads_running_id,
			table->philo_nbr))
		;
	while (!simulation_done(table))
	{
		i = -1;
		while (++i < table->philo_nbr && !simulation_done(table))
		{
			if (philo_dead(table->philos + i))
			{
				write_states(DEAD, table->philos + i, DEBUG_MODE);
				set_bool(&table->table_mutex, &table->end_of_meal, true);
			}
		}
	}
	return (NULL);
}
