#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <libguile.h>

static const int WIDTH  = 10;
static const int HEIGHT = 10;
static FILE *global_output;

static SCM start_gnuplot()
{
	FILE *output;
	int pipes[2];
	pid_t pid;

	pipe(pipes);
	pid = fork();

	if (!pid)
	{
		dup2(pipes[0], STDIN_FILENO);
		char *args[] = { NULL };
		printf("starting gnuplot...\n");
		execvp("gnuplot", args);
		return SCM_UNSPECIFIED;
	}

	printf("sending stuff...\n");

	output = fdopen(pipes[1], "w");

	fprintf (output, "set multiplot\n");
	fprintf (output, "set parametric\n");
	fprintf (output, "set xrange [-%d:%d]\n", WIDTH, WIDTH);
	fprintf (output, "set yrange [-%d:%d]\n", HEIGHT, HEIGHT);
	fprintf (output, "set size ratio -1\n");
	fprintf (output, "unset xtics\n");
	fprintf (output, "unset ytics\n");
	fflush (output);

	global_output = output;
	return SCM_UNSPECIFIED;
}

static SCM stop_gnuplot()
{
	fprintf (global_output, "exit\n");
	fflush (global_output);
	fclose(global_output);
	return SCM_UNSPECIFIED;
}


static void draw_line (FILE* output, double x1, double y1, double x2, double y2)
{
	fprintf (output, "plot [0:1] %f + %f * t, %f + %f * t notitle\n",
			x1, x2 - x1, y1, y2 - y1);
	fflush (output);
}

static double x, y;
static double direction;
static int pendown;

static SCM turtle_reset ()
{
	x = y = 0.0;
	direction = 0.0;
	pendown = 1;

	fprintf (global_output, "clear\n");
	fflush (global_output);

	return SCM_UNSPECIFIED;
}

static SCM turtle_pendown ()
{
	SCM result = scm_from_bool (pendown);
	pendown = 1;
	return result;
}

static SCM turtle_penup ()
{
	SCM result = scm_from_bool (pendown);
	pendown = 0;
	return result;
}

static SCM turtle_turn (SCM degrees)
{
	const double value = scm_to_double (degrees);
	direction += M_PI / 180.0 * value;
	return scm_from_double (direction * 180.0 / M_PI);
}

static SCM turtle_move (SCM length)
{
	const double value = scm_to_double (length);
	double newX, newY;

	newX = x + value * cos (direction);
	newY = y + value * sin (direction);

	if (pendown)
		draw_line (global_output, x, y, newX, newY);

	x = newX;
	y = newY;

	return scm_list_2 (scm_from_double (x), scm_from_double (y));
}

static void *register_functions (void* data)
{
	scm_c_define_gsubr ("start-gnuplot", 0, 0, 0, &start_gnuplot);
	scm_c_define_gsubr ("stop-gnuplot", 0, 0, 0, &stop_gnuplot);
	scm_c_define_gsubr ("turtle-reset", 0, 0, 0, &turtle_reset);
	scm_c_define_gsubr ("turtle-penup", 0, 0, 0, &turtle_penup);
	scm_c_define_gsubr ("turtle-pendown", 0, 0, 0, &turtle_pendown);
	scm_c_define_gsubr ("turtle-turn", 1, 0, 0, &turtle_turn);
	scm_c_define_gsubr ("turtle-move", 1, 0, 0, &turtle_move);

	return NULL;
}

static void inner_main(void *closure, int argc, char *argv[])
{
	//start_gnuplot();

}

int main(int argc, char *argv[])
{
	//scm_boot_guile(argc, argv, inner_main, 0);
	scm_with_guile(&register_functions, NULL);
	scm_shell(argc, argv);

	return EXIT_SUCCESS;
}
