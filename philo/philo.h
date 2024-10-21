/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmraizik <hmraizik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 18:20:36 by hmraizik          #+#    #+#             */
/*   Updated: 2024/09/08 18:11:38 by hmraizik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <stdbool.h>
# include <sys/time.h>

typedef pthread_mutex_t	t_mutex;
typedef struct s_data	t_data;
typedef struct s_philo	t_philo;

// utils
int		ft_atoi(char *s);
void	ft_putstr_fd(char *s, int fd);
long	current_time(void);
void	print_status(t_data *data, int id, const char *status);
void	ft_usleep(long time, t_data *data);
void	ft_thinking(t_data *data, long philo_id);
void	ft_eating(t_data *data, int i);
void	ft_sleeping(t_data *data, int i);
int		init_philos(t_data *data);
int		init_data(t_data *data);
void	*philos_routine(void *void_data);
void	*routine_monitor(void *void_data);
int		parsing(char **av, t_data *philos);
int		check(t_data *data);
int		check_time_dead(t_data *data, int i);
int		check_meals_full_dead(t_data *data, int count);
void	update_infos_of_philo(t_data *data, t_philo *philo);
void	ft_free(void *ptr, void *ptr2);

typedef struct s_philo
{
	long		id;
	long		meals_counter;
	long		last_meal_time;
	pthread_t	thread_id;
	t_mutex		*first_fork;
	t_mutex		*second_fork;
	t_data		*data;
}		t_philo;

typedef struct s_data
{
	long			number_of_philos;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			must_eat;
	bool			end_simulation;
	long			start_time;
	t_mutex			*forks;
	t_philo			*philos;
	t_mutex			print_lock;
	t_mutex			simulation_lock;
	pthread_t		monitor_thread;
}			t_data;

#endif //PHILO_H