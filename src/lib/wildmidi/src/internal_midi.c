/*
 internal_midi.c

 Midi Wavetable Processing library

 Copyright (C) Chris Ison  2001-2011
 Copyright (C) Bret Curtis 2013-2014

 This file is part of WildMIDI.

 WildMIDI is free software: you can redistribute and/or modify the player
 under the terms of the GNU General Public License and you can redistribute
 and/or modify the library under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation, either version 3 of
 the licenses, or(at your option) any later version.

 WildMIDI is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License and
 the GNU Lesser General Public License for more details.

 You should have received a copy of the GNU General Public License and the
 GNU Lesser General Public License along with WildMIDI.  If not,  see
 <http://www.gnu.org/licenses/>.
 */

#include <stdint.h>
#include <stdlib.h>

#include "internal_midi.h"

struct _event {
	void (*do_event)(struct _mdi *mdi, struct _event_data *data);
	uint32_t event_data;
	uint32_t samples_to_next;
	uint32_t samples_to_next_fixed;
};

struct _event_list {
	struct _event *events;
	struct _event *current_event;
	uint32_t event_count;
};

void free_event_list(struct _event_list *event_list) {
	if (event_list) {
		free(event_list->events);
		free(event_list);
	}
}

struct _event_list *add_event(struct _event_list *event_list,
		void (*do_event)(struct _mdi *mdi, struct _event_data *data),
		uint32_t event_data) {
	struct _event *tmp_events = NULL;

	if (!event_list) {
		if (!(event_list = malloc(sizeof(struct _event_list)))) {
			/* TODO: ERROR */
			return NULL;
		}
		event_list->events = NULL;
		event_list->current_event = NULL;
		event_list->event_count = 0;
	}
	if (!(tmp_events = realloc(event_list->events,
			((event_list->event_count + 1) * sizeof(struct _event))))) {

		free_event_list(event_list);
		return NULL;
	}

	event_list->events = tmp_events;
	event_list->events[event_list->event_count]->do_event = do_event;
	event_list->events[event_list->event_count]->event_data = event_data;
	event_list->event_count++;

	return event_list;
}
