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

#include <glib.h>
#include <pHash.h>

#include "phash.h"

G_DEFINE_QUARK (id-phash-error-quark, id_phash_error)

guint64
id_phash_for_file (const gchar *image_path, GError **error)
{
    ulong64 hash;

    if (ph_dct_imagehash (image_path, hash) == -1) {
        g_set_error (error, ID_PHASH_ERROR, ID_PHASH_ERROR_FAILED, "Failed to compute phash for %s\n", image_path);
        return 0;
    }

    return (guint64) hash;
}

gint
id_phash_hamming_distance (const guint64 first_hash, const guint64 second_hash)
{
    return ph_hamming_distance (first_hash, second_hash);
}
