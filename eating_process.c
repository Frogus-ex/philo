/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorette <tlorette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 14:28:45 by frogus            #+#    #+#             */
/*   Updated: 2025/12/04 10:25:27 by tlorette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_thinking(t_philo *philo, bool pre_sim)
{
	long	time_to_think;
	long	time_to_eat;
	long	time_to_sleep;

	if (!pre_sim)
		write_states(THINKING, philo, DEBUG_MODE);
	if (philo->table->philo_nbr % 2 == 0)
		return ;
	time_to_eat = philo->table->time_to_eat;
	time_to_sleep = philo->table->time_to_sleep;
	time_to_think = time_to_eat * 2 - time_to_sleep;
	if (time_to_think < 0)
		time_to_think = 0;
	my_usleep(time_to_think * 0.42, philo->table);
}

static void	philo_eating(t_philo *philo)
{
	safe_mutex(&philo->first_fork->fork, LOCK);
	write_states(TAKE_FIRST_FORK, philo, DEBUG_MODE);
	safe_mutex(&philo->second_fork->fork, LOCK);
	write_states(TAKE_SECOND_FORK, philo, DEBUG_MODE);
	set_long(&philo->philo_mutex, &philo->last_meal_time,
		get_time(MILLISECOND));
	philo->meals_counter++;
	write_states(EATING, philo, DEBUG_MODE);
	my_usleep(philo->table->time_to_eat, philo->table);
	if (philo->table->limit_of_meal > 0
		&& philo->meals_counter == philo->table->limit_of_meal)
		set_bool(&philo->philo_mutex, &philo->full, true);
	safe_mutex(&philo->first_fork->fork, UNLOCK);
	safe_mutex(&philo->second_fork->fork, UNLOCK);
}

void	*lone_philo(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	wait_all_thread(philo->table);
	set_long(&philo->philo_mutex, &philo->last_meal_time,
		get_time(MILLISECOND));
	increase_long(&philo->table->table_mutex,
		&philo->table->threads_running_id);
	write_states(TAKE_FIRST_FORK, philo, DEBUG_MODE);
	while (!simulation_done(philo->table))
		usleep(200);
	return (NULL);
}

void	*dinner_simulation(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	wait_all_thread(philo->table);
	set_long(&philo->philo_mutex, &philo->last_meal_time,
		get_time(MILLISECOND));
	increase_long(&philo->table->table_mutex,
		&philo->table->threads_running_id);
	must_think(philo);
	while (!simulation_done(philo->table))
	{
		if (philo->full)
			break ;
		philo_eating(philo);
		write_states(SLEEPING, philo, DEBUG_MODE);
		my_usleep(philo->table->time_to_sleep, philo->table);
		philo_thinking(philo, false);
	}
}

void	dinner_start(t_table *table)
{
	int	i;

	i = -1;
	if (0 == table->limit_of_meal)
		return ;
	else if (1 == table->philo_nbr)
		safe_thread(&table->philos[0].thread_id, lone_philo, &table->philos[0],
			CREATE);
	else
	{
		while (++i < table->philo_nbr)
			safe_thread(&table->philos[i].thread_id, dinner_simulation,
				&table->philos[i], CREATE);
	}
	safe_thread(&table->monitor, monitor_dinner, table, CREATE);
	table->start_meal = get_time(MILLISECOND);
	set_bool(&table->table_mutex, &table->all_threads_ready, true);
	i = -1;
	while (++i < table->philo_nbr)
		safe_thread(&table->philos[i].thread_id, NULL, NULL, JOIN);
	set_bool(&table->table_mutex, &table->end_of_meal, true);
	safe_thread(&table->monitor, NULL, NULL, JOIN);
}
