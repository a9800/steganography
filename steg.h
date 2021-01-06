#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// A struct to store the rgb values
struct pixel {
    int red; 
    int green; 
    int blue;
};

// A struct for the PixelNodes
struct pixelNode {
    //Pointer to the pxl struct that stores the RGB values
    struct pixel * pxl;
    //pointer to next pixelNode
    struct pixelNode * nextPixel;
};

// A struct for comment nodes
struct commentNode {
    //pointer to the comment string
    char * cmnt; 
    //pointer to next commentNode
    struct commentNode * nextCmnt;
};

// The struct that holds the PPM values
struct PPM {
    // Nodes have head and tail to increase speed
    struct commentNode * cmntHead;
    struct commentNode * cmntTail; 
    int width; 
    int height; 
    int max; 
    struct pixelNode * pxlHead; 
    struct pixelNode * pxlTail;
};

// An enum to keep track of which part we are storing from the image
enum ppmState {ppmCode, dimensions, max, pixel};

void addPixel(struct pixel * pxl, struct PPM * picture);

void addComment(char * cmnt, struct PPM * picture);

struct PPM * getPPM(FILE * f);

void showPPM(struct PPM * im);

int getLineLength(char str[]);

void incrementPtr(char * ptr);

int getLSB(int pxl);

int encodePxl(int pxl, int letter);

struct PPM * encode(struct PPM * im, char * message, unsigned int mSize, unsigned int secret);