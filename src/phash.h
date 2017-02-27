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

#ifndef __ID_PHASH_H
#define __ID_PHASH_H

#include "config.h"

#include <glib.h>

G_BEGIN_DECLS

#define ID_PHASH_ERROR  (id_phash_error_quark ())

typedef enum
{
    ID_PHASH_ERROR_FAILED
} IdPhashError;

GQuark  id_phash_error_quark            (void);

guint64 id_phash_for_file               (const gchar *image_path,
                                         GError     **error);
gint    id_phash_hamming_distance       (const guint64 first_hash,
                                         const guint64 second_hash);

G_END_DECLS

#endif /* __ID_PHASH_H */
