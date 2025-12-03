/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frogus <frogus@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 10:34:54 by frogus            #+#    #+#             */
/*   Updated: 2025/12/03 14:24:29 by frogus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/time.h>
#include <limits.h>
#include <errno.h>

#define DEBUG_MODE 1

typedef enum	e_opcode
{
	LOCK,
	UNLOCK,
	INIT,
	DESTROY,
	CREATE,
	JOIN,
	DETACH,
}				t_opcode;

typedef enum	e_time_code
{
	SECOND,
	MILLISECOND,
	MICROSECOND,
}				t_time_code;

typedef enum	e_states
{
	EATING,
	SLEEPING,
	THINKING,
	TAKE_FIRST_FORK,
	TAKE_SECOND_FORK,
	DEAD,
}				t_philo_states;

typedef pthread_mutex_t	t_mtx;

typedef struct s_table	t_table;

typedef struct	fork
{
	t_mtx	fork;
	int		fork_id;
} 			t_fork;

typedef struct	s_philo
{
	int			id;
	long		meals_counter;
	bool		full;
	long		last_meal_time;
	t_fork		*first_fork;
	t_fork		*second_fork;
	pthread_t	thread_id;
	t_mtx		philo_mutex;
	t_table		*table;
}				t_philo; 

struct	s_table
{
	long		philo_nbr;
	long		time_to_die;
	long		time_to_eat;
	long		time_to_sleep;
	long		limit_of_meal;
	long		start_meal;
	long		threads_running_id;
	bool		end_of_meal;
	bool		all_threads_ready;
	pthread_t	monitor;
	t_mtx		table_mutex;
	t_mtx		write_mutex;
	t_fork		*forks;
	t_philo		*philos;
};


void	print_error(char *error);
void	parse_input(t_table *table, char **av);
void	*safe_malloc(size_t bytes);
void	safe_mutex(t_mtx *mutex, t_opcode opcode);
void	safe_thread(pthread_t *thread, void *(*foo)(void *),
				void *data, t_opcode opcode);
void	data_init(t_table *table);
void	set_bool(t_mtx *mutex, bool *dest, bool value);
bool	get_bool(t_mtx *mutex, bool *value);
void	set_long(t_mtx *mutex, long *dest, long value);
long	get_long(t_mtx *mutex, long *value);
bool	simulation_done(t_table *table);
void	wait_all_thread(t_table *table);
long	get_time(t_time_code time_code);
void	my_usleep(long usec, t_table *table);
void	write_states_debug(t_philo_states states, t_philo *philo, long elapsed);
void	write_states(t_philo_states states, t_philo *philo, bool debug);
void	dinner_start(t_table *table);
bool	all_threads_running(t_mtx *mutex, long *threads, long philo_nbr);
void	increase_long(t_mtx *mutex, long *value);
void	*monitor_dinner(void	*data);
void	clean_all(t_table *table);
void	philo_thinking(t_philo *philo, bool pre_sim);
void	must_think(t_philo *philo);


#endif