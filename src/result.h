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

#ifndef __ID_RESULT_H
#define __ID_RESULT_H

#include "config.h"

#include <glib-object.h>

G_BEGIN_DECLS

#define ID_TYPE_RESULT          (id_result_get_type ())
#define ID_RESULT(o)            (G_TYPE_CHECK_INSTANCE_CAST ((o), ID_TYPE_RESULT, IdResult))
#define ID_RESULT_CLASS(k)      (G_TYPE_CHECK_CLASS_CAST((k), ID_TYPE_RESULT, IdResultClass))
#define ID_IS_RESULT(o)         (G_TYPE_CHECK_INSTANCE_TYPE ((o), ID_TYPE_RESULT))
#define ID_IS_RESULT_CLASS(k)   (G_TYPE_CHECK_CLASS_TYPE ((k), ID_TYPE_RESULT))
#define ID_RESULT_GET_CLASS(o)  (G_TYPE_INSTANCE_GET_CLASS ((o), ID_TYPE_RESULT, IdResultClass))

typedef struct IdResultPrivate IdResultPrivate;

typedef struct
{
    GObject             parent;
    IdResultPrivate    *priv;
} IdResult;

typedef struct
{
    GObjectClass        parent_class;
} IdResultClass;

#ifdef G_DEFINE_AUTOPTR_CLEANUP_FUNC
G_DEFINE_AUTOPTR_CLEANUP_FUNC(IdResult, g_object_unref)
#endif

GType           id_result_get_type      (void);
IdResult       *id_result_new           (const gchar *first,
                                         const gchar *second,
                                         gfloat       score);

const gchar    *id_result_get_first     (IdResult *result);
const gchar    *id_result_get_second    (IdResult *result);
gfloat          id_result_get_score     (IdResult *result);

G_END_DECLS

#endif /* __ID_RESULT_H */

