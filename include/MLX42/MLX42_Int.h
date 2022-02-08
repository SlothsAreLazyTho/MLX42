/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   MLX42_Int.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: W2Wizard <w2.wizzard@gmail.com>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/12/27 23:55:34 by W2Wizard      #+#    #+#                 */
/*   Updated: 2022/02/08 01:04:49 by w2wizard      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MLX42_INT_H
# define MLX42_INT_H
# include "MLX42/MLX42.h"
# include "glad/glad.h"
# include "KHR/khrplatform.h"
# if defined(__APPLE__)
#  define GL_SILENCE_DEPRECATION
#  define IS_APPLE 1
# else
#  define IS_APPLE 0
# endif
# include <GLFW/glfw3.h>
# include <stdlib.h>
# include <memory.h>
# include <stdio.h>
# include <limits.h>
# include <ctype.h>
# include <string.h>
# ifndef VERTEX_PATH
#  define VERTEX_PATH "shaders/default.vert"
# endif
# ifndef FRAGMENT_PATH
#  define FRAGMENT_PATH "shaders/default.frag"
# endif
# ifndef MLX_SWAP_INTERVAL
#  define MLX_SWAP_INTERVAL 1
# endif
# define MLX_INFO "MLX42: Info"
# define MLX_ERROR "MLX42: Error:"
# define MLX_WARNING "MLX42: Warning:"
# define MLX_INVALID_FILE_EXT "Invalid file extension!"
# define MLX_INVALID_FILE "Failed to read file!"
# define MLX_INVALID_ARG "Invalid argument provided!"
# define MLX_NULL_ARG "NULL argument exception!"
# define MLX_SHADER_FAILURE "Shader failure!"
# define MLX_RENDER_FAILURE "Failed to initialize Renderer!"
# define MLX_MEMORY_FAIL "Failed to allocate enough memory!"
# define MLX_XPM_FAILURE "Failed to read XPM42 file!"
# define GLFW_INIT_FAILURE "Failed to initialize GLFW!"
# define GLFW_WIN_FAILURE "Failed to create GLFW Window!"
# define GLFW_GLAD_FAILURE "Failed to initialize GLAD!"

//= Types =//

// A single vertex, identical to the layout in the shader.
typedef struct s_vert
{
	float	x;
	float	y;
	float	z;
	float	u;
	float	v;
}	t_vert;

// Hook layout used to add generic loop hooks.
typedef struct s_mlx_hook
{
	void	*param;
	void	(*func)(void*);
}	t_mlx_hook;

// Layout for linked list.
typedef struct s_mlx_list
{
	void				*content;
	struct s_mlx_list	*next;
	struct s_mlx_list	*prev;
}	t_mlx_list;

// To maintain the drawing order we add every instance to a queue.
typedef struct s_draw_queue
{
	t_mlx_image			*image;
	t_mlx_instance		*instance;
}	t_draw_queue;

// MLX Instance handle context used for OpenGL stuff.
typedef struct s_mlx_ctx
{
	GLuint				vao;
	GLuint				vbo;
	GLuint				shaderprogram;
	t_mlx_list			*hooks;
	t_mlx_list			*images;
	t_mlx_list			*render_queue;
	t_mlx_scrollfunc	scroll_hook;
	t_mlx_keyfunc		key_hook;
}	t_mlx_ctx;

// Additional OpenGL information for images/textures.
typedef struct s_mlx_image_ctx
{
	t_vert	vertices[6];
	GLuint	texture;
}	t_mlx_image_ctx;

//= Linked List Functions =//

t_mlx_list	*mlx_lstnew(void *content);
t_mlx_list	*mlx_lstlast(t_mlx_list *lst);
int32_t		mlx_lstsize(t_mlx_list *lst);
void		mlx_lstclear(t_mlx_list **lst, void (*del)(void*));
void		mlx_lstadd_back(t_mlx_list **lst, t_mlx_list *new);
t_mlx_list	*mlx_lstremove(t_mlx_list **lst, void *value, \
bool (*comp)(void *, void*));

//= Misc functions =//

bool		mlx_equal_image(void *lstcontent, void *value);
bool		mlx_equal_inst(void *lstcontent, void *value);
void		mlx_xpm_putpixel(t_xpm *xpm, int32_t x, int32_t y, uint32_t color);

//= Error/log Handling Functions =//

bool		mlx_log(const char *log, const char *msg);
bool		mlx_freen(int32_t count, ...);

//= IO Functions =//

char		*mlx_readfile(const char *FilePath);

//= OpenGL Functions =//

bool		mlx_init_shaders(t_mlx *mlx, uint32_t *shaders);
bool		mlx_compile_shader(const char *Path, int32_t Type, uint32_t *out);
void		mlx_draw_instance(t_mlx *mlx, t_mlx_image *img, \
t_mlx_instance *instance);

// Utils Functions =//

int32_t		mlx_rgba_to_mono(int32_t color);
int32_t		mlx_atoi_base(const char *str, int32_t base);
#endif
