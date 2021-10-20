/*
 * Mathew Bernard
 * MSCS Ready, Fall 2021
 * Your job:
 * 1 - add code to create a palette array "on the fly"
 * 2 - implement show_palette
 * 3 - implement choose_closest(struct color pal[], struct color orig_color);
 * 4 - implement code in map_to_palette that invokes choose_closest  
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* define a structure named color */
struct color
{
    char name[20];
    char code;
    int  red_val;
    int  green_val;
    int  blue_val;
};


/* function prototypes */
int get_palette(char fname[], struct color pal[], int size);

void show_palette(struct color pal[], int size);

struct color choose_closest(struct color pal[], struct color orig_color,
		int size);

void map_to_palette(char input_image_fname[80], char 
	output_image_fname[80], struct color palette[], int palette_size);


int main(){

	// variables for filenames
	char	palette_fname[80];
	char	input_image_fname[80];
	char	output_image_fname[80];

	// variables for the palette
	int palette_size;

	
	/* get name of palette file */
	printf("Please enter name of file containing color palette: ");
	scanf("%s", palette_fname);

	/* get size of palette file */
	printf("Please enter number of lines in palette file :");
	scanf("%d", &palette_size);

	struct color myPalette[palette_size];


	/* invoke get_palette to fill the array with values from the file */
	get_palette(palette_fname, myPalette, palette_size);

	/* confirm palette contents */
	void show_palette(struct color pal[], int size);


	/* get the name of an input image file */
	printf("\nEnter name of a P6 ppm file containing the original image: ");
	scanf("%s", input_image_fname);

	/* get the name of an output image file */
	printf("\nEnter desired name of P6 ppm file containing output image: ");
	scanf("%s", output_image_fname);

	/*
  	 * map_to_palette should read the pixels in the input file,
	 * map them to the closest colors in the palette,
	 * and write them to the output file
	 */
	map_to_palette(input_image_fname, output_image_fname, myPalette,
			palette_size);

}

/*
 * get_palette reads n=size lines from the file named in palette_fname
 * for each color definition, it creates a color struct 
 * and places the colors in pal, an array of color structs
 */
int get_palette(char palette_fname[], struct color pal[], int size){

	FILE *fp;
	int counter = 0;
	char line[81];

	/* open palette file; report error and exit if unsuccessful */
	if ((fp = fopen(palette_fname, "r")) == NULL) {
        printf("Error! opening file %s \n", palette_fname);
        // Program exits with error code = 1 if file pointer returns NULL.
        exit(1);
    }

	/* read a line at time from the file */
	while(fgets(line, sizeof(line), fp)) {
		sscanf(line,"%c %d %d %d %s \n", &(pal[counter].code),
			&(pal[counter].red_val), &(pal[counter].green_val),
			&(pal[counter].blue_val), pal[counter].name);
	   counter++;
	}

	fclose(fp);
	return(0);
}

/*
 * #2 - implementation of show_palette goes here 
 */

void show_palette(struct color pal[], int size){

	int counter = 0;
	for( ; counter  < size; counter++){
		printf("%s %c %i %i %i \n", (pal[counter].name),
				(pal[counter].code), (pal[counter].red_val),
				(pal[counter].green_val), (pal[counter].blue_val));
		counter++;

	}
}

/*
 * #3 - implementation of choose_closest goes here 
 */
struct color choose_closest(struct color pal[], struct color orig_color,
		int size){


	struct color finalColor;
	int counter;
	float distance_to_current, distance_to_closest;

	finalColor.red_val = pal[0].red_val;
	finalColor.green_val = pal[0].green_val;
	finalColor.blue_val = pal[0].blue_val;

	distance_to_closest = (float)sqrt((finalColor.red_val * 
				finalColor.red_val) + (finalColor.green_val * 
				finalColor.green_val) + (finalColor.blue_val *
				finalColor.blue_val));

	distance_to_current = distance_to_closest;

	for (counter = 1; counter < size; counter++){
	distance_to_current = (float)sqrt(
			((pal[counter].red_val - orig_color.red_val) *  
			(pal[counter].red_val - orig_color.red_val)) +
			((pal[counter].green_val - orig_color.green_val) *
			(pal[counter].green_val - orig_color.green_val)) +
			((pal[counter].blue_val - orig_color.blue_val) * 
			(pal[counter].blue_val - orig_color.blue_val)));
		counter++;

		if (distance_to_current < distance_to_closest){
			distance_to_closest = distance_to_current;
			finalColor.red_val = pal[counter].red_val;
			finalColor.green_val = pal[counter].green_val;
			finalColor.blue_val = pal[counter].blue_val;
			finalColor.code = pal[counter].code;
			strcpy(finalColor.name, pal[counter].name);
		}
	}

	
	return (finalColor);
}	





void map_to_palette(char input_image_fname[80], char output_image_fname[80], 
						struct color palette[], int palette_size){

	FILE *infile;
	FILE *outfile;
	int color_depth, height, width;
	char filetype[3];
	unsigned char c;
	unsigned char red, green, blue;

	/* open input file; report error and exit if unsuccessful */
	infile= fopen(input_image_fname, "r");
	if (!infile) {
         fprintf(stderr, "Unable to open file '%s' for reading \n", 
				input_image_fname);
         exit(1);
   }

	/* get image format; report error and exit if unsuccessful */
	if (!fgets(filetype, sizeof(filetype), infile)) {
  		perror(input_image_fname);
		exit(1);
	}

	/* check image format; report error and exit if unsuccessful */
	if (strcmp(filetype, "P6")){
		fprintf(stderr, "Invalid image format (must be 'P6')\n");
		exit(1);
	}
	else{
		printf("Confirming: file type is %s \n", filetype);
	}

	c = getc(infile);	
	// for every comment line
	while (c == '#'){
		// throw away everything in the line
		while (getc(infile) != '\n');
		// check the next line
		c = getc(infile);	
	}

	/* put back that character that wasn't the start of a comment line */
	ungetc(c, infile);

	/* get dimensions; report error and exit if unsuccessful  */
	if (fscanf(infile, "%d %d", &width, &height)!=2){
		fprintf(stderr, "Invalid image size (error loading'%s') \n", 
			input_image_fname);
		exit(1);
	}
	else{
		printf("Confirming: dimensions are width: %d by height: %d \n", 
			width, height);
	}

	/* get color depth ; report error and exit if unsuccessful */
	if (fscanf(infile, "%d\n", &color_depth)!=1){
		fprintf(stderr, "Invalid color depth (error loading'%s') \n", 
			input_image_fname);
		exit(1);
	}
	else{
		printf("Confirming: color_depth: %d \n", 
			color_depth);
	}
	
	/* open output file */
	outfile= fopen(output_image_fname, "w");
	if (!outfile) {
         fprintf(stderr, "Unable to open file '%s' for writing\n", 
				output_image_fname);
         exit(1);
   }
	else{
		printf("File '%s' open for writing\n", output_image_fname);
	}


	/* write image format, width, height and color dimension */
   fprintf(outfile, "P6\n %d %d %d\n", width, height, color_depth);

	 // for every pixel in the input file 
	 for (int i= 0; i < height*width; i++){
		/* read the red, green and blue values 
	    * from the input image INTO A COLOR STRUCT
		 */
  
		 fscanf(infile,"%c%c%c",&red, &green, &blue);

		/* send that COLOR STRUCT to a function (choose_closest) that chooses 
		 * the color from the palette that is the closest to 
		 * this color and returns a color struct containing that color
		 */

	struct color orig_color;
	orig_color.red_val = red;
	orig_color.green_val = green;
	orig_color.blue_val = blue;

		// your code for #4 goes here 

	struct color selected_color = choose_closest( palette, orig_color,
			palette_size );
	
		/*
		 * write out a pixel to the new file that contains 
		 * the updated color for that pixel FROM THE STRUCT 
		 */
      fprintf(outfile,"%c%c%c", selected_color.red_val,
				selected_color.green_val, selected_color.blue_val);
	 }


};


