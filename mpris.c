#include <inttypes.h>
#include <stdint.h>
#include <sys/stat.h>
#include <mpv/client.h>
#include <glib.h>
#include <gio/gio.h>
#include "mpris.h"
#include "mpvValues.h"
#include <stdio.h>
#define BUS_NAME "org.mpris.MediaPlayer2.musicpv"
#define OBJECT_NAME "/org/mpris/MediaPlayer2"
#define PLAYER_INTERFACE "org.mpris.MediaPlayer2.Player"
#define PROPERTIES_INTERFACE "org.freedesktop.DBus.Properties"
#define CURRENT_TRACK -1
const char* mpv_next[] = {"osd-msg", "playlist-next", "weak", NULL};

const char* mpv_prev[] = {"osd-msg", "playlist-prev", "weak", NULL};

typedef struct Interfaces
{
    GDBusInterfaceInfo *root_interface_info;
    GDBusInterfaceInfo *player_interface_info;
} Interfaces;


static const char xmlForNode[] =
    "<node>\n"
    "  <interface name=\"org.mpris.MediaPlayer2\">\n"
    "    <method name=\"Raise\">\n"
    "    </method>\n"
    "    <method name=\"Quit\">\n"
    "    </method>\n"
    "    <property name=\"CanQuit\" type=\"b\" access=\"read\"/>\n"
    "    <property name=\"Fullscreen\" type=\"b\" access=\"readwrite\"/>\n"
    "    <property name=\"CanSetFullscreen\" type=\"b\" access=\"read\"/>\n"
    "    <property name=\"CanRaise\" type=\"b\" access=\"read\"/>\n"
    "    <property name=\"HasTrackList\" type=\"b\" access=\"read\"/>\n"
    "    <property name=\"Identity\" type=\"s\" access=\"read\"/>\n"
    "    <property name=\"DesktopEntry\" type=\"s\" access=\"read\"/>\n"
    "    <property name=\"SupportedUriSchemes\" type=\"as\" access=\"read\"/>\n"
    "    <property name=\"SupportedMimeTypes\" type=\"as\" access=\"read\"/>\n"
    "  </interface>\n"
    "  <interface name=\"org.mpris.MediaPlayer2.Player\">\n"
    "    <method name=\"Next\">\n"
    "    </method>\n"
    "    <method name=\"Previous\">\n"
    "    </method>\n"
    "    <method name=\"Pause\">\n"
    "    </method>\n"
    "    <method name=\"PlayPause\">\n"
    "    </method>\n"
    "    <method name=\"Stop\">\n"
    "    </method>\n"
    "    <method name=\"Play\">\n"
    "    </method>\n"
    "    <method name=\"Seek\">\n"
    "      <arg type=\"x\" name=\"Offset\" direction=\"in\"/>\n"
    "    </method>\n"
    "    <method name=\"SetPosition\">\n"
    "      <arg type=\"o\" name=\"TrackId\" direction=\"in\"/>\n"
    "      <arg type=\"x\" name=\"Offset\" direction=\"in\"/>\n"
    "    </method>\n"
    "    <method name=\"OpenUri\">\n"
    "      <arg type=\"s\" name=\"Uri\" direction=\"in\"/>\n"
    "    </method>\n"
    "    <signal name=\"Seeked\">\n"
    "      <arg type=\"x\" name=\"Position\" direction=\"out\"/>\n"
    "    </signal>\n"
    "    <property name=\"PlaybackStatus\" type=\"s\" access=\"read\"/>\n"
    "    <property name=\"LoopStatus\" type=\"s\" access=\"readwrite\"/>\n"
    "    <property name=\"Rate\" type=\"d\" access=\"readwrite\"/>\n"
    "    <property name=\"Shuffle\" type=\"b\" access=\"readwrite\"/>\n"
    "    <property name=\"Metadata\" type=\"a{sv}\" access=\"read\"/>\n"
    "    <property name=\"Volume\" type=\"d\" access=\"readwrite\"/>\n"
    "    <property name=\"Position\" type=\"x\" access=\"read\"/>\n"
    "    <property name=\"MinimumRate\" type=\"d\" access=\"read\"/>\n"
    "    <property name=\"MaximumRate\" type=\"d\" access=\"read\"/>\n"
    "    <property name=\"CanGoNext\" type=\"b\" access=\"read\"/>\n"
    "    <property name=\"CanGoPrevious\" type=\"b\" access=\"read\"/>\n"
    "    <property name=\"CanPlay\" type=\"b\" access=\"read\"/>\n"
    "    <property name=\"CanPause\" type=\"b\" access=\"read\"/>\n"
    "    <property name=\"CanSeek\" type=\"b\" access=\"read\"/>\n"
    "    <property name=\"CanControl\" type=\"b\" access=\"read\"/>\n"
    "  </interface>\n"
    "</node>\n";

static GDBusConnection *globalConnection = NULL;
static GMainLoop *loop;







GVariant* getMetadataForTrack() {
    GVariantDict dict;

    g_variant_dict_init(&dict, NULL);


    // mpris:trackid
    // playlist-pos < 0 if there is no playlist or current track

        g_variant_dict_insert(&dict, "pitch_correction", "b", player_pitch);
    // mpris:length
    if (player_filename){
    g_variant_dict_insert(&dict, "mpris:length", "x", (int64_t)(player_duration * 1000000.0));

    // initial value. Replaced with metadata value if available

    g_variant_dict_insert(&dict, "xesam:title", "s", player_filename);
    }

    return g_variant_dict_end(&dict);
}


static void onRootMethodCallHandler(GDBusConnection *connection, const char *sender, const char *objectPath,
                                    const char *interfaceName, const char *methodName, GVariant *parameters,
                                    GDBusMethodInvocation *invocation, void *userData) {

	if (strcmp(methodName, "Quit") == 0) {
		g_dbus_method_invocation_return_value(invocation, NULL);
	} else if (strcmp(methodName, "Raise") == 0) {
		g_dbus_method_invocation_return_value(invocation, NULL);
	} else {
		g_dbus_method_invocation_return_error(invocation, G_DBUS_ERROR, G_DBUS_ERROR_NOT_SUPPORTED,
                                              "Method %s.%s not supported", interfaceName, methodName);
	}
}

static GVariant* onRootGetPropertyHandler(GDBusConnection *connection, const char *sender, const char *objectPath,
                                          const char *interfaceName, const char *propertyName, GError **error,
                                          void *userData) {
	GVariant *result = NULL;

	if (strcmp(propertyName, "CanQuit") == 0) {
		result = g_variant_new_boolean(TRUE);
	} else if (strcmp(propertyName, "CanRaise") == 0) {
		result = g_variant_new_boolean(FALSE);
	} else if (strcmp(propertyName, "HasTrackList") == 0) {
		result = g_variant_new_boolean(FALSE);
	} else if (strcmp(propertyName, "Identity") == 0) {
		result = g_variant_new_string("musicpv");
	}   else if (strcmp(propertyName, "SupportedMimeTypes") == 0){
		GVariantBuilder *builder = g_variant_builder_new(G_VARIANT_TYPE("as"));
		g_variant_builder_add(builder, "s", "audio/mp3");
		result = g_variant_builder_end(builder);
	}
	return result;
}

static const GDBusInterfaceVTable rootInterfaceVTable = {
	onRootMethodCallHandler,
	onRootGetPropertyHandler,
	NULL,
	{0},

};

 void mpris_player_property_change(const char *name)
{

GVariantBuilder *builder = g_variant_builder_new(G_VARIANT_TYPE_ARRAY);

	g_variant_builder_add(builder, "{sv}", "Shuffle", g_variant_new_string(name));
	GVariant *signal[] = {
		g_variant_new_string(PLAYER_INTERFACE),
		g_variant_builder_end(builder),
		g_variant_new_strv(NULL, 0)
	};

	g_dbus_connection_emit_signal(globalConnection, NULL, OBJECT_NAME, PROPERTIES_INTERFACE, "PropertiesChanged",
                                  g_variant_new_tuple(signal, 3), NULL);

	g_variant_builder_unref(builder);

}
static void onPlayerMethodCallHandler(GDBusConnection *connection, const char *sender, const char *objectPath,
                                      const char *interfaceName, const char *methodName, GVariant *parameters,
                                      GDBusMethodInvocation *invocation, void *userData) {
	if (strcmp(methodName, "Next") == 0) {
		g_dbus_method_invocation_return_value(invocation, NULL);
		mpv_command(player_ctx, mpv_next);
	} else if (strcmp(methodName, "Previous") == 0) {
		g_dbus_method_invocation_return_value(invocation, NULL);

		mpv_command(player_ctx, mpv_prev);
	} else if (strcmp(methodName, "Pause") == 0) {
		g_dbus_method_invocation_return_value(invocation, NULL);
		int e = 1;
		mpv_set_property(player_ctx, "pause", MPV_FORMAT_FLAG, &e);

mpris_player_property_change("PlaybackStatus");
	} else if (strcmp(methodName, "PlayPause") == 0) {
		g_dbus_method_invocation_return_value(invocation, NULL);

		int e = -1;
		mpv_get_property(player_ctx, "pause", MPV_FORMAT_FLAG, &e);
		if (e == 0)
			e = 1;
		else
			e = 0;

		mpv_set_property(player_ctx, "pause", MPV_FORMAT_FLAG, &e);
mpris_player_property_change("PlaybackStatus");
	} else if (strcmp(methodName, "Stop") == 0) {
		g_dbus_method_invocation_return_value(invocation, NULL);
	} else if (strcmp(methodName, "Play") == 0) {

		int e = 0;
		mpv_set_property(player_ctx, "pause", MPV_FORMAT_FLAG, &e);
		g_dbus_method_invocation_return_value(invocation, NULL);

mpris_player_property_change("PlaybackStatus");
	} else if (strcmp(methodName, "Seek") == 0) {
		int64_t offset_us;  // in microseconds
		char *offset_str;
		g_variant_get(parameters, "(x)", &offset_us);
		double offset_s = offset_us / 1000000.0;
		offset_str = g_strdup_printf("%f", offset_s);

		const char *cmd[] = {"seek", offset_str, NULL};
		mpv_command_async(player_ctx, 0, cmd);
		g_dbus_method_invocation_return_value(invocation, NULL);
		g_free(offset_str);
	} else if (strcmp(methodName, "SetPosition") == 0) {
		g_dbus_method_invocation_return_value(invocation, NULL);
	} else if (strcmp(methodName, "OpenUri") == 0) {
		g_dbus_method_invocation_return_value(invocation, NULL);
	} else {
		g_dbus_method_invocation_return_error(
		    invocation, G_DBUS_ERROR, G_DBUS_ERROR_NOT_SUPPORTED,
		    "Method %s.%s not supported", interfaceName, methodName);
	}
}

static GVariant* onPlayerGetPropertyHandler(GDBusConnection *connection, const char *sender, const char *objectPath,
                                            const char *interfaceName, const char *propertyName, GError **error,
                                            void *userData) {
	GVariant *result = NULL;

	if (strcmp(propertyName, "PlaybackStatus") == 0) {
		int e = -1;
		mpv_get_property(player_ctx, "pause", MPV_FORMAT_FLAG, &e);
		if (e == 0 && player_filename)
			result = g_variant_new_string("Playing");
		else if (e == 1)
			result = g_variant_new_string("Paused");
		else

			result = g_variant_new_string("Stopped");

	} else if (strcmp(propertyName, "LoopStatus") == 0) {
		if (player_loop_file == 0 && player_loop == 1)

			result = g_variant_new_string("Playlist");
		else if (player_loop == 1)

			result = g_variant_new_string("File");
		else

			result = g_variant_new_string("None");
	} else if (strcmp(propertyName, "Rate") == 0) {
		result = g_variant_new("d", player_rate);
	} else if (strcmp(propertyName, "Shuffle") == 0) {
		result = g_variant_new_boolean(FALSE);
	} else if (strcmp(propertyName, "Metadata") == 0) {
		result = getMetadataForTrack();
	} else if (strcmp(propertyName, "Volume") == 0) {
		result = g_variant_new("d", player_volume / 100);
	} else if (strcmp(propertyName, "Position") == 0) {
		double ee = 0;
   mpv_get_property(player_ctx, "time-pos", MPV_FORMAT_DOUBLE,
				 &ee);

		result = g_variant_new("x", (int64_t)(ee * 1000000.0));

	} else if (strcmp(propertyName, "CanGoNext") == 0) {
		result = g_variant_new_boolean(TRUE);
	} else if (strcmp(propertyName, "CanGoPrevious") == 0) {
		result = g_variant_new_boolean(TRUE);
	} else if (strcmp(propertyName, "CanPlay") == 0) {
		result = g_variant_new_boolean(TRUE);
	} else if (strcmp(propertyName, "CanPause") == 0) {
		result = g_variant_new_boolean(TRUE);
	} else if (strcmp(propertyName, "CanSeek") == 0) {
		result = g_variant_new_boolean(TRUE);
	} else if (strcmp(propertyName, "CanControl") == 0) {
		result = g_variant_new_boolean(TRUE);
	}

	else {
		result = NULL;
		g_set_error(error, G_DBUS_ERROR, G_DBUS_ERROR_UNKNOWN_PROPERTY,
			    "Unknown property %s", propertyName);
	}

	 return result;
}

static int onPlayerSetPropertyHandler(GDBusConnection *connection, const char *sender, const char *objectPath,
                                      const char *interfaceName, const char *propertyName, GVariant *value,
                                      GError **error, gpointer userData) {

	if (strcmp(propertyName, "LoopStatus") == 0) {
		char *status;
		g_variant_get(value, "s", &status);
	} else if (strcmp(propertyName, "Rate") == 0) {

		player_rate  = g_variant_get_double(value) ;

		if (player_rate > 1.5){
		player_rate = 1.5;
		}
		else if (player_rate < 0.5){
		player_rate = 0.5;
		}

		mpris_player_property_change("Rate");
	mpv_set_property(player_ctx, "speed", MPV_FORMAT_DOUBLE, &player_rate);


	} else if (strcmp(propertyName, "Volume") == 0) {
		player_volume  = g_variant_get_double(value)*100 ;

		if (player_volume > 100){
		player_volume = 100;
		}

		mpris_player_property_change("Volume");
		mpv_set_property(player_ctx, "volume", MPV_FORMAT_DOUBLE, &player_volume);
	}

	return TRUE;
}

static const GDBusInterfaceVTable playerInterfaceVTable = {
	onPlayerMethodCallHandler,
	onPlayerGetPropertyHandler,
	onPlayerSetPropertyHandler,
	{0}
};

//***********
//* SIGNALS *
//***********





static void onBusAcquiredHandler(GDBusConnection *connection, const char *name, void *userData) {
	globalConnection = connection;
}

static void onNameAcquiredHandler(GDBusConnection *connection, const char *name, gpointer userData) {

    GError *error = NULL;
    Interfaces *ud = userData;
	g_dbus_connection_register_object(connection, OBJECT_NAME, ud->root_interface_info, &rootInterfaceVTable, userData, NULL,
                                      &error);

	g_dbus_connection_register_object(connection, OBJECT_NAME, ud->player_interface_info, &playerInterfaceVTable, userData, NULL,
                                 &error);

}

static void onConnotConnectToBus(GDBusConnection *connection, const char *name, void *user_data){



	g_main_loop_quit(loop);
}


void * mpris_init(void *args) {
	int ownerId;
	GMainContext *context = g_main_context_new();

fclose(stderr);
    Interfaces ud = {};
    GDBusNodeInfo *introspection_data = NULL;

	g_main_context_push_thread_default(context);

	introspection_data = g_dbus_node_info_new_for_xml(xmlForNode, NULL);

    ud.root_interface_info =
        g_dbus_node_info_lookup_interface(introspection_data, "org.mpris.MediaPlayer2");
    ud.player_interface_info =
        g_dbus_node_info_lookup_interface(introspection_data, "org.mpris.MediaPlayer2.Player");
	ownerId = g_bus_own_name(G_BUS_TYPE_SESSION, BUS_NAME, G_BUS_NAME_OWNER_FLAGS_REPLACE,
                             onBusAcquiredHandler, onNameAcquiredHandler, onConnotConnectToBus,
                             &ud, NULL);

	loop = g_main_loop_new(context, FALSE);
	g_main_loop_run(loop);

	g_bus_unown_name(ownerId);
	g_dbus_node_info_unref(introspection_data);
	g_main_loop_unref(loop);


	return 0;
}

void stopServer() {

	g_main_loop_quit(loop);
}
