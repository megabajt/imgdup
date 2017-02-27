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

#include "engine.h"
#include "result.h"

static GOptionEntry entries[] =
{
    { NULL }
};

int
main(int argc, char* argv[])
{
    g_autoptr (GOptionContext) context = NULL;
    GError *error = NULL;

    context = g_option_context_new ("FILES");

    if (!g_option_context_parse (context, &argc, &argv, &error)) {
        g_printerr ("%s\n", error->message);
        return 1;
    }

    if (argc < 3) {
        g_autofree gchar *usage = g_option_context_get_help (context, TRUE, NULL);
        g_print ("%s\n", usage);
        return 1;
    }

    g_autoptr (IdEngine) engine = id_engine_new ();

    GList *result = id_engine_compare_files (engine, argv + 1);

    for (GList *l = result; l != NULL; l = l->next) {
        g_print ("%s vs %s: Score: %.2f%%\n", id_result_get_first (l->data),
                                              id_result_get_second (l->data),
                                              id_result_get_score (l->data));
    }

    return 0;
}

