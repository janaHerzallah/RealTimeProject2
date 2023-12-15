
#include "header.h"
//#include <GL/glut.h>
#include "shared_memories.h"
#include "customer_header"
#include "cashier_header.h"

unsigned int read_queue_size();

// functions for openGl
void display(void);
void reshape(int w, int h);
void timer(int z);
void drawReceptionHall();
void drawEnterOfQueues();
void drawQueue();
void drawWattingHall();
void drawMetalDetector();
void drawChairs();
void drawTellers();
void drawText();
void renderText(int, int, void *, char *);
void readDataFromSharedMemory();


int peopleInTheInnerHall = 0;
float cashier1_totalCount = 0;
float cashier3_totalCount = 0;
float cashier2_totalCnt=0;

      int   receptionHallMaleCount = 0,
        receptionHallFemaleCount = 0,
        birthCertificateCnt = 0,
        bakeryCnt = 0,
        dair_eggsCnt = 0,
        packagedCnt = 0,

        unservedCostumersCnt=0,
        cashier_stoppedCnt=0,
        fram_rate = 1000 / 120;

float mouseX = 0.0f;
float mouseY = 0.0f;
int windowWidth = 1800;
int windowHeight = 1000;


char *drawer_current_time = "";


void mouseMotion(int x, int y) {
    // Convert pixel coordinates (x, y) to OpenGL coordinates
    mouseX = (x / (float)windowWidth) * 40.0f - 20.0f;
    mouseY = ((windowHeight - y) / (float)windowHeight) * 40.0f - 20.0f;
    glutPostRedisplay(); // Add this line to ensure the display function is called to update the rendering
}



int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(1800, 1000);
    glutCreateWindow("Super Market");
    glLineWidth(5);
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutPassiveMotionFunc(mouseMotion);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}
void display(void) {
    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // All your drawing operations.
    drawReceptionHall();
    drawEnterOfQueues();
    drawQueue();
    drawWattingHall();
    drawMetalDetector();
    drawChairs();
    drawTellers();
    drawText();

    // Render the mouse coordinates
    char mouseCoords[50];
    sprintf(mouseCoords, "Mouse X: %.2f Y: %.2f", mouseX, mouseY);
    renderText(10, -18, GLUT_BITMAP_TIMES_ROMAN_24, mouseCoords);

    // Swap the buffers at the very end after all drawing commands
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-20, 20, -20, 20);
    glMatrixMode(GL_MODELVIEW);
}
void timer(int z)
{
    glutTimerFunc(fram_rate, timer, 0);
    glutPostRedisplay();
    readDataFromSharedMemory();
}
void drawReceptionHall()
{
    glBegin(GL_LINES);
    glVertex2f(-20.0, -13.0);
    glVertex2f(-10.0, -13.0);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(-20.0, 13.0);
    glVertex2f(-10.0, 13.0);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(-10.0, 13.0);
    glVertex2f(-10.0, 11.0);
    glEnd();
    glFlush();

    glBegin(GL_LINES);
    glVertex2f(-10.0, -13.0);
    glVertex2f(-10.0, -10.0);
    glEnd();
    glFlush();




}
void drawBox(float x, float y, float width, float height) {
    // Top line
    glBegin(GL_LINES);
    glColor3f(1.0, 1.0, 255);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glEnd();

    // Bottom line
    glBegin(GL_LINES);
    glVertex2f(x, y - height);
    glVertex2f(x + width, y - height);
    glEnd();

    // Left line
    glBegin(GL_LINES);
    glVertex2f(x, y);
    glVertex2f(x, y - height);
    glEnd();

    // Right line
    glBegin(GL_LINES);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y - height);
    glEnd();

    glFlush();
}

void drawEnterOfQueues() // cashier border
{
    float startY = 13.0;
    float totalSpace = 26.0; // startY - (-13.0)
    int numLines = 4;
    float space = totalSpace / (numLines - 1);

    // Calculate y-coordinates for each cashier
    float y1 = startY - 1 * space;
    float y2 = startY - 2 * space;
    float y3 = startY - 3 * space;

    // Adjust box sizes if necessary
    float boxWidth = 2.0;
    float boxHeight = 2.0;

    // Draw box for cashier 1
    drawBox(-7.0, y1 + 2, boxWidth, boxHeight);

    // Draw box for cashier 2
    drawBox(-7.0, y2 + 2, boxWidth, boxHeight);

    // Draw box for cashier 3
    drawBox(-7.0, y3 + 2, boxWidth, boxHeight);
}



void drawQueue() // cashier1 queue,  cashier2 queue , cashier3 queue
{

    float startY = 13.0;   // Starting y-coordinate (upper limit)
    float endY = -13.0;    // Ending y-coordinate (lower limit)
    int numLines = 4;      // Number of lines
    float totalSpace = startY - endY;
    float space = totalSpace / (numLines - 1);

    for (int i = 0; i < numLines; i++) {
        float y = startY - i * space;

        glBegin(GL_LINES);
        glColor3f(0, 255, 0);
        glVertex2f(-10.0, y);
        glVertex2f(-2.0, y);
        glEnd();

        // Drawing points representing cashiers
        if (i > 0) {
            glPointSize(10);
            glBegin(GL_POINTS);
            glColor3f(0, 255, 0);
            glVertex2f(-6.0, y + 1);
            glEnd();
        }
    }

    glFlush();


}


void drawWattingHall()
{
    glColor3ub(255, 0, 255);

    glBegin(GL_LINES);
    glVertex2f(-2.0, 13.0);
    glVertex2f(-2.0, 11.0);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(-2.0, -13.0);
    glVertex2f(-2.0, -10.0);
    glEnd();


    glBegin(GL_LINES);
    glVertex2f(-2.0, 13.0);
    glVertex2f(20.0, 13.0);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(-2.0, -13.0);
    glVertex2f(20.0, -13.0);
    glEnd();
    glFlush();
}
void drawMetalDetector()
{

    glPointSize(20);
    glBegin(GL_POINTS);
    glColor3f(0.5, 255, 0);
    glVertex2f(4, -13.0);
    glEnd();
    glFlush();

    glPointSize(20);
    glBegin(GL_POINTS);
    glColor3f(255, 0, 0);
    glVertex2f(-15, -13.0);
    glEnd();
    glFlush();

}

// y : -9-9 ,,,, x : 0-8
void drawChairs()
{

    int peopleCount = 0, jTemp = 0, iTemp = -9;

    glPointSize(5);
    glBegin(GL_POINTS);
    glColor3f(1.0, 0.1, 0.1);
    for (int i = 8; i > -1; i--)
    {
        iTemp = i;
        int f = 0;
        for (int j = -9; j < 8; j++)
        {
            jTemp = j;
            glVertex2i(i, j);
            peopleCount++;
            if (peopleCount == peopleInTheInnerHall)
            {
                f = 1;
                break;
            }
        }
        if (f)
            break;
    }
    glEnd();



    glPointSize(5);
    glBegin(GL_POINTS);
    glColor3f(0.0, 0.0, 1.0);
    for (int i = iTemp; i > -1; i--)
    {

        for (int j = jTemp + 1; j < 8; j++)
        {
            glVertex2i(i, j);
        }
        jTemp = -10;
    }
    glEnd();
    glFlush();




}
void drawTellers()
{
    glBegin(GL_POLYGON_BIT);
    glColor3f(7, 0.3, 10);
    glVertex2i(20, 12);
    glVertex2i(12, 12);
    glVertex2i(20, 6);
    glVertex2i(12, 6);
    glEnd();

    glBegin(GL_POLYGON_BIT);
    glColor3f(0, 0, 255);
    glVertex2i(20, 6);
    glVertex2i(12, 6);
    glVertex2i(20, 0);
    glVertex2i(12, 0);
    glEnd();

    glBegin(GL_POLYGON_BIT);
    glColor3f(0.3, 0.8, 0.2);
    glVertex2i(20, 0);
    glVertex2i(12, 0);
    glVertex2i(20, -6);
    glVertex2i(12, -6);
    glEnd();

    glBegin(GL_POLYGON_BIT);
    glColor3f(0.356, 0.514, 0.999);
    glVertex2i(20, -6);
    glVertex2i(12, -6);
    glVertex2i(20, -12);
    glVertex2i(12, -12);
    glEnd();
}

void drawText()
{
    glColor3f(1.0, 0.8, 0.5);
    char
            customerCount[30] = "customerCount : ",
            cashierCount[30] = "cashierCount : ",
            cashier1_total[30] = "cashier1 Total Sales : $ ",
            cashier2_total[30]="cashier2 Total Sales : $ ",
            cashier3_total[30] = "cashier3 Total Sales  : $ ",
            freshProduce[30] = "Milk : ",
            bakery[30] = " Eggs : ",
            dair_eggs[30] = "Bread  : ",
            packaged[30] = "Apples : ",
            unservedCostumers[30]="Costumers unserved   : ",
            cashier_stopped[30]="Cashier stopped : ",
            numberOfPeopleIncashier1_total[5] = "",
            numberOfPeopleInFemalQueue[5] = "",
            numberOfMaleInTheReceptionHall[5] = "",
            numberOfFemaleInTheReceptionHall[5] = "",
            numberOfBusyTellersForBirthCertificate[3]="",
            numberOfBusyTellersForbakery[3]="",
            numberOfBusyTellersFordair_eggs[3]="",
            numberOfBusyTellersForpackaged[3]="",
            numberOfcashier2_total[3]="",
            numberOfunservedCostumers[3]="",
            numberOfcashier_stopped[3]="";

    sprintf(numberOfPeopleIncashier1_total, "%0.2f", cashier1_totalCount);
    sprintf(numberOfcashier2_total, "%0.2f", cashier2_totalCnt);
    sprintf(numberOfPeopleInFemalQueue, "%0.2f", cashier3_totalCount);
    sprintf(numberOfFemaleInTheReceptionHall, "%d", receptionHallFemaleCount);
    sprintf(numberOfMaleInTheReceptionHall, "%d", peopleInTheInnerHall);

    sprintf(numberOfBusyTellersForBirthCertificate, "%d", birthCertificateCnt);
    sprintf(numberOfBusyTellersForbakery, "%d", bakeryCnt);
    sprintf(numberOfBusyTellersFordair_eggs, "%d", dair_eggsCnt);
    sprintf(numberOfBusyTellersForpackaged, "%d", packagedCnt);


    sprintf(numberOfunservedCostumers, "%d", unservedCostumersCnt);
    sprintf(numberOfcashier_stopped, "%d", cashier_stoppedCnt);


    strcat(cashier1_total, numberOfPeopleIncashier1_total);
    strcat(cashier3_total, numberOfPeopleInFemalQueue);
    strcat(customerCount, numberOfMaleInTheReceptionHall);
    strcat(cashierCount, numberOfFemaleInTheReceptionHall);

    strcat(freshProduce, numberOfBusyTellersForBirthCertificate);
    strcat(bakery, numberOfBusyTellersForbakery);
    strcat(dair_eggs, numberOfBusyTellersFordair_eggs);
    strcat(packaged, numberOfBusyTellersForpackaged);

    strcat(cashier2_total,numberOfcashier2_total);
    strcat(unservedCostumers, numberOfunservedCostumers);
    strcat(cashier_stopped, numberOfcashier_stopped);

    renderText(-9, 12, GLUT_BITMAP_TIMES_ROMAN_24, cashier1_total); // first cashier queue
    renderText(-9, 3, GLUT_BITMAP_TIMES_ROMAN_24, cashier2_total); // cashier2 total sales
    renderText(-9, -15, GLUT_BITMAP_TIMES_ROMAN_24, cashier3_total); // cashier3 queue
    renderText(2, 10, GLUT_BITMAP_TIMES_ROMAN_24, customerCount); //cousomer count
    renderText(-18, -2, GLUT_BITMAP_TIMES_ROMAN_24, cashierCount); // cahier count
    renderText(14, 9, GLUT_BITMAP_TIMES_ROMAN_24, freshProduce);
    renderText(14, 3, GLUT_BITMAP_TIMES_ROMAN_24, bakery);
    renderText(14, -3, GLUT_BITMAP_TIMES_ROMAN_24, dair_eggs);
    renderText(14, -9, GLUT_BITMAP_TIMES_ROMAN_24, packaged);
    renderText(13, 18, GLUT_BITMAP_TIMES_ROMAN_24, drawer_current_time);

    renderText(-8, 15, GLUT_BITMAP_TIMES_ROMAN_24, unservedCostumers);
    renderText(-18, 2, GLUT_BITMAP_TIMES_ROMAN_24, cashier_stopped); // cashier stopped
}

void renderText(int x, int y, void *font, char *string)
{
    glRasterPos2f(x, y);
    int len, i;
    len = strlen(string);
    for (i = 0; i < len && string[i] != '\0'; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}




// read data from shared memory in this function
void readDataFromSharedMemory()
{


     peopleInTheInnerHall = get_total_customers();


    cashier1_totalCount = get_total_sales(1); // for cahier total sales 1

    cashier2_totalCnt= get_total_sales(2); // for cahier total sales 2
    cashier3_totalCount = get_total_sales(3); // for cahier total sales 3

  //  receptionHallMaleCount = read_queue_size(OUTSIDE, MALE) ;
   // receptionHallFemaleCount =read_queue_size(OUTSIDE, FEMALE);
   // birthCertificateCnt = read_tellers_numbers(B);

   // bakeryCnt = read_tellers_numbers(T);
  //  dair_eggsCnt = read_tellers_numbers(R);
  //  packagedCnt = read_tellers_numbers(I);
  //  drawer_current_time = time_to_string();
   // unservedCostumersCnt= read_ends(UNSERVED);

   // cashier_stoppedCnt= read_ends(SATISFIED)


}


