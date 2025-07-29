#include <deadbeef/deadbeef.h>

static DB_functions_t *deadbeef;

static int fileadded_listener_id = 0; //A valid id is positive as seen in deadbeef/playlist.c/listen_file_added.

static int on_file_added(ddb_fileadd_data_t *data,void *user_data){
	if(data->visibility == 0){
		time_t timer = time(NULL);
		struct tm* tm_info = localtime(&timer);
		char buffer[32]; //YYYY-MM-DD HH:MM:SS
		strftime(buffer,32,"%Y-%m-%d %H:%M:%S",tm_info);

		deadbeef->pl_lock();
		deadbeef->pl_replace_meta(data->track,":playlist_added",buffer);
		deadbeef->pl_unlock();
	}
	return 0;
}

static int on_start(void){
	fileadded_listener_id = deadbeef->listen_file_added(on_file_added,NULL);
	return 0;
}

static int on_stop(void){
	if(fileadded_listener_id) deadbeef->unlisten_file_added(fileadded_listener_id);
	return 0;
}

//TODO: While on_file_added is not emitted when copying (CTRL+C, CTRL+V) a track into a playlist, this on_message detects it but only emits (DB_EV_PLAYLISTCHANGED,0,DDB_PLAYLIST_CHANGE_CONTENT,0), which is not really helpful.
/*
static int on_message(uint32_t id,uintptr_t ctx,uint32_t p1,uint32_t p2){
	switch(id){
		case DB_EV_PLAYLISTCHANGED:
			printf("Event: %i %lu %i %i\n",id,ctx,p1,p2);
			break;
	}
	return 0;
}
*/

static DB_misc_t plugin = {
	.plugin.api_vmajor = DB_API_VERSION_MAJOR,
	.plugin.api_vminor = DB_API_VERSION_MINOR,
	.plugin.version_major = 1,
	.plugin.version_minor = 0,
	.plugin.type = DB_PLUGIN_MISC,
	.plugin.name = "Playlist Item Metadata",
	.plugin.descr = "Stores metadata about addition date of tracks in playlists.",
	.plugin.copyright =
		"For private use with no guarantees.\n"
		"\n"
		"Author: EDT4",
	.plugin.website = "",
	.plugin.start = on_start,
	.plugin.stop = on_stop,
	//.plugin.message = on_message,
};

DB_plugin_t *playlist_item_meta_load(DB_functions_t *api){
	deadbeef = api;
	return DB_PLUGIN(&plugin);
}
