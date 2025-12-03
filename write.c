/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frogus <frogus@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 15:38:45 by frogus            #+#    #+#             */
/*   Updated: 2025/12/03 11:51:48 by frogus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	write_states_debug(t_philo_states states, t_philo *philo, long elapsed)
{
	if (TAKE_FIRST_FORK == states && !simulation_done(philo->table))
		printf("%-6ld %d has taken 1 fork   fork nbr : %d\n", elapsed, philo->id,
			philo->first_fork->fork_id);
	if (TAKE_SECOND_FORK == states && !simulation_done(philo->table))
		printf("%-6ld %d has taken 2 fork   fork nbr : %d\n", elapsed, philo->id,
			philo->second_fork->fork_id);
	if (EATING == states && !simulation_done(philo->table))
		printf("%-6ld %d is eating          meal nbr : %ld\n", elapsed, philo->id,
			philo->meals_counter);
	if (SLEEPING == states && !simulation_done(philo->table))
		printf("%-6ld %d is sleeping\n", elapsed, philo->id);
	if (THINKING == states && !simulation_done(philo->table))
		printf("%-6ld %d is thinking %d\n", elapsed, philo->id,
			philo->first_fork->fork_id);
	if (DEAD == states && !simulation_done(philo->table))
		printf("%-6ld %d is dead\n", elapsed, philo->id);
}

void	write_states(t_philo_states states, t_philo *philo, bool debug)
{
	long	elapsed;
	elapsed = get_time(MILLISECOND) - philo->table->start_meal;

	if (philo->full)
		return ;
	safe_mutex(&philo->table->write_mutex, LOCK);
	if (debug)
		write_states_debug(states, philo, elapsed);
	else
	{
		if ((TAKE_FIRST_FORK == states || TAKE_SECOND_FORK == states) && !simulation_done(philo->table))
			printf("%-6ld %d has taken fork\n", elapsed, philo->id);
		else if (EATING == states && !simulation_done(philo->table))
			printf("%-6ld %d is eating\n", elapsed, philo->id);
		else if (SLEEPING == states && !simulation_done(philo->table))
			printf("%-6ld %d is sleeping\n", elapsed, philo->id);
		else if (THINKING == states && !simulation_done(philo->table))
			printf("%-6ld %d is thinking\n", elapsed, philo->id);
		else if (DEAD == states && !simulation_done(philo->table))
			printf("%-6ld %d is dead\n", elapsed, philo->id);
	}
	safe_mutex(&philo->table->write_mutex, UNLOCK);
}
