#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include<pthread.h>
#include<string.h>

static GtkWidget *number1;
static GtkWidget *number2;
static GtkWidget *result;
static GtkWidget *title;
static GtkWidget *reset_btn;
static GtkWidget *bingo_btn;
static GtkWidget *number_display;
static GtkWidget *start_btn;
static GtkWidget *display_label;
int call_out_array[25];
int game_state=0;
int array[25];
int timer=6;
pthread_t newthread;
GtkWidget *widgets[5][5];

void hide_widgets(){
	gtk_widget_hide(start_btn);
	gtk_widget_hide(reset_btn);
	gtk_widget_hide(display_label);
}

void* incrementTimer(void * arg){
	while(1){
		if(game_state==25) {
			hide_widgets();
			gtk_widget_show(reset_btn);
			break;
		}else if(game_state==-1){
			break;
		}else{
			sleep(1);
			timer--;
			updateTime();
			if(timer==0){
				show_call_out_arry_number();
				sleep(4);
				reset_timer();	
			}
		}		
	}
	return NULL;
}

void reset_timer(){
	timer =6;
	gtk_label_set_text(GTK_LABEL(number_display),itoa(timer));
	gtk_widget_hide(display_label);
}

void show_call_out_arry_number(){
	char number[25];
	itoa(call_out_array[game_state],number);	
	gtk_label_set_text(GTK_LABEL(number_display),number);
	gtk_widget_show(display_label);
	game_state++;
}

void updateTime(){
	char label[25];
	itoa(timer,label);
	gtk_label_set_text(GTK_LABEL(number_display),label);
}

/* reverse:  reverse string s in place */
void reverse(char s[]){
     int i, j;
     char c;

     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
}  

 /* itoa:  convert n to characters in s */
void itoa(int n, char s[]){
     int i;
     i = 0;
     do {       /* generate digits in reverse order */
         s[i++] = n % 10 + '0';   /* get next digit */
     } while ((n /= 10) > 0);     /* delete it */
     s[i] = '\0';
     reverse(s);
}  

/*void do_calculate(GtkWidget *calculate, gpointer data) {
    	int num1 = atoi((char *)gtk_entry_get_text(GTK_ENTRY(number1)));
    	int num2 = atoi((char *)gtk_entry_get_text(GTK_ENTRY(number2)));

    	char buffer[32];
    	snprintf(buffer, sizeof(buffer), "result: %d", num1 + num2);

    	gtk_label_set_text(GTK_LABEL(result), buffer);
}*/

void add_buttons(GtkWidget *grid){
	int i,j,c=0;
	char label[10];
	// Make and add buttons for the numbers
	for(i = 0 ;i <5; i++){
		for(j = 0; j<5; j++){
			itoa(array[c],label);
			c++;
			widgets[i][j] = gtk_toggle_button_new_with_label(label);
			gtk_widget_set_size_request(widgets[i][j],60,60);
			gtk_grid_attach(GTK_GRID(grid),widgets[i][j],i,j,1,1);
		}	
	}
}

void shuffle_call_out_array(){
	int random, temp,i,j;
	// Create a list from 1 to 25
	for (i=0; i<25;i++){
 		call_out_array[i] = i+1;
	}
	for(j=0;j<2;j++){
		for(i = 0; i<25; i++){ 
			random = rand() %25;
			temp = call_out_array[i];
			call_out_array[i] = call_out_array[random];
			call_out_array[random] = temp;
		}
	}
}

void start_game(){
	game_state=0;
	shuffle_call_out_array();
	hide_widgets();
	gtk_widget_show(number_display);
	gtk_widget_show(reset_btn);
	pthread_create(&newthread, NULL, incrementTimer,NULL);
}

void shuffle_array(){
	int random, temp,i,j;
	// Create a list from 1 to 25
	for (i=0; i<25;i++){
 		array[i] = i+1;
	}
	// Shuffle the list twice
	for(j=0;j<2;j++){
		for(i = 0; i<25; i++){ 
			random = rand() %25;
			temp = array[i];
			array[i] = array[random];
			array[random] = temp;
		}
	}
}

void bingo(GtkWidget *bingo_btn, gpointer *data){

}

void reset_board(GtkWidget *reset_btn, gpointer * data){
	game_state=-1;
	timer = 6;
	gtk_label_set_text(GTK_LABEL(number_display)," ");
	hide_widgets();
	gtk_widget_hide(number_display);
	gtk_widget_show(start_btn);
	shuffle_array();
	int i,j,c=0;
	char label[10];

	for(i = 0 ;i <5; i++){
		for(j = 0; j<5;j++){
			itoa(array[c],label);
			c++;
			gtk_button_set_label(GTK_BUTTON(widgets[i][j]),label);	
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widgets[i][j]),FALSE);
		}
	}	
	
}


// gcc 007_gtk.c -o 007_gtk `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0`
int main(int argc, char **argv) {
	GtkWidget *window, *grid, *fixed;
    	gtk_init(&argc, &argv);

	srand(time(0));

    	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	gtk_window_set_default_size(GTK_WINDOW(window),400,500);
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);

	fixed = gtk_fixed_new();
    	grid = gtk_grid_new();

	title = gtk_label_new("BINGO");
	number_display = gtk_label_new(" ");
	display_label = gtk_label_new("The number is: ");
	reset_btn = gtk_button_new_with_label("Reset");
	bingo_btn = gtk_button_new_with_label("BINGO!");
	start_btn = gtk_button_new_with_label("Start");
		
	shuffle_array();	

	add_buttons(grid);

	g_signal_connect(reset_btn, "clicked", G_CALLBACK(reset_board),NULL);
	g_signal_connect(bingo_btn, "clicked", G_CALLBACK(bingo),NULL);
	g_signal_connect(start_btn, "clicked", G_CALLBACK(start_game),NULL);	

    	/*calculate = gtk_button_new_with_label("calculate");
    	g_signal_connect(calculate, "clicked", G_CALLBACK(do_calculate), NULL);
    	gtk_grid_attach(GTK_GRID(grid), calculate, 2, 0, 1, 1);

    	result = gtk_label_new("result:");
    	gtk_grid_attach(GTK_GRID(grid), result, 3, 0, 1, 1);*/

	gtk_container_add(GTK_CONTAINER(window), fixed);
    	
	gtk_fixed_put(GTK_FIXED(fixed),grid,50,100);

	gtk_fixed_put(GTK_FIXED(fixed),title,180,20);	
	
	gtk_fixed_put(GTK_FIXED(fixed),bingo_btn,40,450);
	gtk_fixed_put(GTK_FIXED(fixed),reset_btn,300,450);
	gtk_fixed_put(GTK_FIXED(fixed),start_btn,300,450);
	gtk_fixed_put(GTK_FIXED(fixed),number_display,195,65);
	gtk_fixed_put(GTK_FIXED(fixed),display_label,160,50);

    	gtk_widget_show_all(window);
	hide_widgets();
	gtk_widget_show(start_btn);
    	gtk_main();

    	return 0;
}
