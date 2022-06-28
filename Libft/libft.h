#ifndef LIBFT_H
# define LIBFT_H

#include <unistd.h>
#include <stdlib.h>

typedef struct s_list
{
    void            *content;
    struct s_list   *next;
}                   t_list;

int ft_isalpha(int c);
int ft_isdigit(int c);
int ft_isalnum(int c);
int ft_isascii(int c);
int ft_isprint(int c);
int ft_toupper(int c);
int ft_tolower(int c);


size_t  ft_strlen(const char *str);
size_t  ft_strlcpy(char *dst, const char *src, size_t len);
size_t  ft_strlcat(char *dst, const char *src, size_t len);



#endif