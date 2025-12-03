/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   leaks_protections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frogus <frogus@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 11:40:09 by frogus            #+#    #+#             */
/*   Updated: 2025/12/03 11:02:30 by frogus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*safe_malloc(size_t bytes)
{
	void	*ret;
	
	ret = malloc(bytes);
	if (NULL == ret)
		print_error("Malloc failed");
	return (ret);
}

static void	handle_mutex_error(int status, t_opcode opcode)
{
	if (0 == status)
		return;
	if (EINVAL == status && LOCK == opcode || UNLOCK == opcode)
		print_error("The value specified by mutex is invalid.");
	else if (EINVAL == status && INIT == opcode)
		print_error("The value specified by attr is invalid.");
	else if (EDEADLK == status)
		print_error("A  deadlock would occur if the thread blocked"
			"waiting for mutex.");
	else if (EPERM == status)
		print_error(" The current thread does not hold a lock on mutex.");
	else if (ENOMEM == status)
		print_error("The process cannot allocate enough memory"
			"to create another mutex.");
	else if (EBUSY == status)
		print_error("Mutex	is locked by another thread.");
}

void	safe_mutex(t_mtx *mutex, t_opcode opcode)
{
	if (LOCK == opcode)
		handle_mutex_error(pthread_mutex_lock(mutex), opcode);
	else if (UNLOCK == opcode)
		handle_mutex_error(pthread_mutex_unlock(mutex), opcode);
	else if (INIT == opcode)
		handle_mutex_error(pthread_mutex_init(mutex, NULL), opcode);
	else if (DESTROY == opcode)
		handle_mutex_error(pthread_mutex_destroy(mutex), opcode);
	else
		print_error("opcode error for safe mutex");
}

static void	handle_thread_error(int status, t_opcode opcode)
{
	if (0 == status)
		return ;
	else if (EAGAIN == status)
		print_error("no ressources to create another thread");
	else if (EPERM == status)
		print_error("The caller does not have permission");
	else if (EINVAL == status && CREATE == opcode)
		print_error("A value specified by attr is invalid.");
	else if (EINVAL == status && (JOIN == opcode || DETACH == opcode))
		print_error("A value specified by thread is not joinable");
	else if (ESRCH == status)
		print_error(" No thread could be found corresponding to that"
			"specified by the given thread ID, thread.");
	else if (EDEADLK == status)
		print_error("A deadlock was detected or the value of thread"
			"specifies the calling thread.");
}

void	safe_thread(pthread_t *thread, void *(*foo)(void *),
		void *data, t_opcode opcode)
{
	if (CREATE == opcode)
		handle_thread_error(pthread_create(thread, NULL, foo, data), opcode);
	else if (JOIN == opcode)
		handle_thread_error(pthread_join(*thread, NULL), opcode);
	else if (DETACH == opcode)
		handle_thread_error(pthread_detach(*thread), opcode);
	else
		print_error("wrong opcode for thread handle"
			"use create join or detach");
}