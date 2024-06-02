#ifndef PHILO_H
#define PHILO_H

# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <string.h>

# define RED  "\x1B[31m"
// utils
void	ft_exit(const char *s);
int		ft_atoi(char *s);

typedef struct s_philos
{
	long		number_of_philos;
	long		time_to_die;
	long		time_to_eat;
	long		time_to_sleep;
	long		how_much_time_must_eat;
}			t_philo;
#endif //PHILO_H