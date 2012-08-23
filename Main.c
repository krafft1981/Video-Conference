#include <gtk/gtk.h>
#include <glib.h>


#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <gst/gst.h>

#include "player-xml.h"



int rtp_src_v, rtcp_src_v,rtcp_sink_v;
int rtp_src_v_c, rtcp_src_v_c,rtcp_sink_v_c;
int mutex=0;

gint delete_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
  return(FALSE);
}




void destroy(GtkWidget *widget, gpointer data)
{
  gtk_main_quit();
}



static gpointer
thread_func( gpointer data )
{
    while( TRUE )
    {
	sleep( 3 );
	if(stream_on && mutex<1){
       
	mutex++;
        client_video_stream(rtp_src_v_c, rtcp_src_v_c,rtcp_sink_v_c);
	//loopCl = g_main_loop_new (NULL, FALSE);
        //g_main_loop_run (loopCl);
	gst_element_set_state (pipelineVC, GST_STATE_PLAYING);
        }
    }

    return( NULL );
}



void on_click(GtkWidget *widget, gpointer data) {
    

    stream_on=TRUE;   
   //Dynamic ports
   const gchar *text = gtk_entry_get_text( port );
   rtp_src_v=atoi(text);
   rtcp_src_v=rtp_src_v+1;
   rtcp_sink_v=rtp_src_v+4;
      const gchar *textC = gtk_entry_get_text( portC );
   rtp_src_v_c=atoi(textC);
   rtcp_src_v_c=rtp_src_v_c+1;
   rtcp_sink_v_c=rtp_src_v_c+4;

   server_video_stream(rtp_src_v, rtcp_src_v,rtcp_sink_v);
   
	gst_element_set_state (pipelineVideo, GST_STATE_PLAYING);
   /*gdk_threads_enter();
   if(!g_thread_create( thread_func, NULL,FALSE, &t_error ))
    {
		g_printerr ("Failed to create YES thread: %s\n", t_error->message);
	        return 1;
    }
   /gdk_threads_leave();
    */

}


int main(int argc, char *argv[]) {
     gdk_threads_init();

    /* Obtain gtk's global lock */
    

    /* Do stuff as usual */
    gtk_init( &argc, &argv );
    gtk_init(&argc, &argv);
    gst_init (&argc, &argv);
	GMutex* lock;
	GList *list;
	

  
    t_error=NULL;
    builder = gtk_builder_new();
    if (!gtk_builder_add_from_file(builder, "Video-Conference.glade", NULL)) {
        g_print("Error opening file");
        return 0;
    }

    

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    vBox = GTK_WIDGET(gtk_builder_get_object(builder, "vbox1"));
    hBox = GTK_WIDGET(gtk_builder_get_object(builder, "hbox1"));
    menu = GTK_MENU_BAR(gtk_builder_get_object(builder, "menubar1"));
    client_video = GTK_WIDGET(gtk_builder_get_object(builder, "user_drawingareaClient"));
    remote_video = GTK_WIDGET(gtk_builder_get_object(builder, "user_drawingareaRemote"));
    aspectFrameClient = GTK_WIDGET(gtk_builder_get_object(builder, "user_frame"));
    aspectFrameRemote = GTK_WIDGET(gtk_builder_get_object(builder, "user_frame1"));
    label_remote = GTK_WIDGET(gtk_builder_get_object(builder, "frame_label"));
    label_client = GTK_WIDGET(gtk_builder_get_object(builder, "frame_label1"));
    button = GTK_WIDGET(gtk_builder_get_object(builder, "button"));
    port = GTK_ENTRY(gtk_builder_get_object(builder, "Port"));
    portC = GTK_ENTRY(gtk_builder_get_object(builder, "PortC"));
    gtk_signal_connect(GTK_OBJECT (window), "delete_event",
		     GTK_SIGNAL_FUNC(delete_event), NULL);
          
  gtk_signal_connect(GTK_OBJECT (window), "destroy",
		     GTK_SIGNAL_FUNC(destroy), NULL);
    gtk_signal_connect(GTK_OBJECT (button), "clicked", GTK_SIGNAL_FUNC(on_click), NULL);

          
 
/*
    //Ports
    //Video
    RTP_SRC_V=8002;
    RTCP_SRC_V=8003;
    RTCP_SINK_V=8007;

    //Audio
    RTP_SRC_A=5002;
    RTCP_SRC_A=5003;
    RTCP_SINK_A=5007;
*/



if(!g_thread_create( thread_func, NULL,FALSE, &t_error ))
    {
		g_printerr ("Failed to create YES thread: %s\n", t_error->message);
	        return 1;
    }
    
   
    gtk_builder_connect_signals(builder, NULL);

    g_object_unref(G_OBJECT(builder));

    gtk_widget_show_all(window);
    gdk_threads_enter();
    //loopSv = g_main_loop_new (NULL, FALSE);
    //g_main_loop_run (loopSv);
   
    gtk_main();
    

    gdk_threads_leave();

    exit(0);
}
