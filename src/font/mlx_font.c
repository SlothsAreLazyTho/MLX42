/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mlx_font.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: W2Wizard <main@w2wizard.dev>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/22 12:01:37 by W2Wizard      #+#    #+#                 */
/*   Updated: 2024/02/28 13:44:54 by cbijman       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "font.h"

#include "MLX42/MLX42.h"
#include "MLX42/MLX42_Int.h"

//= Private =//

/**
 * Does the actual copying of pixels form the atlas buffer to the
 * image buffer.
 * 
 * Skips any non-printable characters.
 * 
 * @param image The image to draw on.
 * @param texture The font_atlas.
 * @param texoffset The character texture X offset.
 * @param imgoffset The image X offset.
 */
static void mlx_draw_char(mlx_image_t* image, int32_t texoffset, int32_t imgoffset,	t_fontatlas *atlas)
{
	if (texoffset < 0)
		return;

	char* pixelx;
	uint8_t* pixeli;
	for (uint32_t y = 0; y < atlas->font_height; y++)
	{
		pixelx = &atlas->pixels[(y * atlas->width + texoffset) * atlas->bytes_per_pixel];
		pixeli = image->pixels + ((y * image->width + imgoffset) * atlas->bytes_per_pixel);
		memcpy(pixeli, pixelx, atlas->font_width * atlas->bytes_per_pixel);
	}
}

//= Public =//
const t_fontatlas* mlx_get_font(mlx_t *mlx)
{
	MLX_NONNULL(mlx);
	
    return ((const t_fontatlas*)&mlx->font);
}

void	mlx_set_font(mlx_t *mlx, t_fontatlas *atlas)
{
	MLX_NONNULL(mlx);
	MLX_NONNULL(atlas);
	
	mlx->font = atlas;
	mlx->font_changed = 1;
}

int32_t mlx_get_texoffset(mlx_t *mlx, char c)
{
    const bool _isprint = isprint(c);

	if (mlx->font_changed == 0)
    	return (-1 * !_isprint + ((mlx->font->font_width + 2) * (c - 32)) * _isprint); //Standard fonts need +2 to hide it's dividers
    return (-1 * !_isprint + ((mlx->font->font_width) * (c - 32)) * _isprint);
}

mlx_image_t* mlx_put_string(mlx_t* mlx, const char* str, int32_t x, int32_t y)
{
	MLX_NONNULL(mlx);
	MLX_NONNULL(str);

	mlx_image_t* strimage;
	const size_t len = strlen(str);
	if (len > MLX_MAX_STRING)
		return ((void*)mlx_error(MLX_STRTOOBIG));	
	if (!(strimage = mlx_new_image(mlx, len * mlx->font->font_width,  mlx->font->font_height)))
		return (NULL);

	// Draw the text itself
	for (size_t i = 0; i < len; i++)
		mlx_draw_char(strimage, mlx_get_texoffset(mlx, str[i]), i * mlx->font->font_width, mlx->font);

	if (mlx_image_to_window(mlx, strimage, x, y) == -1)
		return (mlx_delete_image(mlx, strimage), NULL);
	return (strimage);
}
