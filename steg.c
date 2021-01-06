#include "steg.h"

//Method for adding pixel to linked list
void addPixel(struct pixel * pxl, struct PPM * picture){
    //Checking if pixelHead is storing nothing
    if(picture->pxlHead->pxl == NULL){
        //if it is set its values to be the same as pxl
        picture->pxlHead->pxl = pxl; 
    }else{
        //otherwise create a new node with the new values and add it to the end of tail and make this new node the Tail
        struct pixelNode * pNode = malloc(sizeof(struct pixelNode));
        pNode->pxl = pxl;
        pNode->nextPixel = NULL;
        picture->pxlTail->nextPixel = pNode;
        picture->pxlTail = pNode;
    }
}

//Method for adding comment to comment linked list
void addComment(char * cmnt, struct PPM * picture){
    //Checking if cmntHead is storing nothing
    if(picture->cmntHead->cmnt == NULL){
        //if it is set its values to be the same as cmnt
        picture->cmntHead->cmnt = cmnt; 
    }else{
        //otherwise create a new node with the correct values and add it to the end of tail and make this new node the Tail
        struct commentNode * cNode = malloc(sizeof(struct commentNode));
        cNode->cmnt = cmnt;
        cNode->nextCmnt = NULL;
        picture->cmntTail->nextCmnt = cNode;
        picture->cmntTail = cNode;
    }
}
//Function to increment a pointer until it reaches a charachter other than ' '
void incrementPtr(char * ptr){
    while(ptr[0] == ' ' && ptr[1] == ' '){
        ptr++;
    }
}
//Function to get the length of the Line
int getLineLength(char str[]){
    int i;
    for(i = 0; str[i] != '\0'; ++i);
    return i;
    
}

//Function to vreate a PPM struct using the values of a PPM image
struct PPM * getPPM(FILE * f){
    int pixelCount = 0;
    size_t count = 0;
    size_t commentCount = 0;
    
    char line[85];
    char commentLine[100];
    memset(line, '\0', sizeof(line));

    //Getting the first character of the PPM
    char ch;
    ch = getc(f);
    
    //Setting the Enum state to ppmCode
    enum ppmState state = ppmCode;
    struct PPM * picture = malloc(sizeof(struct PPM));
    bool comment = false;
    
    //Mallocing and creating the Head Nodes for commentNodes and PixellNodes and setting their values to NULL
    picture->pxlHead = picture->pxlTail = malloc(sizeof(struct pixelNode));
    picture->pxlHead-> nextPixel = NULL;
    picture->pxlHead-> pxl = NULL;
    
    picture->cmntHead = picture->cmntTail = malloc(sizeof(struct commentNode));
    picture->cmntHead->cmnt = NULL;
    picture->cmntHead->nextCmnt = NULL;

    //A while loop that runs until it reaches the End of the file
    while(ch != EOF){
        //Checking if the comment bool is true and that it is not the end of the line
        if(comment && ch != '\n'){
            if(commentCount<sizeof(commentLine)){
                //Adds appends ch to commentLine string
                commentLine[commentCount++] = ch;
            }
        }
        //Checking if comment is not true and that it is not the end of the line
        if(!comment && ch != '\n'){
            if(count<sizeof(line)){
                //It appends ch to the line string
                line[count++] = ch;
            }
            //if ch is a # then set the comment bool to be true
            if(ch == '#'){
                comment = true;
            }
        
        }
        //When the ch is new line
        if(ch == '\n'){
            //if comment is true
            if(comment){
                //Creating a new string mallocing the size of comment Line
                char * finalCmnt = malloc(sizeof(commentLine));
                //Copuing the values from Comment Line to final Comment
                memcpy(finalCmnt,commentLine,sizeof(commentLine));
                //Creating new commentNode and adding it to struct
                addComment(finalCmnt, picture);
                //printf("cline:#%s\n#%s\n",cLine, commentLine);
            }
            //Checking if it is not a comment and count not zero
            if(count != 0 && !comment){
                //Switch statement to check what the enum is
                switch(state){

                    case ppmCode:
                        //If the line is P3 then change state to dimensions
                        if(strcmp(line,"P3") == 0 ) {
                            state = dimensions;
                        }
                        break;

                    case dimensions:
                    {
                        char *ptr;
                        long width;
                        long height;
                        //Width and height are on the same line so we use strtol to get their values
                        width = strtol(line, &ptr, 10);
                        ptr++;
                        height = strtol(ptr, &ptr, 10);

                        //adding width and height to the PPM struct
                        picture->width = (int) width;
                        picture->height = (int) height;
                        //printf("width: %ld\n", width);
                        //printf("height: %ld\n", height);
                        //Setting the state to Max
                        state = max;
                        break;
                    }
                    case max: 
                    {   
                        //Getting the max and adding it to the PPM struct then changing state to pixel
                        char *ptr;
                        long max;

                        max = strtol(line, &ptr, 10);

                        picture->max = (int) max;
                        //printf("max: %ld\n", max);
                        state = pixel;
                        break;
                    }
                    case pixel:
                    {    
                        
                        char *ptr = &line[0];
                        while(true){
                            //Creating pixel struct
                            struct pixel * pxl = malloc(sizeof(struct pixel));
                            long red;
                            long green;
                            long blue;
                            
                            //Incrementing Pixel Count
                            pixelCount++;

                            //Use strtol to get the values of the pixel
                            red = strtol(ptr, &ptr, 10);
                            //Then use increment Ptr to incremenet the pointer past all the spaces to the next character
                            incrementPtr(ptr);
                            
                            green = strtol(ptr, &ptr, 10);
                            incrementPtr(ptr);
                            
                            blue = strtol(ptr, &ptr, 10);
                            incrementPtr(ptr);
                            
                            //Setting the value of the struct
                            pxl->red = (int) red;
                            pxl->green = (int) green;
                            pxl->blue = (int) blue;

                            //printf("adding pixel r: %ld g:%ld b:%ld \n", red, green, blue);
                            //printf("pixel count : %d \n", pixelCount);

                            //Addung the pixel to the linked list
                            addPixel(pxl, picture);
                            
                            //If the Ptr got the the 3 rgb values then break
                            if(getLineLength(ptr)<3){
                                break;
                            }
                        }
                        break;
                    }

                }
            }
            //Setting comment line and line to \0 and the comment to false and comment count and count to zero
            memset(commentLine, '\0', sizeof(commentLine));
            commentCount = 0;
            memset(line, '\0', sizeof(line));
            comment = false;
            count = 0;
        }
        //Getting the next character
        ch = getc(f);
    
    }
    //returning the ppm struct
    return picture;
}

//Function to print the PPM
void showPPM(struct PPM * im){
    printf("P3\n");
    
    //Setting tmp node to be cmnt head
    struct commentNode * tmpCmntNode = im->cmntHead;
    //Checking if the tmpCmnt is storing a value
    if(tmpCmntNode->cmnt != NULL){
        //While tmpCMnt is not null print out its value and then set tmpCmnt to be the next CommentNode
        while(tmpCmntNode != NULL){
            printf("#%s\n", tmpCmntNode->cmnt);
            tmpCmntNode = tmpCmntNode->nextCmnt;
        } 
    }
    
    //Printing the width height and max
    printf("%d %d\n%d\n",im->width,im->height,im->max);

    //Setting tmp node to be pixel head
    struct pixelNode * tmpPxlNode = im->pxlHead;
    while(tmpPxlNode != NULL){
        //While tmpPxlNode is not null print out its pxl values then set tmpPxlNode to be next PixelNode
        printf("%d %d %d\n", tmpPxlNode->pxl->red, tmpPxlNode->pxl->green, tmpPxlNode->pxl->blue);
        tmpPxlNode = tmpPxlNode->nextPixel;
    }
}

/*
  Using bit wise operations to get the binary value at a index of a char
  Example:
  The binary value of char a is 01100001
  getCharBit(a,0) would return 1 
*/
int getCharBit(int ch, int bitIndex){
    //Shifting the binary values of the ch to the right by the value of bit index
    //Then foing & 1 to return the bit on the right
    return ((ch>>bitIndex) & 1);
}

//Using Bitwise operations to get the Least significant Bit
int getLSB(int colour){
    return (colour & 1);
}

//Using Bitwise operations to change the LSB
int encodePxl(int pxl, int letter){
    //We Set the LSB to 0 then we can change it to be the number of the bit of the letter
    return ((pxl & 0xFE)|letter);  
}

struct PPM * encode(struct PPM * im, char * msg, unsigned int mSize, unsigned int secret){
    //adding a character with value of all zeros to siginify that we reached the end of the message
    char message[mSize+1];
    memset(message,'\0',sizeof(message));
    memcpy(message,msg,mSize);
    //char end = 0x00;
    //message[strlen(message)] = end; 
    //setting the seed for the random variable so that it produces the same sequence of integers
    srand(secret);
    int pixelCount = 0;
    //Setting tmpPxlNode to be pxlHead
    struct pixelNode * tmpPxlNode = im->pxlHead;
    int chCount = 0;
    //Setting ch as the first character of message
    char ch = message[chCount];
    int bitIndex = 7;
    //adding one to random as rand()%6 can equal zero which can cause pixelCount to be greater than random
    int random = (rand()%6) +1;


    while(tmpPxlNode!=NULL && chCount < mSize){  
        //Checking if the pixelCount is equal to the random variable
        if(pixelCount == random){
            //printf("random: %d\n", random);
            int i;
            //encoding the Pixel values using bitwise operarations
            for(i = 0;i < 3; i++){
                if(i == 0){
                    tmpPxlNode->pxl->red = encodePxl(tmpPxlNode->pxl->red,getCharBit(ch,bitIndex));
                    //printf("%d ", getLSB(tmpPxlNode->pxl->red));
                }
                if(i == 1){
                    tmpPxlNode->pxl->green = encodePxl(tmpPxlNode->pxl->green,getCharBit(ch,bitIndex));
                    //printf("%d ",getLSB(tmpPxlNode->pxl->green));
                    }
                if(i == 2){
                    tmpPxlNode->pxl->blue = encodePxl(tmpPxlNode->pxl->blue,getCharBit(ch,bitIndex));
                    //printf("%d ",getLSB(tmpPxlNode->pxl->blue));
                }
                //Incrementing the character count after getting to the end of the character and set the bitIndex to 7
                if(bitIndex == 0){
                    bitIndex = 7;
                    if(chCount >= sizeof(message)){
                        tmpPxlNode = NULL;
                        break;
                    }
                    chCount++;
                    ch = message[chCount];
                    //printf("\n");        
                }else{
                    //If bit index is not zero we decrement bit Index
                    bitIndex--;
                } 
            }
            //Setting a new random after finishing encoding a pixel
            random = random + (rand()%6) + 1 ;
        }
        //Set tmpPxlNode to be next pixel Node and increment pixel count
        if(tmpPxlNode != NULL){
            tmpPxlNode = tmpPxlNode->nextPixel;
            pixelCount++;
        }
        
    }
    //printf("pixel count: b%d\n", pixelCount);
    return im;
}

char * decode(struct PPM * im, unsigned int secret){
    //setting the seed for the random variable so that it produces the same sequence of integers
    srand(secret);
    int pixelCount = 0;
    //Setting tmpPxlNode to be pxlHead
    struct pixelNode * tmpPxlNode = im->pxlHead;
    int chCount = 0;
    int bitCount = 0;
    //Getting a random value using the seed
    int random = (rand()%6) + 1;
    char message[128];
    //Setting ch and lsb to 0
    char ch = 0;
    int lsb = 0;

    while(tmpPxlNode!=NULL){
        //Checking if the pixelCount is equal to the random variable
        if(pixelCount==random){
            //printf("random: %d\n", random);
            random = random + (rand()%6) + 1;
            int i;
            for(i = 0; i < 3; i++){
                if(i == 0){
                    //Getting the lsb of the pixel colour value
                    lsb = getLSB(tmpPxlNode->pxl->red);
                    
                    //Shifting the ch binary value to the left
                    ch = ch<<1;
                    //printf("%d ",lsb);

                    //Setting the last bit of ch to be lsb
                    ch = encodePxl(ch,lsb);
                    
                }
                if(i == 1){
                    lsb = getLSB(tmpPxlNode->pxl->green);
                    ch = ch<<1;
                    //printf("%d ",lsb);
                    ch = encodePxl(ch,lsb);
                    
                }
                if(i == 2){
                    lsb = getLSB(tmpPxlNode->pxl->blue);
                    ch = ch<<1;
                    //printf("%d ",lsb);
                    ch = encodePxl(ch,lsb);
                    
                }
                if(bitCount == 7){
                    //Once we are finsihed getting all the bits of a character we append ch to to message and increment chCount
                    message[chCount] = ch;
                    chCount++;
                    //printf("\n");
                    //printf("  %c \n",ch);

                    //to break from the while loop when we reach the end of the message
                    if(ch == '\n'){
                        //Setting the last ch of message to be \0 to signify that it is the end of the message
                        message[chCount]='\0';
                        tmpPxlNode = NULL;
                        //printf("%s\n", message);
                        break;
                    }
                    
                    //Set ch to 0 and bit count as 0
                    ch = 0;
                    bitCount = 0;
                }else{
                    //Else if we didnt finish creating the character we increment BitCount
                    bitCount++;
                }
            }
        }
        if(tmpPxlNode!=NULL){
            //If tmpPxlNode not null then set tmpPxlNode to be next pixel node and increment picelCount
            tmpPxlNode = tmpPxlNode->nextPixel;
            pixelCount++;
        }
        
    }
     
    //Creating a new character array and mallocing the size of message 
    char *finalMsg = malloc(sizeof(message));
    //Copying messages value to finalMsg
    strcpy(finalMsg, message);

    return finalMsg;
}
void deallocImg(struct PPM * im){
    //I free everything that i used malloc for
    struct pixelNode * tmpPxlNode = im->pxlHead;
    while(tmpPxlNode != NULL){
        struct pixelNode * pNode = tmpPxlNode->nextPixel;
        free(tmpPxlNode->pxl);
        free(tmpPxlNode);
        tmpPxlNode = pNode;
    }

    struct commentNode * tmpCmntNode = im->cmntHead;
    while(tmpCmntNode != NULL){
        struct commentNode * cNode = tmpCmntNode->nextCmnt;
        free(tmpCmntNode->cmnt);
        free(tmpPxlNode);
        tmpCmntNode = cNode;
    }
}

int main ( int argc, char **argv ) {
    if(argc == 2){
        fprintf(stderr,"Usage: %s [e for encrypt or d for decode] [PPM image] \n", argv[0]);
    }else{
        FILE * PPMimage;

        //Setting PPM image to be the second argument
        PPMimage = fopen(argv[2],"r");

        if(PPMimage == NULL){
           //If the PPM image can not be opened
            printf("Could not open file\n");

        }else{
            //If the first argument is e run the code for encoding
            if(strcmp(argv[1],"e")==0){
                struct PPM * PPMpic = getPPM(PPMimage);
                char msg[128];
                int secret;
            
                fprintf(stderr,"Enter the message that you wish to encrypt:");
                //Getting the message thhat will be encrypted
                fgets(msg, sizeof(msg),stdin);
            
                while(strlen(msg)==1){
                    fprintf(stderr,"The message must be more than zero characters, Please enter again:");
                    fgets(msg, sizeof(msg),stdin);
                }
                //Getting the secret number that will be used to encrypt
                fprintf(stderr,"Enter the secret number:");
                scanf("%d", &secret);
                while(secret < 0){
                    fprintf(stderr,"Please Enter a positive number: ");
                    scanf("%d", &secret);
                }
            
                PPMpic = encode(PPMpic, msg, strlen(msg), secret);
                showPPM(PPMpic);
                deallocImg(PPMpic);
            }
            //if the first argument is d run the code for decoding
            if(strcmp(argv[1],"d")==0){
                int secret;
                struct PPM * PPMpic = getPPM(PPMimage);
                //showPPM(PPMpic);
                //Getting the secret number that will be used to decrypt
                fprintf(stderr,"Enter the secret number:");
                scanf("%d", &secret);
                while(secret < 0){
                    fprintf(stderr,"\n Please Enter a positive number \n");
                }
            
                char * decodedMsg = decode(PPMpic, secret);
                printf("Encrypted Message: %s\n", decodedMsg);
                deallocImg(PPMpic);
            }
        }
    }
}