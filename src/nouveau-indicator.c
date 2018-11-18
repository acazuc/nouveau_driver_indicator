#include <libappindicator/app-indicator.h>
#include <gtk/gtk.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

//#define FILENAME "/sys/kernel/debug/dri/0/pstate"
#define FILENAME "pstate_test"

static void menu_item_callback(GtkWidget *widget, char *data)
{
	(void)widget;
	(void)data;
}

int main(int argc, char **argv)
{
	gtk_init(&argc, &argv);
	AppIndicator *indicator = app_indicator_new("nouveau-indicator", "/tmp/nouveau_driver_indicator_icon.png", APP_INDICATOR_CATEGORY_HARDWARE);
	GtkWidget *indicator_menu = gtk_menu_new();
	int pstate_file = open(FILENAME, O_RDONLY);
	if (pstate_file == -1)
	{
		GtkWidget *indicator_menu_item = gtk_menu_item_new_with_label("No pstate file found");
		gtk_menu_append(GTK_MENU(indicator_menu), indicator_menu_item);
		goto display;
	}
	char data[4096];
	ssize_t ret = read(pstate_file, data, sizeof(data) - 1);
	if (ret == -1)
	{
		char tmp[1024];
		char *msg = "Failed to read pstate file: ";
		memcpy(tmp, msg, strlen(msg));
		strcat(msg, strerror(errno));
		GtkWidget *indicator_menu_item = gtk_menu_item_new_with_label(tmp);
		gtk_menu_append(GTK_MENU(indicator_menu), indicator_menu_item);
		goto display;
	}
	data[ret] = '\0';
	char *newline = data;
	while (1)
	{
		char *tmp = strchr(newline, '\n');
		if (!tmp && newline[0] == '\0')
			break;
		tmp[0] = '\0';
		GtkWidget *indicator_menu_item = gtk_menu_item_new_with_label(newline);
		gtk_menu_append(GTK_MENU(indicator_menu), indicator_menu_item);
		g_signal_connect(indicator_menu_item, "activate", G_CALLBACK(menu_item_callback), strdup(newline));
		newline = tmp + 1;
	}
display:
	gtk_widget_show_all(indicator_menu);
	app_indicator_set_status(indicator, APP_INDICATOR_STATUS_ACTIVE);
	app_indicator_set_menu(indicator, GTK_MENU(indicator_menu));
	gtk_main();
	return 0;
}

