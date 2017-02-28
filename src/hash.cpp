/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2017 Marcin Banasiak <marcin.banasiak@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include <gdk-pixbuf/gdk-pixbuf.h>
#include <glib.h>
#include <pHash.h>

#include "hash.h"

G_DEFINE_QUARK (id-hash-error-quark, id_hash_error)

#define GRAYSCALE(r, g, b) ((guchar) ((r) * 0.30 + (g) * 0.59 + (b) * 0.11))

guint64
id_phash_for_file (const gchar *image_path, GError **error)
{
    ulong64 hash;

    if (ph_dct_imagehash (image_path, hash) == -1) {
        g_set_error (error, ID_HASH_ERROR, ID_HASH_ERROR_FAILED, "Failed to compute phash for %s\n", image_path);
        return 0;
    }

    return (guint64) hash;
}

guint64
id_dhash_for_file (const gchar *image_path, GError **error)
{
    GdkPixbuf *image = NULL;
    guchar *pixels = NULL;
    gint rowstride;
    gint pixstride;
    gint n_rows;
    gint n_cols;
    gint bit = 0;
    guint64 hash = 0;

    image = gdk_pixbuf_new_from_file_at_scale (image_path, 9, 8, FALSE, error);
    pixels = gdk_pixbuf_get_pixels (image);

    rowstride = gdk_pixbuf_get_rowstride (image);
    pixstride = gdk_pixbuf_get_has_alpha (image) ? 4 : 3;

    n_rows = gdk_pixbuf_get_height (image);
    n_cols = gdk_pixbuf_get_width (image);

    for (gint row = 0; row < n_rows; row++) {
        guchar *pixel = pixels + row * rowstride;

        guchar prev = GRAYSCALE (pixel[0], pixel[1], pixel[2]);

        for (gint col = 1; col < n_cols; col++) {
            pixel += col * pixstride;

            guchar next = GRAYSCALE (pixel[0], pixel[1], pixel[2]);

            if (prev > next) {
                hash |= (guint64) 1 << (63 - bit);
            }

            prev = next;
            bit++;
        }
    }
    
    g_object_unref (image);

    return hash;
}

gint
id_hash_hamming_distance (const guint64 first_hash, const guint64 second_hash)
{
    return __builtin_popcountll (first_hash ^ second_hash);
}
