# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: crondeau <crondeau@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/10/27 16:19:19 by esivre            #+#    #+#              #
#    Updated: 2022/03/09 14:15:11 by crondeau         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	minishell

SRCS	=	minishell.c built_in_echo.c built_in_env.c check_pipe.c expand.c check_token.c \
			free.c lexer.c llist_create.c llist_env.c print.c quotes.c split_cmd.c token.c utils.c \
			built_in_cd.c exit.c
OBJS	=	$(addprefix $(OBJDIR),$(SRCS:.c=.o))
DEPS	=	$(addprefix $(DEPDIR),$(SRCS:.c=.d))
LIBFT	=   libft/libft.a
FLAGS	=	-Wall -Wextra -Werror -g
SRCDIR	=	./srcs/
OBJDIR	=	./objs/
DEPDIR	=	./objs/
INCDIR	=	./includes/

all:		$(LIBFT) $(NAME)

$(OBJDIR)%.o:	$(SRCDIR)%.c
				gcc $(FLAGS) -MMD -MP -c -I libft -I $(INCDIR) $< -o $@

$(LIBFT):		
			make bonus -C libft

$(OBJDIR):
			mkdir -p $(OBJDIR)

$(NAME):	$(OBJDIR) $(OBJS)
		gcc $(FLAGS) $(OBJS) -o $(NAME) $(LIBFT) -lreadline

clean:
			/bin/rm -rf objs
			make -C ./libft clean

fclean:		clean
			/bin/rm -f $(NAME)
			make -C ./libft/ fclean

re:			fclean all

.PHONY:		all clean fclean re

-include $(DEPS)